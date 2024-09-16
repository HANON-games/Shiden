// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "AssetRegistry/AssetRegistryModule.h"
#include "DelayAction.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Engine/AssetManager.h"
#include "Engine/Engine.h"
#include "Engine/StreamableManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "HAL/PlatformFilemanager.h"
#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/OutputDeviceNull.h"
#include "Misc/Paths.h"
#include "Serialization/Csv/CsvParser.h"
#include "ShidenBacklogItem.h"
#include "ShidenCommand.h"
#include "ShidenCommandDefinitions.h"
#include "ShidenCsvParsedRow.h"
#include "ShidenScenario.h"
#include "ShidenSubsystem.h"
#include "ShidenSystemSaveGame.h"
#include "ShidenUserSaveGame.h"
#include "UnrealClient.h"
#include "Widgets/SWindow.h"
#include "AssetRegistry/AssetData.h"
#include "ShidenCoreFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void TakeScreenshotWithFileName(const FString InFileName, bool bInShowUI);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static void GetAsset(const FString ObjectPath, UObject*& Asset, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void UnloadAssets(const bool ForceGC);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void TakeScreenshot(const FString InFilePath, bool bInShowUI, bool bAddFilenameSuffix);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static FString MakeErrorMessage(const FGuid ScenarioId, const int32 Index, const FString CommandName, const FString ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void SaveUserData(const FString SlotName, bool bSaveSystemData);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void SaveSystemData();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadUserData(const FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadSystemData();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeleteUserData(const FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeleteSystemData();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesUserDataExist(const FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesSystemDataExist();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static void AddBacklog(const FString SlotName, const FString Text);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static void UpdateBacklog(int32 Index, const FString SlotName, const FString Text);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Backlog")
	static TArray<FShidenBacklogItem> GetBacklog();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static void ClearBacklog();

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void CopyToClipboard(const FString Str);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetFromClipboard(FString& Dest);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static int32 GetParsedLength(const FString Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void CallFunctionByName(UObject* TargetObject, const FString FunctionName, const FString Parameters);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void LoadTextFile(FString& FileName, FString& FileData, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static bool SaveFileAsCsv(const FString DefaultFileName, const FString SaveText);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void ParseCsv(FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static FString GetCharactersWithParsedLength(const FString Text, const int32 Len);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", Duration = "0.2", Keywords = "sleep"))
	static void MultiThreadDelay(UObject* WorldContextObject, float Duration, struct FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static UClass* ConstructBlueprintClassFromSoftObjectPath(const FSoftObjectPath SoftObjectPath);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetScenarioDataAsset(const FString ObjectPath, UShidenScenario*& Scenario, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Utility")
	static FString GetObjectPathFromClass(const UClass* InClass);

	UFUNCTION()
	static void InitCommandDefinitions();

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Utility")
	static TMap<FString, FShidenCommandDefinition> GetCommandDefinitionsCache();

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void ClearAllTempValues();

	UFUNCTION()
	static FString MakeLocalVariableKeyInternal(const UShidenSubsystem* ShidenSubsystem, const FString& ProcessName, bool& bSuccess);

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static FString GetCommandArgument(const FShidenCommand Command, const FString ArgName);
};