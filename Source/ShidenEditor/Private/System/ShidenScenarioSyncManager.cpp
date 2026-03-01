// Copyright (c) 2026 HANON. All Rights Reserved.

#include "System/ShidenScenarioSyncManager.h"
#include "System/ShidenStructuredLog.h"
#include "System/ShidenEditorBlueprintLibrary.h"
#include "Scenario/ShidenScenario.h"
#include "Config/ShidenProjectConfig.h"
#include "Config/ShidenEditorConfig.h"
#include "Config/ShidenEditorUserConfig.h"
#include "System/ShidenPluginVersion.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"
#include "DirectoryWatcherModule.h"
#include "IDirectoryWatcher.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "UObject/SavePackage.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "ScopedTransaction.h"
#include "Containers/Ticker.h"
#include "Async/Async.h"
#include "Serialization/Csv/CsvParser.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

FDelegateHandle UShidenScenarioSyncManager::DirectoryWatcherHandle;
FDelegateHandle UShidenScenarioSyncManager::AssetRemovedHandle;
FDelegateHandle UShidenScenarioSyncManager::AssetAddedHandle;
IDirectoryWatcher* UShidenScenarioSyncManager::DirectoryWatcher = nullptr;
FString UShidenScenarioSyncManager::WatchedDirectoryPath;
UShidenScenarioSyncManager* UShidenScenarioSyncManager::Instance = nullptr;

namespace
{
	// Track recently exported files to prevent circular updates
	TSet<FString> RecentlyExportedFiles;
	FCriticalSection RecentlyExportedFilesLock;

