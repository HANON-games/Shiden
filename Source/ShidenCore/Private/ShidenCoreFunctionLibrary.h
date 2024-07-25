// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Serialization/Csv/CsvParser.h"
#include "ShidenCsvParsedRow.h"
#include "Engine/Blueprint.h"
#include "UnrealClient.h"
#include "Misc/Paths.h"
#include "ShidenScenario.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Engine.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "ShidenSubsystem.h"
#include "ShidenUserSaveGame.h"
#include "ShidenSystemSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ShidenBacklogItem.h"
#include "ShidenCoreFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void CopyToClipboard(const FString& Str);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void GetFromClipboard(FString& Dest);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static int32 GetParsedLength(const FString& text);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void CallFunctionByName(UObject* targetObject, const FString functionName, const FString parameters);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void LoadTextFile(FString& FileName, FString& FileData, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static bool SaveFileAsCsv(const FString& DefaultFileName, const FString& SaveText);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void ParseCsv(FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static FString GetCharactersWithParsedLength(const FString& text, const int32& len);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", Duration = "0.2", Keywords = "sleep"))
	static void MultiThreadDelay(UObject* WorldContextObject, float Duration, struct FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static UClass* ConstructBlueprintClassFromSoftObjectPath(const FSoftObjectPath softObjectPath);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void TakeScreenshotWithFileName(const FString InFileName, bool bInShowUI);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void TakeScreenshot(const FString InFilePath, bool bInShowUI, bool bAddFilenameSuffix);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void GetScenarioDataAsset(const FString ObjectPath, UShidenScenario*& Scenario, bool& bSucceeded);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static void GetAsset(const FString ObjectPath, UObject*& Asset, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void UnloadAssets(const bool ForceGC);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void SaveUserData(const FString SlotName, bool bSaveSystemData);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void SaveSystemData();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadUserData(const FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadSystemData();

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
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Internal|Utility")
	static FString GetObjectPathFromClass(const UClass* InClass);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Utility")
	static void InitCommandDefinitions();

	/*
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Utility")
	static void GetCommandFromCache(const UObject* Outer, const FString ProcessName,const FSoftObjectPath CommandSoftObjectPath, UObject*& Command, bool& bSuccess);
	*/

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Utility")
	static void ClearAllTempValues();

	UFUNCTION()
	static FString MakeLocalVariableKeyInternal(const UShidenSubsystem* ShidenSubsystem, const FString ProcessName, bool& bSuccess);
};