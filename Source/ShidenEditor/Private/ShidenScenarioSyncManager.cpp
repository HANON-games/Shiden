// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenScenarioSyncManager.h"
#include "System/ShidenStructuredLog.h"
#include "ShidenEditorBlueprintLibrary.h"
#include "Scenario/ShidenScenario.h"
#include "Config/ShidenProjectConfig.h"
#include "ShidenEditorConfig.h"
#include "ShidenPluginVersion.h"
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
#include "ObjectTools.h"

FDelegateHandle UShidenScenarioSyncManager::DirectoryWatcherHandle;
IDirectoryWatcher* UShidenScenarioSyncManager::DirectoryWatcher = nullptr;
FString UShidenScenarioSyncManager::WatchedDirectoryPath;
UShidenScenarioSyncManager* UShidenScenarioSyncManager::Instance = nullptr;

UShidenScenarioSyncManager* UShidenScenarioSyncManager::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<UShidenScenarioSyncManager>();
		Instance->AddToRoot(); // Prevent garbage collection
	}
	return Instance;
}

bool UShidenScenarioSyncManager::ExportAllScenarios()
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync)
	{
		return false;
	}

	if (EditorConfig->ScenarioSyncDirectoryPath.Path.IsEmpty())
	{
		SHIDEN_WARNING("Scenario sync directory path is empty");
		return false;
	}

	// Create the sync directory if it doesn't exist
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Convert to absolute path (using Content directory as base for relative paths)
	FString AbsolutePath = EditorConfig->ScenarioSyncDirectoryPath.Path;
	if (FPaths::IsRelative(AbsolutePath))
	{
		AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir(), AbsolutePath);
	}

	if (!PlatformFile.DirectoryExists(*AbsolutePath))
	{
		if (!PlatformFile.CreateDirectoryTree(*AbsolutePath))
		{
			SHIDEN_ERROR("Failed to create sync directory: {path}", *AbsolutePath);
			return false;
		}
	}

	bool bAllSuccessful = true;

	// Get all scenario assets
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> ScenarioAssets;
	AssetRegistry.GetAssetsByClass(UShidenScenario::StaticClass()->GetClassPathName(), ScenarioAssets);

	SHIDEN_LOG("Exporting {num} scenarios to {path}", ScenarioAssets.Num(), *EditorConfig->ScenarioSyncDirectoryPath.Path);

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
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync)
	{
		return false;
	}

	SHIDEN_LOG("Exporting scenario '{name}' with GUID: {guid}", *Scenario->GetName(), *Scenario->ScenarioId.ToString());

	FString ExportData;
	if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
	{
		if (!UShidenEditorBlueprintLibrary::TryConvertToJsonFromScenario(Scenario, true, ExportData))
		{
			SHIDEN_ERROR("Failed to convert scenario to JSON: {name}", *Scenario->GetName());
			return false;
		}
	}
	else
	{
		ExportData = UShidenEditorBlueprintLibrary::ConvertToCsvFromScenario(Scenario, true);
		if (ExportData.IsEmpty())
		{
			SHIDEN_ERROR("Failed to convert scenario to CSV: {name}", *Scenario->GetName());
			return false;
		}
	}

	const FString FilePath = GetSyncFilePathForScenario(Scenario);

	// Create directory if it doesn't exist
	const FString DirectoryPath = FPaths::GetPath(FilePath);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*DirectoryPath))
	{
		if (!PlatformFile.CreateDirectoryTree(*DirectoryPath))
		{
			SHIDEN_ERROR("Failed to create directory: {path}", *DirectoryPath);
			return false;
		}
	}

	if (!FFileHelper::SaveStringToFile(ExportData, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
	{
		SHIDEN_ERROR("Failed to save file: {path}", *FilePath);
		return false;
	}

	SHIDEN_LOG("Exported scenario to: {path}", *FilePath);
	return true;
}

bool UShidenScenarioSyncManager::ImportAndUpdateScenario(const FString& FilePath)
{
	if (!FPaths::FileExists(FilePath))
	{
		SHIDEN_WARNING("File does not exist: {path}", *FilePath);
		return false;
	}

	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync)
	{
		return false;
	}

	// Read file content
	FString FileContent;
	if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		SHIDEN_ERROR("Failed to read file: {path}", *FilePath);
		return false;
	}

	// Convert from CSV or JSON to scenario
	UShidenScenario* ImportedScenario = nullptr;
	FShidenPluginVersion SourcePluginVersion(1, 0, 0); // Default to 1.0.0 if unknown

	if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
	{
		if (!UShidenEditorBlueprintLibrary::TryConvertToScenarioFromJson(FileContent, ImportedScenario))
		{
			SHIDEN_ERROR("Failed to convert JSON to scenario: {path}", *FilePath);
			return false;
		}

		// Extract PluginVersion from JSON
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
					SourcePluginVersion = ParsedVersion;
				}
			}
		}
	}
	else
	{
		ImportedScenario = UShidenEditorBlueprintLibrary::ConvertToScenarioFromCsv(FileContent);
		if (!ImportedScenario)
		{
			SHIDEN_ERROR("Failed to convert CSV to scenario: {path}", *FilePath);
			return false;
		}

		// Extract PluginVersion from CSV comments (format: "#PluginVersion x.y.z")
		TArray<FString> Lines;
		FileContent.ParseIntoArrayLines(Lines, true);
		for (const FString& Line : Lines)
		{
			if (Line.StartsWith(TEXT("#PluginVersion ")))
			{
				const FString VersionStr = Line.RightChop(16).TrimStartAndEnd();
				FShidenPluginVersion ParsedVersion;
				if (UShidenEditorBlueprintLibrary::TryParseVersionString(VersionStr, ParsedVersion))
				{
					SourcePluginVersion = ParsedVersion;
				}
				break;
			}
		}
	}

	// Apply command redirector based on source plugin version
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

	// Find existing scenario by GUID
	const TObjectPtr<UShidenScenario> ExistingScenario = FindScenarioByGuid(ImportedScenario->ScenarioId);
	if (!ExistingScenario)
	{
		SHIDEN_WARNING("No existing scenario found with GUID: {id}", *ImportedScenario->ScenarioId.ToString());
		return false;
	}

	// Update the existing scenario
	ExistingScenario->Modify();
	ExistingScenario->Note = ImportedScenario->Note;
	ExistingScenario->Commands = ImportedScenario->Commands;
	ExistingScenario->MacroParameterDefinitions = ImportedScenario->MacroParameterDefinitions;
	ExistingScenario->LocalVariableDefinitions = ImportedScenario->LocalVariableDefinitions;
	ExistingScenario->WidgetClassOverride = ImportedScenario->WidgetClassOverride;

	// Mark package as dirty and save
	// ReSharper disable once CppExpressionWithoutSideEffects
	ExistingScenario->MarkPackageDirty();

	if (const TObjectPtr<UPackage> Package = ExistingScenario->GetPackage())
	{
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		const FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());

		if (UPackage::SavePackage(Package, ExistingScenario, *PackageFileName, SaveArgs))
		{
			SHIDEN_LOG("Updated and saved scenario: {name}", *ExistingScenario->GetName());
			return true;
		}

		SHIDEN_ERROR("Failed to save scenario package: {name}", *ExistingScenario->GetName());
		return false;
	}

	return false;
}

