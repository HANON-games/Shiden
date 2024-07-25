// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenCoreFunctionLibrary.h"
#include "Internationalization/Regex.h"
#include "Misc/FileHelper.h"
#include <Misc/OutputDeviceNull.h>
#include "Misc/Paths.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Serialization/Csv/CsvParser.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "DelayAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Widgets/SWindow.h"
#include "ShidenCommand.h"

SHIDENCORE_API void UShidenCoreFunctionLibrary::CopyToClipboard(const FString& Str)
{
	FPlatformApplicationMisc::ClipboardCopy(*Str);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetFromClipboard(FString& Dest)
{
	FPlatformApplicationMisc::ClipboardPaste(Dest);
}

SHIDENCORE_API int32 UShidenCoreFunctionLibrary::GetParsedLength(const FString& text)
{
	FString resultText = text;

	resultText.ReplaceInline(TEXT("&quot;"), TEXT("\""), ESearchCase::CaseSensitive);
	resultText.ReplaceInline(TEXT("&amp;"), TEXT("&"), ESearchCase::CaseSensitive);
	resultText.ReplaceInline(TEXT("&apos;"), TEXT("'"), ESearchCase::CaseSensitive);
	resultText.ReplaceInline(TEXT("&lt;"), TEXT("<"), ESearchCase::CaseSensitive);

	// pattern like <img id="value"/>
	const FRegexPattern pattern = FRegexPattern(FString(TEXT("(<[\\w\\d\\.-]+((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)))")));
	FRegexMatcher matcher(pattern, text);

	while (matcher.FindNext())
	{
		FString s1 = matcher.GetCaptureGroup(1);
		resultText.ReplaceInline(*s1, TEXT("o"), ESearchCase::CaseSensitive);
	}

	// pattern like <tagname> ... </>
	const FRegexPattern pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher matcher2(pattern2, text);

	while (matcher2.FindNext())
	{
		FString s1 = matcher2.GetCaptureGroup(0);
		FString s2 = matcher2.GetCaptureGroup(1);
		resultText.ReplaceInline(*s1 , *s2, ESearchCase::CaseSensitive);
	}

	return resultText.Len();
}

struct TextPosition
{
	int32 OpenTagStart;
	int32 CloseTagEnd;
	int32 ContentStart;
	int32 ContentEnd;
};

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCharactersWithParsedLength(const FString& text, const int32& len)
{
	FString resultText = text;
	int32 length = len;

	resultText.ReplaceInline(TEXT("&quot;"), TEXT("\""), ESearchCase::CaseSensitive);
	resultText.ReplaceInline(TEXT("&amp;"), TEXT("&"), ESearchCase::CaseSensitive);
	resultText.ReplaceInline(TEXT("&apos;"), TEXT("'"), ESearchCase::CaseSensitive);
	resultText.ReplaceInline(TEXT("&lt;"), TEXT("<"), ESearchCase::CaseSensitive);

	TArray<TextPosition> pos;

	// pattern like <img id="value"/>
	const FRegexPattern pattern = FRegexPattern(FString(TEXT("(<[\\w\\d\\.-]+((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)))")));
	FRegexMatcher matcher(pattern, text);

	while (matcher.FindNext())
	{
		int32 s1 = matcher.GetMatchBeginning();
		int32 s2 = matcher.GetMatchEnding();
		pos.Add({ s1, s2, -1, -1});
	}

	// pattern like <tagname> ... </>
	const FRegexPattern pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher matcher2(pattern2, text);

	while (matcher2.FindNext())
	{
		int32 s1 = matcher2.GetMatchBeginning();
		int32 s2 = matcher2.GetMatchEnding();

		pos.Add({ s1, s2, matcher2.GetCaptureGroupBeginning(1), matcher2.GetCaptureGroupEnding(1)});
	}

	if (pos.Num() == 0)
	{
		return text.Left(len);
	}

	bool bIsInTag = false;
	int32 resultLen = 0;

	for (int i = 0; i < len; i++)
	{
		for (auto& p : pos)
		{
			if (p.OpenTagStart == resultLen)
			{
				if (p.ContentStart == -1)
				{
					// TODO: Image タグは 1 文字扱いなので -1 して調整
					// タグごとに処理を分けられるようにする
					resultLen = p.CloseTagEnd - 1;
					break;
				}
				else 
				{
					bIsInTag = true;
					resultLen = p.ContentStart;
					break;
				}
			}
			else if (p.ContentEnd == resultLen)
			{
				bIsInTag = false;
				resultLen = p.CloseTagEnd;
				break;
			}
		}
		resultLen++;
	}

	resultText = resultText.Left(resultLen);

	if (bIsInTag)
	{
		resultText += TEXT("</>");
	}

	return resultText;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::CallFunctionByName(UObject* targetObject, const FString functionName, const FString parameters)
{
	if (targetObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("targetObject is nullptr"));
		return;
	}

	FString trimmedParam = parameters.TrimStartAndEnd();
	FString cmd = trimmedParam.IsEmpty() ? functionName : FString::Printf(TEXT("%s %s"), *functionName, *parameters);
	FOutputDeviceNull _Null;

	targetObject->CallFunctionByNameWithArguments(*cmd, _Null, nullptr, true);
}

SHIDENCORE_API bool UShidenCoreFunctionLibrary::SaveFileAsCsv(const FString& DefaultFileName, const FString& SaveText)
{
	// get window handle
	void* windowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = "MainFrame";
		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			windowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	if (windowHandle)
	{
		IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();
		if (desktopPlatform)
		{
			TArray<FString> FilePath = { };

			bool bResult = desktopPlatform->SaveFileDialog(
				windowHandle,
				TEXT("Save File Dialog"),
				TEXT(""),
				DefaultFileName,
				TEXT("Scenario CSV (*.csv)|*.csv"),
				EFileDialogFlags::Type::None,
				FilePath
			);

			if (bResult)
			{
				return FFileHelper::SaveStringToFile(SaveText, *FPaths::ConvertRelativePathToFull(FilePath[0]), FFileHelper::EEncodingOptions::ForceUTF8);			
			}
		}
	}
	return false;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ParseCsv(FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow)
{
	FCsvParser Parser = FCsvParser(CsvText);
	auto& rows = Parser.GetRows();
	bool bCommentEnd = false;

	for (auto& row : rows)
	{
		// Skip comment lines like "# comment"
		if (!bCommentEnd && FString(row[0]).TrimStart().TrimEnd().StartsWith(TEXT("#")))
		{
			continue;
		}

		bCommentEnd = true;

		FShidenCsvParsedRow ParsedRow;
		for (const TCHAR* cell : row)
		{
			ParsedRow.Row.Add(FString(cell));
		}
		CsvParsedRow.Add(ParsedRow);
	}

	return;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::LoadTextFile(FString& FileName, FString& FileData, bool& bSuccess)
{
	// Get window handle
	void* windowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = "MainFrame";
		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			windowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	TArray<FString> FilePath = { };

	if (windowHandle)
	{
		IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();
		if (desktopPlatform)
		{
			//ダイアログを開く
			bool bResult = desktopPlatform->OpenFileDialog(
				windowHandle,
				TEXT("Open File Dialog"),
				TEXT(""),
				TEXT(""),
				TEXT("Scenario CSV (*.csv)|*.csv"),
				EFileDialogFlags::Type::None,
				FilePath
			);

			if (bResult)
			{
				if (GEngine)
				{
					FString LoadFilePath = FPaths::ConvertRelativePathToFull(FilePath[0]);
					
					if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*LoadFilePath))
					{
						bSuccess = false;
						return;
					}
					FileName = FPaths::GetCleanFilename(LoadFilePath);
					const int64 FileSize = FPlatformFileManager::Get().GetPlatformFile().FileSize(*LoadFilePath);
					FFileHelper::LoadFileToString(FileData, *LoadFilePath);
					bSuccess = true;

				}
			}
		}
	}
	bSuccess = false;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::MultiThreadDelay(UObject* WorldContextObject, float Duration, struct FLatentActionInfo LatentInfo)
{
	if (TObjectPtr <UWorld> World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelayAction(Duration, LatentInfo));
	}
}

SHIDENCORE_API UClass* UShidenCoreFunctionLibrary::ConstructBlueprintClassFromSoftObjectPath(const FSoftObjectPath softObjectPath)
{
	FString path = softObjectPath.GetAssetPathString() + "_C";
	return StaticLoadClass(UObject::StaticClass(), NULL, *path, NULL, LOAD_None, NULL);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::TakeScreenshotWithFileName(const FString InFileName, bool bInShowUI)
{
	FString ScreenShotDir = FPaths::ScreenShotDir();
	FString FilePath = ScreenShotDir + FString(TEXT("/")) + InFileName + FString(TEXT(".png"));
	FScreenshotRequest request = FScreenshotRequest();
	request.RequestScreenshot(InFileName, bInShowUI, false);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::TakeScreenshot(const FString InFilePath, bool bInShowUI, bool bAddFilenameSuffix)
{
	FScreenshotRequest request = FScreenshotRequest();
	request.RequestScreenshot(InFilePath, bInShowUI, bAddFilenameSuffix);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetScenarioDataAsset(const FString ObjectPath, UShidenScenario*& Scenario, bool& bSucceeded)
{
	Scenario = nullptr;
	bSucceeded = false;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(ObjectPath));

	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AssetData is invalid for ObjectPath: %s"), *ObjectPath);
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	TObjectPtr <UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load asset from ObjectPath: %s"), *ObjectPath);
		return;
	}

	TObjectPtr <UShidenScenario> ShidenScenario = Cast<UShidenScenario>(LoadedAsset);
	if (!ShidenScenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded asset to UShidenScenario"));
		return;
	}

	Scenario = ShidenScenario;
	bSucceeded = true;
	return;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetAsset(const FString ObjectPath, UObject*& Asset, bool& bSuccess)
{
	Asset = nullptr;
	bSuccess = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	TObjectPtr <UObject>* AssetPtr = ShidenSubsystem->AssetCache.Find(ObjectPath);
	if (AssetPtr)
	{
		Asset = *AssetPtr;
		bSuccess = true;
		return;
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UnloadAssets(const bool ForceGC)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->AssetCache.Empty();

	if (ForceGC)
	{
		GEngine->ForceGarbageCollection(true);
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::SaveUserData(const FString SlotName, bool bSaveSystemData)
{
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		return;
	}

	TObjectPtr <UShidenUserSaveGame> SaveGameInstance = Cast<UShidenUserSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenUserSaveGame::StaticClass()));

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	SaveGameInstance->WidgetClass = ShidenSubsystem->CurrentWidgetClass;
	SaveGameInstance->ScenarioProperties = ShidenSubsystem->CurrentScenarioProperties;
	SaveGameInstance->UserVariables = ShidenSubsystem->UserVariable;
	SaveGameInstance->LocalVariables = ShidenSubsystem->LocalVariables;
	SaveGameInstance->ScenarioProgressStack = ShidenSubsystem->ScenarioProgressStack;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (bSaveSystemData)
	{
		UShidenCoreFunctionLibrary::SaveSystemData();
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::SaveSystemData()
{
	TObjectPtr <UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSystemSaveGame::StaticClass()));

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	SaveGameInstance->SystemVariables = ShidenSubsystem->SystemVariable;
	SaveGameInstance->PredefinedSystemVariables = ShidenSubsystem->PredefinedSystemVariable;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::LoadUserData(const FString SlotName)
{
	if (!UShidenCoreFunctionLibrary::DoesUserDataExist(SlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("User data does not exist for slot name: %s"), *SlotName);
		return;
	}

	TObjectPtr <UShidenUserSaveGame> SaveGameInstance = Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));


	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->CurrentScenarioProperties = SaveGameInstance->ScenarioProperties;
	ShidenSubsystem->UserVariable = SaveGameInstance->UserVariables;
	ShidenSubsystem->ScenarioProgressStack = SaveGameInstance->ScenarioProgressStack;
	ShidenSubsystem->LocalVariables = SaveGameInstance->LocalVariables;
	ShidenSubsystem->CurrentWidgetClass = SaveGameInstance->WidgetClass;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::LoadSystemData()
{
	if (!UShidenCoreFunctionLibrary::DoesUserDataExist(TEXT("ShidenSystemData")))
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return;
	}

	TObjectPtr <UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSystemData"), 0));

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->SystemVariable = SaveGameInstance->SystemVariables;
	ShidenSubsystem->PredefinedSystemVariable = SaveGameInstance->PredefinedSystemVariables;
}

SHIDENCORE_API bool UShidenCoreFunctionLibrary::DoesUserDataExist(const FString SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

SHIDENCORE_API bool UShidenCoreFunctionLibrary::DoesSystemDataExist()
{
	return UGameplayStatics::DoesSaveGameExist(TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::AddBacklog(const FString SlotName, const FString Text)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->BacklogItems.Add(FShidenBacklogItem{ SlotName, Text });
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UpdateBacklog(int32 Index, const FString SlotName, const FString Text)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	if (!ShidenSubsystem->BacklogItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid backlog index: %d"), Index);
		return;
	}

	FShidenBacklogItem& BacklogItem = ShidenSubsystem->BacklogItems[Index];
	BacklogItem.SlotName = SlotName;
	BacklogItem.Text = Text;
}

SHIDENCORE_API TArray<FShidenBacklogItem> UShidenCoreFunctionLibrary::GetBacklog()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return TArray<FShidenBacklogItem>();
	}

	return ShidenSubsystem->BacklogItems;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearBacklog()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->BacklogItems.Empty();
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetObjectPathFromClass(const UClass* InClass)
{
	FTopLevelAssetPath Path = FTopLevelAssetPath(InClass);
	return Path.ToString().LeftChop(2);
}

SHIDENCORE_API void MakeObjectPathFromPackagePath(const FString PackagePath, FName& ObjectPath)
{
	FString ObjectPathString;
	FString Right;
	PackagePath.Split(TEXT("."), nullptr, &Right);
	ObjectPathString = PackagePath + TEXT(".") + Right;
	ObjectPath = FName(*ObjectPathString);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::InitCommandDefinitions()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	if (ShidenSubsystem->CommandDefinitionCache.Num() > 0)
	{
		return;
	}

	const UShidenProjectConfig* ProjectConfig = GetDefault<UShidenProjectConfig>();

	for (const FSoftObjectPath& CommandDefinitionSoftObjectPath : ProjectConfig->CommandDefinitions)
	{
		FString ObjectPath = CommandDefinitionSoftObjectPath.GetAssetPathString();
		TObjectPtr <UShidenCommandDefinitions> CommandDefinitions = Cast<UShidenCommandDefinitions>(StaticLoadObject(UShidenCommandDefinitions::StaticClass(), nullptr, *ObjectPath));
		if (!CommandDefinitions)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load CommandDefinitions from ObjectPath: %s"), *ObjectPath);
			continue;
		}

		for (const auto& [CommandName, CommandDefinition] : CommandDefinitions->CommandDefinitions)
		{
			if (ShidenSubsystem->CommandDefinitionCache.Contains(CommandName))
			{
				UE_LOG(LogTemp, Warning, TEXT("CommandDefinition already exists for CommandName: %s"), *CommandName);
				continue;
			}

			ShidenSubsystem->CommandDefinitionCache.Add(CommandName, CommandDefinition);
		}
	}
}

/*
SHIDENCORE_API void GetCommand(UObject* Outer, const FSoftObjectPath CommandSoftObjectPath, UShidenCommandObject*& CommandObject, bool& Success)
{
	UClass* CommandClass = UShidenCommandObject::StaticClass();
	CommandObject = NewObject<UShidenCommandObject>(Outer, CommandClass);
	if (!CommandObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UShidenCommandObject from ObjectPath: %s"), *CommandSoftObjectPath.GetAssetPathString());
		Success = false;
		return;
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetCommandFromCache(const UObject* Outer, const FString ProcessName, const FSoftObjectPath CommandSoftObjectPath, UObject*& Command, bool& bSuccess)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		bSuccess = false;
		return;
	}

	FString CommandCacheKey = ProcessName + TEXT("::") + CommandSoftObjectPath.GetAssetPathString();
	TObjectPtr <UShidenCommandObject>* CommandPtr = ShidenSubsystem->CommandCache.Find(CommandCacheKey);
	if (CommandPtr)
	{
		Command = *CommandPtr;
		bSuccess = true;
		return;
	}

	UClass* CommandClass = UShidenCommand::StaticClass();
	Command = NewObject<UShidenCommand>(Outer, CommandClass);
	if (!Command)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UShidenCommand from ObjectPath: %s"), *ObjectPath);
		bSuccess = false;
		return;
	}

	Command->SetFlags(RF_Standalone);
	Command->AddToRoot();
	Command->SetPathName(*ObjectPath);
	Command->SetOuter(Outer);

	ShidenSubsystem->CommandCache.Add(ObjectPath, Command);
	bSuccess = true;
	return;
}
*/

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearAllTempValues()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->CurrentScenarioProperties.Empty();
	ShidenSubsystem->AssetCache.Empty();
	ShidenSubsystem->CommandCache.Empty();
	ShidenSubsystem->CommandDefinitionCache.Empty();
	ShidenSubsystem->ScenarioCache.Empty();

	for (const auto& [ProcessName, _] : ShidenSubsystem->ScenarioProgressStack)
	{
		if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
		{
			bool bSuccess = false;
			const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
			if (!bSuccess)
			{
				continue;
			}
			ShidenSubsystem->LocalVariables.Remove(LocalKey);
		}
	}

	ShidenSubsystem->ScenarioProgressStack.Empty();
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(const UShidenSubsystem* ShidenSubsystem, const FString ProcessName, bool& bSuccess)
{
	const FShidenScenarioProgressStack* Stack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (Stack)
	{
		const int32 lastIndex = Stack->Stack.Num() - 1;
		bSuccess = true;
		return FString::Printf(TEXT("%s$%d"), *ProcessName, lastIndex);
	}
#if WITH_EDITOR
	TObjectPtr<UGameViewportClient> GameViewport = GEngine->GameViewport;
	if (GameViewport)
	{
		UWorld* World = GameViewport->GetWorld();
		if (World)
		{
			if (World->WorldType == EWorldType::PIE)
			{
				UE_LOG(LogTemp, Warning, TEXT("Local variable not found: Process Name \"%s\""), *ProcessName);
			}
		}
	}
#else
	UE_LOG(LogTemp, Warning, TEXT("Local variable not found: Process Name \"%s\""), *ProcessName);
#endif
	bSuccess = false;
	return FString();
}