	void MarkFileAsRecentlyExported(const FString& FilePath)
	{
		FScopeLock Lock(&RecentlyExportedFilesLock);
		FString NormalizedPath = FilePath;
		FPaths::NormalizeFilename(NormalizedPath);
		RecentlyExportedFiles.Add(NormalizedPath);

		// Schedule removal after a short delay to prevent circular updates
		AsyncTask(ENamedThreads::GameThread, [NormalizedPath]
		{
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([NormalizedPath](float) -> bool
			{
				FScopeLock ScopeLock(&RecentlyExportedFilesLock);
				RecentlyExportedFiles.Remove(NormalizedPath);
				return false; // Remove ticker after one execution
			}), 0.5f); // Wait 0.5 seconds before allowing re-import
		});
	}

	bool IsFileRecentlyExported(const FString& FilePath)
	{
		FScopeLock ScopeLock(&RecentlyExportedFilesLock);
		FString NormalizedPath = FilePath;
		FPaths::NormalizeFilename(NormalizedPath);
		return RecentlyExportedFiles.Contains(NormalizedPath);
	}

	bool IsEditorConfigValid(const TObjectPtr<const UShidenEditorConfig> EditorConfig)
	{
		return EditorConfig && EditorConfig->bEnableScenarioSync;
	}

	FString GetAbsoluteSyncPath(const TObjectPtr<const UShidenEditorConfig> EditorConfig)
	{
		if (!EditorConfig || EditorConfig->ScenarioSyncDirectoryPath.IsEmpty())
		{
			return FString();
		}

		FString AbsolutePath = EditorConfig->ScenarioSyncDirectoryPath;
		if (FPaths::IsRelative(AbsolutePath))
		{
			AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), AbsolutePath);
		}
		return AbsolutePath;
	}

	bool EnsureDirectoryExists(const FString& DirectoryPath)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (PlatformFile.DirectoryExists(*DirectoryPath))
		{
			return true;
		}
		if (PlatformFile.CreateDirectoryTree(*DirectoryPath))
		{
			return true;
		}
		SHIDEN_ERROR("Failed to create directory: {path}", *DirectoryPath);
		return false;
	}

	bool IsValidSyncFile(const FString& FilePath, const TObjectPtr<const UShidenEditorConfig> EditorConfig)
	{
		if (!EditorConfig)
		{
			return false;
		}
		const FString Extension = FPaths::GetExtension(FilePath).ToLower();
		const FString ExpectedExtension = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT("json") : TEXT("csv");
		return Extension == ExpectedExtension;
	}

	FString GetExpectedFileExtension(const TObjectPtr<const UShidenEditorConfig> EditorConfig)
	{
		if (!EditorConfig)
		{
			return FString();
		}
		return EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT("*.json") : TEXT("*.csv");
	}

	bool ExtractPluginVersionFromJson(const FString& FileContent, FShidenPluginVersion& OutVersion)
	{
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FString VersionStr;
			if (JsonObject->TryGetStringField(TEXT("PluginVersion"), VersionStr) && !VersionStr.IsEmpty())
			{
				FShidenPluginVersion ParsedVersion;
				if (UShidenEditorBlueprintLibrary::TryParseVersionString(VersionStr, ParsedVersion))
				{
					OutVersion = ParsedVersion;
					return true;
				}
			}
		}
		return false;
	}

	bool ExtractPluginVersionFromCsv(const FString& FileContent, FShidenPluginVersion& OutVersion)
	{
		TArray<FString> Lines;
		FileContent.ParseIntoArrayLines(Lines, true);
		for (const FString& Line : Lines)
		{
			if (Line.StartsWith(TEXT("#PluginVersion ")))
			{
				const FString VersionStr = Line.RightChop(15).TrimStartAndEnd();
				FShidenPluginVersion ParsedVersion;
				if (UShidenEditorBlueprintLibrary::TryParseVersionString(VersionStr, ParsedVersion))
				{
					OutVersion = ParsedVersion;
					return true;
				}
				break;
			}
		}
		return false;
	}

	bool HasCommandIdIssuesInCsv(const FString& FileContent)
	{
		const FCsvParser Parser(FileContent);
		const TArray<TArray<const TCHAR*>>& Rows = Parser.GetRows();
		bool bCommentEnd = false;
		bool bHeaderFound = false;
		TSet<FGuid> SeenGuids;

		for (const TArray<const TCHAR*>& Row : Rows)
		{
			// Skip empty rows
			if (Row.Num() == 0)
			{
				continue;
			}

			// Skip comment lines like "# comment"
			if (!bCommentEnd && FString(Row[0]).TrimStart().TrimEnd().StartsWith("#"))
			{
				continue;
			}

			bCommentEnd = true;

			// Skip header row
			if (!bHeaderFound)
			{
				bHeaderFound = true;
				continue;
			}

			// Check if row has sufficient columns and if CommandId (first column) is missing, invalid, or duplicate
			if (Row.Num() >= 4)
			{
				const FString CommandIdString = FString(Row[0]).TrimStartAndEnd();
				FGuid ParsedGuid;
				if (CommandIdString.IsEmpty() || !FGuid::Parse(CommandIdString, ParsedGuid))
				{
					return true; // Found a row with missing or invalid CommandId
				}

				if (SeenGuids.Contains(ParsedGuid))
				{
					return true; // Found a duplicate CommandId
				}
				SeenGuids.Add(ParsedGuid);
			}
		}

		return false;
	}

	bool HasCommandIdIssuesInJson(const FString& FileContent)
	{
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContent);
		TSharedPtr<FJsonObject> JsonObject;

		if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
		{
			return false;
		}

		const TArray<TSharedPtr<FJsonValue>>* CommandsArray = nullptr;
		if (!JsonObject->TryGetArrayField(TEXT("Commands"), CommandsArray))
		{
			return false;
		}

		TSet<FGuid> SeenGuids;

		for (const TSharedPtr<FJsonValue>& CommandValue : *CommandsArray)
		{
			const TSharedPtr<FJsonObject>* CommandObject = nullptr;
			if (CommandValue->TryGetObject(CommandObject) && CommandObject->IsValid())
			{
				FString CommandIdString;
				if (!(*CommandObject)->TryGetStringField(TEXT("CommandId"), CommandIdString))
				{
					return true; // CommandId field is missing
				}

				FGuid ParsedGuid;
				if (CommandIdString.IsEmpty() || !FGuid::Parse(CommandIdString, ParsedGuid))
				{
					return true; // CommandId is empty or invalid
				}

				if (SeenGuids.Contains(ParsedGuid))
				{
					return true; // Found a duplicate CommandId
				}
				SeenGuids.Add(ParsedGuid);
			}
		}

		return false;
	}

	bool LoadScenarioFromFile(const FString& FilePath, const TObjectPtr<const UShidenEditorConfig> EditorConfig, UShidenScenario*& OutScenario, FShidenPluginVersion& OutVersion)
	{
		if (!FPaths::FileExists(FilePath))
		{
			SHIDEN_WARNING("File does not exist: {path}", *FilePath);
			return false;
		}

		FString FileContent;
		if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
		{
			SHIDEN_ERROR("Failed to read file: {path}", *FilePath);
			return false;
		}

		OutVersion = FShidenPluginVersion(1, 0, 0); // Default version

		if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
		{
			if (!UShidenEditorBlueprintLibrary::TryConvertToScenarioFromJson(FileContent, OutScenario))
			{
				SHIDEN_ERROR("Failed to convert JSON to scenario: {path}", *FilePath);
				return false;
			}
			ExtractPluginVersionFromJson(FileContent, OutVersion);
		}
		else
		{
			OutScenario = UShidenEditorBlueprintLibrary::ConvertToScenarioFromCsv(FileContent);
			if (!OutScenario)
			{
				SHIDEN_ERROR("Failed to convert CSV to scenario: {path}", *FilePath);
				return false;
			}
			ExtractPluginVersionFromCsv(FileContent, OutVersion);
		}

		return OutScenario != nullptr;
	}

	void SyncAssetTimestampWithExternalFile(const FString& PackageFileName, const FString& ExternalFilePath)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		const FDateTime ExternalFileTimestamp = PlatformFile.GetTimeStamp(*ExternalFilePath);
		if (ExternalFileTimestamp != FDateTime::MinValue())
		{
			PlatformFile.SetTimeStamp(*PackageFileName, ExternalFileTimestamp);
		}
	}

	void SyncExternalFileTimestampWithAsset(const FString& ExternalFilePath, const TObjectPtr<const UShidenScenario> Scenario)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FString PackageFilename;
		if (FPackageName::TryConvertLongPackageNameToFilename(Scenario->GetPackage()->GetName(), PackageFilename, FPackageName::GetAssetPackageExtension()))
		{
			const FDateTime AssetTimestamp = PlatformFile.GetTimeStamp(*PackageFilename);
			if (AssetTimestamp != FDateTime::MinValue())
			{
				PlatformFile.SetTimeStamp(*ExternalFilePath, AssetTimestamp);
			}
		}
	}

	void UpdateScenarioData(const TObjectPtr<UShidenScenario> Target, const TObjectPtr<const UShidenScenario> Source)
	{
		Target->Note = Source->Note;
		Target->Commands = Source->Commands;
		Target->MacroParameterDefinitions = Source->MacroParameterDefinitions;
		Target->LocalVariableDefinitions = Source->LocalVariableDefinitions;
		Target->WidgetClassOverride = Source->WidgetClassOverride;
	}

	bool SaveScenarioPackage(const TObjectPtr<UShidenScenario> Scenario, const FString& FilePath)
	{
		const TObjectPtr<UPackage> Package = Scenario->GetPackage();
		if (!Package)
		{
			return false;
		}

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		const FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());

		if (UPackage::SavePackage(Package, Scenario, *PackageFileName, SaveArgs))
		{
			SyncAssetTimestampWithExternalFile(PackageFileName, FilePath);
			SHIDEN_LOG("Updated and saved scenario: {name}", *Scenario->GetName());
			return true;
		}

		SHIDEN_ERROR("Failed to save scenario package: {name}", *Scenario->GetName());
		return false;
	}

	void BroadcastSyncEvent()
	{
		if (const TObjectPtr<UShidenScenarioSyncManager> Manager = UShidenScenarioSyncManager::GetInstance())
		{
			Manager->OnScenarioSynced.Broadcast();
		}
	}

	bool IsFileNewerThanAsset(const FString& FilePath, const UShidenScenario* Scenario)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		const FDateTime FileModTime = PlatformFile.GetTimeStamp(*FilePath);

		if (const TObjectPtr<UPackage> Package = Scenario->GetPackage())
		{
			const FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());
			const FDateTime AssetModTime = PlatformFile.GetTimeStamp(*PackageFileName);
			return FileModTime > AssetModTime;
		}

		return false;
	}

	FString ConvertAssetPathToRelativePath(const FString& AssetPath)
	{
		FString RelativePath = AssetPath;

		if (RelativePath.StartsWith(TEXT("/Game/")))
		{
			RelativePath = RelativePath.RightChop(6);
		}
		else if (RelativePath.StartsWith(TEXT("/Script/")))
		{
			RelativePath = RelativePath.RightChop(8);
		}

		int32 DotIndex;
		if (RelativePath.FindLastChar(TEXT('.'), DotIndex))
		{
			RelativePath = RelativePath.Left(DotIndex);
		}

		return RelativePath;
	}

	FString GetValidFileName(const FString& RelativePath)
	{
		const FString FileName = FPaths::GetCleanFilename(RelativePath);
		return FPaths::MakeValidFileName(FileName);
	}

	FString ConvertFilePathToAssetPath(const FString& FilePath, const UShidenEditorConfig* EditorConfig)
	{
		FString AbsoluteSyncPath = GetAbsoluteSyncPath(EditorConfig);
		FString NormalizedFilePath = FilePath;

		FPaths::NormalizeFilename(NormalizedFilePath);
		FPaths::NormalizeFilename(AbsoluteSyncPath);

		FString RelativePath = NormalizedFilePath;
		if (RelativePath.StartsWith(AbsoluteSyncPath))
		{
			RelativePath = RelativePath.RightChop(AbsoluteSyncPath.Len());
			if (RelativePath.StartsWith(TEXT("/")))
			{
				RelativePath = RelativePath.RightChop(1);
			}
		}

		RelativePath = FPaths::GetPath(RelativePath) / FPaths::GetBaseFilename(RelativePath);
		return TEXT("/Game/") + RelativePath;
	}

	UShidenScenario* CreateNewScenarioAsset(const FString& AssetPath, const UShidenScenario* SourceScenario)
	{
		const TObjectPtr<UPackage> Package = CreatePackage(*AssetPath);
		if (!Package)
		{
			SHIDEN_ERROR("Failed to create package: {path}", *AssetPath);
			return nullptr;
		}

		const FString AssetName = FPaths::GetBaseFilename(AssetPath);
		const TObjectPtr<UShidenScenario> NewScenario = NewObject<UShidenScenario>(Package, *AssetName, RF_Public | RF_Standalone);
		if (!NewScenario)
		{
			SHIDEN_ERROR("Failed to create scenario asset: {name}", *AssetName);
			return nullptr;
		}

		NewScenario->ScenarioId = SourceScenario->ScenarioId;
		UpdateScenarioData(NewScenario, SourceScenario);
		// ReSharper disable once CppExpressionWithoutSideEffects
		Package->MarkPackageDirty();

		return NewScenario;
	}

	bool SaveNewScenarioPackage(const TObjectPtr<UShidenScenario> Scenario, const FString& FilePath)
	{
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		const FString PackageFileName = FPackageName::LongPackageNameToFilename(
			Scenario->GetPackage()->GetName(), FPackageName::GetAssetPackageExtension());

		if (!UPackage::SavePackage(Scenario->GetPackage(), Scenario, *PackageFileName, SaveArgs))
		{
			SHIDEN_ERROR("Failed to save new scenario package: {name}", *Scenario->GetName());
			return false;
		}

		SyncAssetTimestampWithExternalFile(PackageFileName, FilePath);
		FAssetRegistryModule::AssetCreated(Scenario);

		SHIDEN_LOG("Created new scenario asset: {name} (GUID: {guid})",
		           *Scenario->GetName(), *Scenario->ScenarioId.ToString());
		return true;
	}
	
	UShidenScenario* FindScenarioByGuid(const FGuid& ScenarioId)
	{
		const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
		if (!ProjectConfig)
		{
			return nullptr;
		}

		if (const FString* ScenarioPath = ProjectConfig->ScenarioPaths.Find(ScenarioId))
		{
			const FSoftObjectPath SoftPath(*ScenarioPath);
			return Cast<UShidenScenario>(SoftPath.TryLoad());
		}

		return nullptr;
	}

	struct FExternalChangeResult
	{
		int32 UpdatedCount = 0;
		int32 AddedCount = 0;

		bool HasChanges() const
		{
			return UpdatedCount > 0 || AddedCount > 0;
		}
	};

	FString GetSyncFilePathForScenario(const UShidenScenario* Scenario)
	{
		if (!Scenario)
		{
			return FString();
		}

		const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
		if (!EditorConfig)
		{
			return FString();
		}

		const FString Extension = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT(".json") : TEXT(".csv");
		const FString AssetPath = Scenario->GetPackage()->GetName();
		const FString RelativePath = ConvertAssetPathToRelativePath(AssetPath);
		const FString FileName = GetValidFileName(RelativePath);
		const FString DirectoryPath = FPaths::GetPath(RelativePath);
		const FString AbsolutePath = GetAbsoluteSyncPath(EditorConfig);
		const FString FullDirectory = FPaths::Combine(AbsolutePath, DirectoryPath);

		return FPaths::Combine(FullDirectory, FileName + Extension);
	}

	bool ImportAndUpdateScenario(const FString& FilePath)
	{
		const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
		if (!IsEditorConfigValid(EditorConfig))
		{
			return false;
		}

		// Check if the file has missing, invalid, or duplicate CommandIds before import
		FString FileContent;
		bool bHasCommandIdIssues = false;
		if (FFileHelper::LoadFileToString(FileContent, *FilePath))
		{
			if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
			{
				bHasCommandIdIssues = HasCommandIdIssuesInJson(FileContent);
			}
			else
			{
				bHasCommandIdIssues = HasCommandIdIssuesInCsv(FileContent);
			}
		}

		UShidenScenario* ImportedScenario = nullptr;
		FShidenPluginVersion SourcePluginVersion;

		if (!LoadScenarioFromFile(FilePath, EditorConfig, ImportedScenario, SourcePluginVersion))
		{
			return false;
		}

		bool bAnyCommandUpdated = false;
		UShidenEditorBlueprintLibrary::RedirectCommands(ImportedScenario, SourcePluginVersion, bAnyCommandUpdated);
		if (bAnyCommandUpdated)
		{
			SHIDEN_LOG("Commands were redirected during import: {path}", *FilePath);
		}

		if (!ImportedScenario || !ImportedScenario->ScenarioId.IsValid())
		{
			SHIDEN_ERROR("Imported scenario has invalid GUID: {path}", *FilePath);
			return false;
		}

		const TObjectPtr<UShidenScenario> ExistingScenario = FindScenarioByGuid(ImportedScenario->ScenarioId);
		if (!ExistingScenario)
		{
			SHIDEN_WARNING("No existing scenario found with GUID: {id}", *ImportedScenario->ScenarioId.ToString());
			return false;
		}

		// Update the existing scenario with transaction for undo support
		{
			FScopedTransaction Transaction(NSLOCTEXT("ShidenNamespace", "ImportScenarioFromExternalFile", "Import Scenario from External File"));
			ExistingScenario->Modify();
			UpdateScenarioData(ExistingScenario, ImportedScenario);
			// ReSharper disable once CppExpressionWithoutSideEffects
			ExistingScenario->MarkPackageDirty();
		}

		// If CommandIds were missing, invalid, or duplicated in the original file, write back the updated scenario
		if (bHasCommandIdIssues)
		{
			SHIDEN_LOG("Missing, invalid, or duplicate CommandIds detected in file, writing back updated GUIDs: {path}", *FilePath);

			FString UpdatedFileContent;
			bool bConverted;

			if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
			{
				bConverted = UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(ExistingScenario, UpdatedFileContent);
			}
			else
			{
				UpdatedFileContent = UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(ExistingScenario);
				bConverted = !UpdatedFileContent.IsEmpty();
			}

			if (bConverted)
			{
				if (FFileHelper::SaveStringToFile(UpdatedFileContent, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
				{
					SHIDEN_LOG("Successfully wrote back CommandIds to file: {path}", *FilePath);
					MarkFileAsRecentlyExported(FilePath);
				}
				else
				{
					SHIDEN_ERROR("Failed to write back CommandIds to file: {path}", *FilePath);
				}
			}
		}

		const TObjectPtr<const UShidenEditorUserConfig> UserConfig = GetDefault<UShidenEditorUserConfig>();
		if (UserConfig && UserConfig->bAutoSaveScenario)
		{
			return SaveScenarioPackage(ExistingScenario, FilePath);
		}

		SHIDEN_LOG("Updated scenario (not saved due to auto-save disabled): {name}", *ExistingScenario->GetName());
		return true;
	}

	bool CreateScenarioAssetFromFile(const FString& FilePath)
	{
		const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
		if (!IsEditorConfigValid(EditorConfig))
		{
			return false;
		}

		bool bNeedsWriteBack = false;

		// Check if the file has missing, invalid, or duplicate CommandIds before import
		FString FileContent;
		if (FFileHelper::LoadFileToString(FileContent, *FilePath))
		{
			const bool bHasCommandIdIssues = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON
				                                 ? HasCommandIdIssuesInJson(FileContent)
				                                 : HasCommandIdIssuesInCsv(FileContent);
			if (bHasCommandIdIssues)
			{
				SHIDEN_LOG("Missing, invalid, or duplicate CommandIds detected in file: {path}", *FilePath);
				bNeedsWriteBack = true;
			}
		}

		UShidenScenario* ImportedScenario = nullptr;
		FShidenPluginVersion SourcePluginVersion;

		if (!LoadScenarioFromFile(FilePath, EditorConfig, ImportedScenario, SourcePluginVersion))
		{
			return false;
		}

		if (!ImportedScenario || !ImportedScenario->ScenarioId.IsValid())
		{
			SHIDEN_ERROR("Imported scenario has invalid GUID: {path}", *FilePath);
			return false;
		}
		
		// Check if GUID conflicts with existing scenario, regenerate if necessary
		if (FindScenarioByGuid(ImportedScenario->ScenarioId))
		{
			const FGuid OldGuid = ImportedScenario->ScenarioId;
			ImportedScenario->ScenarioId = FGuid::NewGuid();
			SHIDEN_LOG("GUID conflict detected for {path}, regenerated from {oldGuid} to {newGuid}",
			           *FilePath, *OldGuid.ToString(), *ImportedScenario->ScenarioId.ToString());
			bNeedsWriteBack = true;
		}
		
		// Write back to file if ScenarioId or CommandIds were updated
		if (bNeedsWriteBack)
		{
			FString UpdatedFileContent;
			bool bConverted;
			
			if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
			{
				bConverted = UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(ImportedScenario, UpdatedFileContent);
			}
			else
			{
				UpdatedFileContent = UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(ImportedScenario);
				bConverted = !UpdatedFileContent.IsEmpty();
			}

			if (bConverted)
			{
				if (FFileHelper::SaveStringToFile(UpdatedFileContent, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
				{
					SHIDEN_LOG("Successfully wrote back updated GUIDs to file: {path}", *FilePath);
					MarkFileAsRecentlyExported(FilePath);
				}
				else
				{
					SHIDEN_ERROR("Failed to write back updated GUIDs to file: {path}", *FilePath);
				}
			}
		}

		bool bAnyCommandUpdated = false;
		UShidenEditorBlueprintLibrary::RedirectCommands(ImportedScenario, SourcePluginVersion, bAnyCommandUpdated);

		const FString AssetPath = ConvertFilePathToAssetPath(FilePath, EditorConfig);
		const TObjectPtr<UShidenScenario> NewScenario = CreateNewScenarioAsset(AssetPath, ImportedScenario);
		if (!NewScenario)
		{
			return false;
		}

		return SaveNewScenarioPackage(NewScenario, FilePath);
	}

	void ProcessExternalFile(const FString& FilePath, const UShidenEditorConfig* EditorConfig, FExternalChangeResult& Result)
	{
		UShidenScenario* ImportedScenario = nullptr;
		FShidenPluginVersion SourcePluginVersion;

		if (!LoadScenarioFromFile(FilePath, EditorConfig, ImportedScenario, SourcePluginVersion))
		{
			return;
		}

		if (!ImportedScenario || !ImportedScenario->ScenarioId.IsValid())
		{
			return;
		}

		// Find existing scenario by expected asset path instead of GUID
		const FString ExpectedAssetPath = ConvertFilePathToAssetPath(FilePath, EditorConfig);
		const FSoftObjectPath SoftPath(ExpectedAssetPath);
		const TObjectPtr<const UShidenScenario> ExistingScenario = Cast<UShidenScenario>(SoftPath.TryLoad());
		if (!ExistingScenario)
		{
			SHIDEN_LOG("New file detected, creating asset: {path}", *FilePath);
			if (CreateScenarioAssetFromFile(FilePath))
			{
				Result.AddedCount++;
			}
			return;
		}

		if (IsFileNewerThanAsset(FilePath, ExistingScenario))
		{
			SHIDEN_LOG("File is newer than asset, importing: {path}", *FilePath);
			if (ImportAndUpdateScenario(FilePath))
			{
				Result.UpdatedCount++;
			}
		}
	}

	void CheckForExternalChanges()
	{
		const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
		if (!IsEditorConfigValid(EditorConfig) || EditorConfig->ScenarioSyncDirectoryPath.IsEmpty())
		{
			return;
		}

		const FString AbsolutePath = GetAbsoluteSyncPath(EditorConfig);
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (!PlatformFile.DirectoryExists(*AbsolutePath))
		{
			return;
		}

		const FString Extension = GetExpectedFileExtension(EditorConfig);
		TArray<FString> FoundFiles;
		IFileManager::Get().FindFilesRecursive(FoundFiles, *AbsolutePath, *Extension, true, false);

		SHIDEN_LOG("Checking for external changes in {fileCount} files...", FoundFiles.Num());

		FExternalChangeResult Result;

		for (const FString& FilePath : FoundFiles)
		{
			ProcessExternalFile(FilePath, EditorConfig, Result);
		}

		if (Result.HasChanges())
		{
			SHIDEN_LOG("External changes summary - Updated: {updated}, Added: {added}",
			           Result.UpdatedCount, Result.AddedCount);
			BroadcastSyncEvent();
		}
	}

}

UShidenScenarioSyncManager* UShidenScenarioSyncManager::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<UShidenScenarioSyncManager>();
		Instance->SetFlags(RF_Transactional); // Enable undo/redo support
		Instance->AddToRoot(); // Prevent garbage collection
	}
	return Instance;
}

void UShidenScenarioSyncManager::PostEditUndo()
{
	Super::PostEditUndo();
	// Broadcast events for Editor Utility Widgets
	if (const TObjectPtr<UShidenScenarioSyncManager> Manager = GetInstance())
	{
		Manager->OnPostUndo.Broadcast();
	}
}

bool UShidenScenarioSyncManager::ExportAllScenarios()
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!IsEditorConfigValid(EditorConfig))
	{
		return false;
	}

	if (EditorConfig->ScenarioSyncDirectoryPath.IsEmpty())
	{
		SHIDEN_WARNING("Scenario sync directory path is empty");
		return false;
	}

	const FString AbsolutePath = GetAbsoluteSyncPath(EditorConfig);
	if (!EnsureDirectoryExists(AbsolutePath))
	{
		return false;
	}

	bool bAllSuccessful = true;

	// Get all scenario assets
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> ScenarioAssets;
	AssetRegistry.GetAssetsByClass(UShidenScenario::StaticClass()->GetClassPathName(), ScenarioAssets);

	SHIDEN_LOG("Exporting {num} scenarios to {path}", ScenarioAssets.Num(), *EditorConfig->ScenarioSyncDirectoryPath);

	for (const FAssetData& AssetData : ScenarioAssets)
	{
		if (TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(AssetData.GetAsset()))
		{
			if (!ExportScenario(Scenario))
			{
				SHIDEN_WARNING("Failed to export scenario: {path}", *AssetData.GetObjectPathString());
				bAllSuccessful = false;
			}
		}
	}

	return bAllSuccessful;
}

