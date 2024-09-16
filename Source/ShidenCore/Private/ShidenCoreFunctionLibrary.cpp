// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenCoreFunctionLibrary.h"

SHIDENCORE_API void UShidenCoreFunctionLibrary::CopyToClipboard(const FString Str)
{
	FPlatformApplicationMisc::ClipboardCopy(*Str);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetFromClipboard(FString& Dest)
{
	FPlatformApplicationMisc::ClipboardPaste(Dest);
}

SHIDENCORE_API int32 UShidenCoreFunctionLibrary::GetParsedLength(const FString Text)
{
	FString ResultText = Text;

	ResultText.ReplaceInline(TEXT("&quot;"), TEXT("\""), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&amp;"), TEXT("&"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&apos;"), TEXT("'"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&lt;"), TEXT("<"), ESearchCase::CaseSensitive);

	// pattern like <img id="value"/>
	const FRegexPattern Pattern = FRegexPattern(FString(TEXT("(<[\\w\\d\\.-]+((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)))")));
	FRegexMatcher Matcher(Pattern, Text);

	while (Matcher.FindNext())
	{
		FString Str = Matcher.GetCaptureGroup(1);
		ResultText.ReplaceInline(*Str, TEXT("o"), ESearchCase::CaseSensitive);
	}

	// pattern like <tagname> ... </>
	const FRegexPattern Pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher Matcher2(Pattern2, Text);

	while (Matcher2.FindNext())
	{
		FString Str1 = Matcher2.GetCaptureGroup(0);
		FString Str2 = Matcher2.GetCaptureGroup(1);
		ResultText.ReplaceInline(*Str1 , *Str2, ESearchCase::CaseSensitive);
	}

	return ResultText.Len();
}

struct FTextPosition
{
	int32 OpenTagStart;
	int32 CloseTagEnd;
	int32 ContentStart;
	int32 ContentEnd;
};

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCharactersWithParsedLength(const FString Text, const int32 Len)
{
	FString ResultText = Text;
	int32 Length = Len;

	ResultText.ReplaceInline(TEXT("&quot;"), TEXT("\""), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&amp;"), TEXT("&"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&apos;"), TEXT("'"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&lt;"), TEXT("<"), ESearchCase::CaseSensitive);

	TArray<FTextPosition> Pos;

	// pattern like <img id="value"/>
	const FRegexPattern Pattern = FRegexPattern(FString(TEXT("(<[\\w\\d\\.-]+((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)))")));
	FRegexMatcher Matcher(Pattern, Text);

	while (Matcher.FindNext())
	{
		int32 Str1 = Matcher.GetMatchBeginning();
		int32 Str2 = Matcher.GetMatchEnding();
		Pos.Add({ Str1, Str2, -1, -1});
	}

	// pattern like <tagname> ... </>
	const FRegexPattern Pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher Matcher2(Pattern2, Text);

	while (Matcher2.FindNext())
	{
		int32 Str1 = Matcher2.GetMatchBeginning();
		int32 Str2 = Matcher2.GetMatchEnding();

		Pos.Add({ Str1, Str2, Matcher2.GetCaptureGroupBeginning(1), Matcher2.GetCaptureGroupEnding(1)});
	}

	if (Pos.Num() == 0)
	{
		return Text.Left(Len);
	}

	bool bIsInTag = false;
	int32 ResultLen = 0;

	for (int32 Index = 0; Index < Len; Index++)
	{
		for (auto& P : Pos)
		{
			if (P.OpenTagStart == ResultLen)
			{
				if (P.ContentStart == -1)
				{
					// TODO: Image tag is treated as 1 character, so adjust by -1.
					// Make it possible to process each tag separately.
					ResultLen = P.CloseTagEnd - 1;
					break;
				}
				else 
				{
					bIsInTag = true;
					ResultLen = P.ContentStart;
					break;
				}
			}
			else if (P.ContentEnd == ResultLen)
			{
				bIsInTag = false;
				ResultLen = P.CloseTagEnd;
				break;
			}
		}
		ResultLen++;
	}

	ResultText = ResultText.Left(ResultLen);

	if (bIsInTag)
	{
		ResultText += TEXT("</>");
	}

	return ResultText;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::CallFunctionByName(UObject* TargetObject, const FString FunctionName, const FString Parameters)
{
	if (TargetObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("targetObject is nullptr"));
		return;
	}

	FString TrimmedParam = Parameters.TrimStartAndEnd();
	FString Cmd = TrimmedParam.IsEmpty() ? FunctionName : FString::Printf(TEXT("%s %s"), *FunctionName, *Parameters);
	FOutputDeviceNull _Null;

	TargetObject->CallFunctionByNameWithArguments(*Cmd, _Null, nullptr, true);
}

SHIDENCORE_API bool UShidenCoreFunctionLibrary::SaveFileAsCsv(const FString DefaultFileName, const FString SaveText)
{
	// get window handle
	void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = "MainFrame";
		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	if (WindowHandle)
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			TArray<FString> FilePath = { };

			bool bResult = DesktopPlatform->SaveFileDialog(
				WindowHandle,
				TEXT("Save File Dialog"),
				TEXT(""),
				DefaultFileName + TEXT(".csv"),
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
	auto& Rows = Parser.GetRows();
	bool bCommentEnd = false;

	for (auto& Row : Rows)
	{
		// Skip comment lines like "# comment"
		if (!bCommentEnd && FString(Row[0]).TrimStart().TrimEnd().StartsWith(TEXT("#")))
		{
			continue;
		}

		bCommentEnd = true;

		FShidenCsvParsedRow ParsedRow;
		for (const TCHAR* Cell : Row)
		{
			ParsedRow.Row.Add(FString(Cell));
		}
		CsvParsedRow.Add(ParsedRow);
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::LoadTextFile(FString& FileName, FString& FileData, bool& bSuccess)
{
	// Get window handle
	void* WindowHandle = nullptr;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		const FName MainFrameModuleName = "MainFrame";
		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(MainFrameModuleName);
		TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			WindowHandle = MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	TArray<FString> FilePath = { };

	if (WindowHandle)
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			bool bResult = DesktopPlatform->OpenFileDialog(
				WindowHandle,
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
	if (TObjectPtr<UWorld> World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelayAction(Duration, LatentInfo));
	}
}

SHIDENCORE_API UClass* UShidenCoreFunctionLibrary::ConstructBlueprintClassFromSoftObjectPath(const FSoftObjectPath SoftObjectPath)
{
	if (SoftObjectPath.IsNull())
	{
		return nullptr;
	}
	FString Path = SoftObjectPath.GetAssetPathString() + "_C";
	return StaticLoadClass(UObject::StaticClass(), NULL, *Path, NULL, LOAD_None, NULL);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::TakeScreenshotWithFileName(const FString InFileName, bool bInShowUI)
{
	FString ScreenShotDir = FPaths::ScreenShotDir();
	FString FilePath = ScreenShotDir + FString(TEXT("/")) + InFileName + FString(TEXT(".png"));
	FScreenshotRequest Request = FScreenshotRequest();
	Request.RequestScreenshot(InFileName, bInShowUI, false);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::TakeScreenshot(const FString InFilePath, bool bInShowUI, bool bAddFilenameSuffix)
{
	FScreenshotRequest Request = FScreenshotRequest();
	Request.RequestScreenshot(InFilePath, bInShowUI, bAddFilenameSuffix);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetScenarioDataAsset(const FString ObjectPath, UShidenScenario*& Scenario, bool& bSuccess)
{
	Scenario = nullptr;
	bSuccess = false;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(ObjectPath));

	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AssetData is invalid for ObjectPath: %s"), *ObjectPath);
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	TObjectPtr<UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load asset from ObjectPath: %s"), *ObjectPath);
		return;
	}

	TObjectPtr<UShidenScenario> ShidenScenario = Cast<UShidenScenario>(LoadedAsset);
	if (!ShidenScenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded asset to UShidenScenario"));
		return;
	}

	Scenario = ShidenScenario;
	bSuccess = true;
	return;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetAsset(const FString ObjectPath, UObject*& Asset, bool& bSuccess)
{
	Asset = nullptr;
	bSuccess = false;

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	TObjectPtr<UObject>* AssetPtr = ShidenSubsystem->AssetCache.Find(ObjectPath);
	if (AssetPtr)
	{
		Asset = *AssetPtr;
		bSuccess = true;
		return;
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UnloadAssets(const bool ForceGC)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();

	if (ForceGC)
	{
		GEngine->ForceGarbageCollection(true);
	}
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::MakeErrorMessage(const FGuid ScenarioId, const int32 Index, const FString CommandName, const FString ErrorMessage)
{
	TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	if (!ProjectConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShidenProjectConfig is null"));
		return TEXT("");
	}

	const FString ScenarioPath = ProjectConfig->ScenarioPaths.FindRef(ScenarioId);

	FString LeftS, RightS;
	ScenarioPath.Split(TEXT("/"), nullptr, &RightS, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	RightS.Split(TEXT("."), &LeftS, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString LineNumber = Index == -1 ? TEXT("") : TEXT(" L") + FString::FromInt(Index + 1);

	FString Command = CommandName.IsEmpty() ? TEXT("") : CommandName + TEXT(": ");

	return FString::Printf(TEXT("[%s%s] %s%s"), *LeftS, *LineNumber, *Command, *ErrorMessage);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::SaveUserData(const FString SlotName, bool bSaveSystemData)
{
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		return;
	}

	TObjectPtr<UShidenUserSaveGame> SaveGameInstance = Cast<UShidenUserSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenUserSaveGame::StaticClass()));

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

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
	TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSystemSaveGame::StaticClass()));

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

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

	TObjectPtr<UShidenUserSaveGame> SaveGameInstance = Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

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

	TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSystemData"), 0));

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable = SaveGameInstance->SystemVariables;
	ShidenSubsystem->PredefinedSystemVariable = SaveGameInstance->PredefinedSystemVariables;
	// Update platform name
	ShidenSubsystem->PredefinedSystemVariable.PlatformName = UGameplayStatics::GetPlatformName();
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::DeleteUserData(const FString SlotName)
{
	if (UShidenCoreFunctionLibrary::DoesUserDataExist(SlotName))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::DeleteSystemData()
{
	if (UShidenCoreFunctionLibrary::DoesSystemDataExist())
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("ShidenSystemData"), 0);
	}
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
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Add(FShidenBacklogItem{ SlotName, Text });
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UpdateBacklog(int32 Index, const FString SlotName, const FString Text)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

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
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	return ShidenSubsystem->BacklogItems;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearBacklog()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Empty();
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetObjectPathFromClass(const UClass* InClass)
{
	FTopLevelAssetPath Path = FTopLevelAssetPath(InClass);
	return Path.ToString().LeftChop(2);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::InitCommandDefinitions()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() > 0)
	{
		return;
	}

	TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	for (const FSoftObjectPath& CommandDefinitionSoftObjectPath : ProjectConfig->CommandDefinitions)
	{
		FString ObjectPath = CommandDefinitionSoftObjectPath.GetAssetPathString();
		TObjectPtr<UShidenCommandDefinitions> CommandDefinitions = Cast<UShidenCommandDefinitions>(StaticLoadObject(UShidenCommandDefinitions::StaticClass(), nullptr, *ObjectPath));
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

SHIDENCORE_API TMap<FString, FShidenCommandDefinition> UShidenCoreFunctionLibrary::GetCommandDefinitionsCache()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		InitCommandDefinitions();
	}

	return ShidenSubsystem->CommandDefinitionCache;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearAllTempValues()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

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

SHIDENCORE_API FString UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(const UShidenSubsystem* ShidenSubsystem, const FString& ProcessName, bool& bSuccess)
{
	const FShidenScenarioProgressStack* Stack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (Stack)
	{
		const int32 LastIndex = Stack->Stack.Num() - 1;
		bSuccess = true;
		return FString::Printf(TEXT("%s$%d"), *ProcessName, LastIndex);
	}
#if WITH_EDITOR
	TObjectPtr<UGameViewportClient> GameViewport = GEngine->GameViewport;
	if (GameViewport)
	{
		TObjectPtr<UWorld> World = GameViewport->GetWorld();
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

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCommandArgument(const FShidenCommand Command, const FString ArgName)
{
	return Command.Args.Contains(ArgName) ? Command.Args[ArgName] : TEXT("");
}