void UShidenScenarioSyncManager::StartWatchingDirectory()
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync || EditorConfig->ScenarioSyncDirectoryPath.Path.IsEmpty())
	{
		return;
	}

	// Check for changes made while the editor was closed
	CheckForExternalChanges();

	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>("DirectoryWatcher");
	DirectoryWatcher = DirectoryWatcherModule.Get();

	if (DirectoryWatcher != nullptr)
	{
		const IDirectoryWatcher::FDirectoryChanged Delegate = IDirectoryWatcher::FDirectoryChanged::CreateLambda(
			[](const TArray<FFileChangeData>& FileChanges)
			{
				if (FileChanges.Num() == 0)
				{
					return;
				}

				for (const FFileChangeData& FileChange : FileChanges)
				{
					switch (FileChange.Action)
					{
					case FFileChangeData::FCA_Added:
						OnFileAdded(FileChange.Filename);
						break;
					case FFileChangeData::FCA_Modified:
						OnFileModified(FileChange.Filename);
						break;
					case FFileChangeData::FCA_Removed:
						OnFileRemoved(FileChange.Filename);
						break;
					default:
						break;
					}
				}

				// Broadcast events for Editor Utility Widgets
				if (const UShidenScenarioSyncManager* Manager = GetInstance())
				{
					// Broadcast sync event for added/modified/removed files
					Manager->OnScenarioSynced.Broadcast();
				}
			}
		);

		// Convert to absolute path
		FString AbsolutePath = EditorConfig->ScenarioSyncDirectoryPath.Path;
		if (FPaths::IsRelative(AbsolutePath))
		{
			AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir(), AbsolutePath);
		}

		// Store the absolute path for later use in StopWatchingDirectory
		WatchedDirectoryPath = AbsolutePath;

		if (DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(AbsolutePath, Delegate, DirectoryWatcherHandle))
		{
			SHIDEN_LOG("Started watching directory: {path}", *AbsolutePath);
		}
		else
		{
			SHIDEN_WARNING("Failed to start watching directory: {path}", *AbsolutePath);
		}
	}
}