bool UShidenScenarioSyncManager::ExportScenario(UShidenScenario* Scenario)
{
	if (!Scenario)
	{
		return false;
	}

	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!IsEditorConfigValid(EditorConfig))
	{
		return false;
	}

	SHIDEN_LOG("Exporting scenario '{name}' with GUID: {guid}", *Scenario->GetName(), *Scenario->ScenarioId.ToString());

	FString ExportData;
	if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
	{
		if (!UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(Scenario, ExportData))
		{
			SHIDEN_ERROR("Failed to convert scenario to JSON: {name}", *Scenario->GetName());
			return false;
		}
	}
	else
	{
		ExportData = UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(Scenario);
		if (ExportData.IsEmpty())
		{
			SHIDEN_ERROR("Failed to convert scenario to CSV: {name}", *Scenario->GetName());
			return false;
		}
	}

	const FString FilePath = GetSyncFilePathForScenario(Scenario);
	const FString DirectoryPath = FPaths::GetPath(FilePath);

	if (!EnsureDirectoryExists(DirectoryPath))
	{
		return false;
	}

	if (!FFileHelper::SaveStringToFile(ExportData, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
	{
		SHIDEN_ERROR("Failed to save file: {path}", *FilePath);
		return false;
	}

	SyncExternalFileTimestampWithAsset(FilePath, Scenario);

	// Mark this file as recently exported to prevent circular updates
	MarkFileAsRecentlyExported(FilePath);

	SHIDEN_LOG("Exported scenario to: {path}", *FilePath);
	return true;
}

void UShidenScenarioSyncManager::OnAssetAdded(const FAssetData& AssetData)
{
	// Check if the added asset is a ShidenScenario
	if (AssetData.AssetClassPath != UShidenScenario::StaticClass()->GetClassPathName())
	{
		return;
	}

	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!IsEditorConfigValid(EditorConfig))
	{
		return;
	}

	// Load the scenario to export it
	const TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(AssetData.GetAsset());
	if (!Scenario)
	{
		SHIDEN_WARNING("Failed to load scenario asset for export: {path}", *AssetData.GetObjectPathString());
		return;
	}

	const FString FilePath = GetSyncFilePathForScenario(Scenario);
	if (FilePath.IsEmpty())
	{
		SHIDEN_WARNING("Could not determine file path for scenario: {name}", *Scenario->GetName());
		return;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*FilePath))
	{
		SHIDEN_VERBOSE("External file already exists, no export needed: {path}", *FilePath);
		return;
	}

	// Export the scenario to external file
	if (ExportScenario(Scenario))
	{
		SHIDEN_LOG("Exported external file for added scenario: {path}", *FilePath);
	}
	else
	{
		SHIDEN_ERROR("Failed to export external file: {path}", *FilePath);
	}
}

void UShidenScenarioSyncManager::OnAssetRemoved(const FAssetData& AssetData)
{
	// Check if the removed asset is a ShidenScenario
	if (AssetData.AssetClassPath != UShidenScenario::StaticClass()->GetClassPathName())
	{
		return;
	}

	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!IsEditorConfigValid(EditorConfig))
	{
		return;
	}

	// Load the scenario to get its file path
	const TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(AssetData.GetAsset());
	if (!Scenario)
	{
		SHIDEN_WARNING("Failed to load scenario asset for deletion: {path}", *AssetData.GetObjectPathString());
		return;
	}

	const FString FilePath = GetSyncFilePathForScenario(Scenario);
	if (FilePath.IsEmpty())
	{
		SHIDEN_WARNING("Could not determine file path for scenario: {name}", *Scenario->GetName());
		return;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.FileExists(*FilePath))
	{
		SHIDEN_VERBOSE("External file does not exist, no deletion needed: {path}", *FilePath);
		return;
	}

	// Delete the external file
	if (PlatformFile.DeleteFile(*FilePath))
	{
		SHIDEN_LOG("Deleted external file for removed scenario: {path}", *FilePath);
	}
	else
	{
		SHIDEN_ERROR("Failed to delete external file: {path}", *FilePath);
	}
}