void UShidenScenarioSyncManager::StopWatchingDirectory()
{
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

void UShidenScenarioSyncManager::CheckForExternalChanges()
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync || EditorConfig->ScenarioSyncDirectoryPath.Path.IsEmpty())
	{
		return;
	}

	// Convert to absolute path
	FString AbsolutePath = EditorConfig->ScenarioSyncDirectoryPath.Path;
	if (FPaths::IsRelative(AbsolutePath))
	{
		AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir(), AbsolutePath);
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*AbsolutePath))
	{
		return;
	}

	const FString Extension = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT("*.json") : TEXT("*.csv");

	TArray<FString> FoundFiles;
	PlatformFile.FindFilesRecursively(FoundFiles, *AbsolutePath, *Extension);

	SHIDEN_LOG("Checking for external changes in {fileCount} files...", FoundFiles.Num());

	int32 UpdatedCount = 0;
	for (const FString& FilePath : FoundFiles)
	{
		// Read file to get GUID
		FString FileContent;
		if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
		{
			continue;
		}

		// Parse GUID from file
		UShidenScenario* ImportedScenario = nullptr;
		if (EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON)
		{
			if (!UShidenEditorBlueprintLibrary::TryConvertToScenarioFromJson(FileContent, ImportedScenario))
			{
				continue;
			}
		}
		else
		{
			ImportedScenario = UShidenEditorBlueprintLibrary::ConvertToScenarioFromCsv(FileContent);
			if (!ImportedScenario)
			{
				continue;
			}
		}

		if (!ImportedScenario || !ImportedScenario->ScenarioId.IsValid())
		{
			continue;
		}

		// Find existing scenario by GUID
		TObjectPtr<const UShidenScenario> ExistingScenario = FindScenarioByGuid(ImportedScenario->ScenarioId);
		if (!ExistingScenario)
		{
			continue;
		}

		// Get file modification time
		FDateTime FileModTime = PlatformFile.GetTimeStamp(*FilePath);

		// Get asset modification time
		if (const UPackage* Package = ExistingScenario->GetPackage())
		{
			const FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());

			FDateTime AssetModTime = PlatformFile.GetTimeStamp(*PackageFileName);

			// If file is newer than asset, import it
			if (FileModTime > AssetModTime)
			{
				SHIDEN_LOG("File is newer than asset, importing: {path}", *FilePath);
				if (ImportAndUpdateScenario(FilePath))
				{
					UpdatedCount++;
				}
			}
		}
	}

	if (UpdatedCount > 0)
	{
		SHIDEN_LOG("Updated {num} scenarios from external changes", UpdatedCount);

		// Broadcast sync event
		if (const TObjectPtr<const UShidenScenarioSyncManager> Manager = GetInstance())
		{
			Manager->OnScenarioSynced.Broadcast();
		}
	}
}

void UShidenScenarioSyncManager::OnFileAdded(const FString& FilePath)
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync || !EditorConfig->bWatchFileAdditions)
	{
		return;
	}

	const FString Extension = FPaths::GetExtension(FilePath).ToLower();
	const FString ExpectedExtension = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT("json") : TEXT("csv");

	if (Extension == ExpectedExtension)
	{
		SHIDEN_LOG("File added detected: {path}", *FilePath);
		ImportAndUpdateScenario(FilePath);
	}
}