void UShidenScenarioSyncManager::StartWatchingDirectory()
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!IsEditorConfigValid(EditorConfig) || EditorConfig->ScenarioSyncDirectoryPath.IsEmpty())
	{
		return;
	}

	CheckForExternalChanges();

	// Register asset addition and deletion callbacks
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	AssetAddedHandle = AssetRegistry.OnAssetAdded().AddStatic(&UShidenScenarioSyncManager::OnAssetAdded);
	AssetRemovedHandle = AssetRegistry.OnAssetRemoved().AddStatic(&UShidenScenarioSyncManager::OnAssetRemoved);
	SHIDEN_LOG("Registered asset addition and removal callbacks");

	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>("DirectoryWatcher");
	DirectoryWatcher = DirectoryWatcherModule.Get();

	if (DirectoryWatcher == nullptr)
	{
		return;
	}

	const IDirectoryWatcher::FDirectoryChanged Delegate = IDirectoryWatcher::FDirectoryChanged::CreateLambda(
		[](const TArray<FFileChangeData>& FileChanges)
		{
			if (FileChanges.Num() == 0)
			{
				return;
			}

			bool bAnyFileProcessed = false;
			for (const FFileChangeData& FileChange : FileChanges)
			{
				switch (FileChange.Action)
				{
				case FFileChangeData::FCA_Modified:
					if (!IsFileRecentlyExported(FileChange.Filename))
					{
						const TObjectPtr<const UShidenEditorConfig> ShidenEditorConfig = GetDefault<UShidenEditorConfig>();
						if (IsEditorConfigValid(ShidenEditorConfig) && IsValidSyncFile(FileChange.Filename, ShidenEditorConfig))
						{
							SHIDEN_LOG("File modified detected: {path}", *FileChange.Filename);
							ImportAndUpdateScenario(FileChange.Filename);
							bAnyFileProcessed = true;
						}
					}
					break;
				default:
					break;
				}
			}

			if (bAnyFileProcessed)
			{
				BroadcastSyncEvent();
			}
		}
	);

	WatchedDirectoryPath = GetAbsoluteSyncPath(EditorConfig);

	if (DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(WatchedDirectoryPath, Delegate, DirectoryWatcherHandle))
	{
		SHIDEN_LOG("Started watching directory: {path}", *WatchedDirectoryPath);
	}
	else
	{
		SHIDEN_WARNING("Failed to start watching directory: {path}", *WatchedDirectoryPath);
	}
}

void UShidenScenarioSyncManager::StopWatchingDirectory()
{
	// Unregister asset addition and deletion callbacks
	if (AssetAddedHandle.IsValid() || AssetRemovedHandle.IsValid())
	{
		// Check if the module is still loaded instead of trying to load it
		// During shutdown, AssetRegistry may already be unloaded
		if (const FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>("AssetRegistry"))
		{
			IAssetRegistry& AssetRegistry = AssetRegistryModule->Get();

			if (AssetAddedHandle.IsValid())
			{
				AssetRegistry.OnAssetAdded().Remove(AssetAddedHandle);
				AssetAddedHandle.Reset();
			}

			if (AssetRemovedHandle.IsValid())
			{
				AssetRegistry.OnAssetRemoved().Remove(AssetRemovedHandle);
				AssetRemovedHandle.Reset();
			}
		}
		else
		{
			// Module already unloaded, just reset the handles
			if (AssetAddedHandle.IsValid())
			{
				AssetAddedHandle.Reset();
			}
			if (AssetRemovedHandle.IsValid())
			{
				AssetRemovedHandle.Reset();
			}
		}

		SHIDEN_LOG("Unregistered asset addition and removal callbacks");
	}

	if (DirectoryWatcher != nullptr && DirectoryWatcherHandle.IsValid())
	{
		if (!WatchedDirectoryPath.IsEmpty())
		{
			DirectoryWatcher->UnregisterDirectoryChangedCallback_Handle(WatchedDirectoryPath, DirectoryWatcherHandle);
			SHIDEN_LOG("Stopped watching directory: {path}", *WatchedDirectoryPath);
		}
		DirectoryWatcherHandle.Reset();
		WatchedDirectoryPath.Empty();
	}
}

#undef LOCTEXT_NAMESPACE