void UShidenScenarioSyncManager::OnFileModified(const FString& FilePath)
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync)
	{
		return;
	}

	const FString Extension = FPaths::GetExtension(FilePath).ToLower();
	const FString ExpectedExtension = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT("json") : TEXT("csv");

	if (Extension == ExpectedExtension)
	{
		SHIDEN_LOG("File modified detected: {path}", *FilePath);
		ImportAndUpdateScenario(FilePath);
	}
}

void UShidenScenarioSyncManager::OnFileRemoved(const FString& FilePath)
{
	const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
	if (!EditorConfig || !EditorConfig->bEnableScenarioSync || !EditorConfig->bWatchFileDeletions)
	{
		return;
	}

	const FString Extension = FPaths::GetExtension(FilePath).ToLower();
	const FString ExpectedExtension = EditorConfig->ScenarioSyncFormat == EShidenScenarioSyncFormat::JSON ? TEXT("json") : TEXT("csv");

	if (Extension != ExpectedExtension)
	{
		return;
	}

	SHIDEN_WARNING("File removed detected: {path}", *FilePath);

	// Get all scenario assets and find the one that matches the deleted file path
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> ScenarioAssets;
	AssetRegistry.GetAssetsByClass(UShidenScenario::StaticClass()->GetClassPathName(), ScenarioAssets);

	// Normalize the deleted file path for comparison
	FString NormalizedDeletedPath = FPaths::ConvertRelativePathToFull(FilePath);
	FPaths::NormalizeFilename(NormalizedDeletedPath);

	for (const FAssetData& AssetData : ScenarioAssets)
	{
		if (TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(AssetData.GetAsset()))
		{
			FString ExpectedFilePath = GetSyncFilePathForScenario(Scenario);
			FPaths::NormalizeFilename(ExpectedFilePath);

			if (ExpectedFilePath.Equals(NormalizedDeletedPath, ESearchCase::IgnoreCase))
			{
				// Found the scenario that corresponds to the deleted file
				SHIDEN_WARNING("Deleting scenario asset: {name} (GUID: {guid})", *Scenario->GetName(), *Scenario->ScenarioId.ToString());

				TArray<UObject*> ObjectsToDelete;
				ObjectsToDelete.Add(Scenario);

				if (ObjectTools::DeleteObjects(ObjectsToDelete, true) > 0)
				{
					SHIDEN_LOG("Successfully deleted scenario asset: {name}", *AssetData.AssetName.ToString());
				}
				else
				{
					SHIDEN_ERROR("Failed to delete scenario asset: {name}", *AssetData.AssetName.ToString());
				}

				return;
			}
		}
	}

	SHIDEN_LOG("No matching scenario found for deleted file: {path}", *FilePath);
}

FString UShidenScenarioSyncManager::GetSyncFilePathForScenario(const UShidenScenario* Scenario)
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

	// Convert from /Game/... to relative path
	// Example: /Game/Shiden/Scenarios/Foo/Bar -> Shiden/Scenarios/Foo/Bar
	FString RelativePath = AssetPath;
	if (RelativePath.StartsWith(TEXT("/Game/")))
	{
		RelativePath = RelativePath.RightChop(6); // Remove "/Game/"
	}
	else if (RelativePath.StartsWith(TEXT("/Script/")))
	{
		RelativePath = RelativePath.RightChop(8); // Remove "/Script/"
	}

	// Remove class suffix if present (e.g., ".ClassName_C")
	int32 DotIndex;
	if (RelativePath.FindLastChar(TEXT('.'), DotIndex))
	{
		RelativePath = RelativePath.Left(DotIndex);
	}

	// Ensure the filename is valid
	FString FileName = FPaths::GetCleanFilename(RelativePath);
	FileName = FPaths::MakeValidFileName(FileName);

	FString DirectoryPath = FPaths::GetPath(RelativePath);

	// Convert to absolute path (using Content directory as base for relative paths)
	FString AbsolutePath = EditorConfig->ScenarioSyncDirectoryPath.Path;
	if (FPaths::IsRelative(AbsolutePath))
	{
		AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir(), AbsolutePath);
	}

	// Combine sync directory + preserved directory structure + filename + extension
	FString FullDirectory = FPaths::Combine(AbsolutePath, DirectoryPath);
	return FPaths::Combine(FullDirectory, FileName + Extension);
}

UShidenScenario* UShidenScenarioSyncManager::FindScenarioByGuid(const FGuid& ScenarioId)
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
