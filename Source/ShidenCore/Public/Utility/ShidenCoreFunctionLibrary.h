// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "System/ShidenBacklogItem.h"
#include "Command/ShidenCommand.h"
#include "Scenario/ShidenScenario.h"
#include "System/ShidenSubsystem.h"
#include "UnrealClient.h"
#include "Widgets/SWindow.h"
#include "AssetRegistry/AssetData.h"
#include "UI/ShidenTextType.h"
#include "ShidenCoreFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static void GetOrLoadAsset(const FString& ObjectPath, UObject*& Asset, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void UnloadAssets(const bool bForceGC);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static FString MakeErrorMessage(const FGuid& ScenarioId, const int32 Index, const FString& CommandName, const FString& ErrorMessage);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog", meta = (AutoCreateRefTerm = "AdditionalProperties"))
	static void AddBacklogItem(const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog", meta = (AutoCreateRefTerm = "AdditionalProperties"))
	static void UpdateBacklogItem(int32 Index, const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static TArray<FShidenBacklogItem>& GetBacklogItems();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static void ClearBacklogItems();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	static TMap<FString, FShidenTextType> GetShidenTextTypes();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility", meta = (WorldContext = "WorldContextObject"))
	static void ClearLoadedSystemData(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void ClearLoadedUserData();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void ClearAllCache();

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void CopyToClipboard(const FString& Str);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetFromClipboard(FString& Dest);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static int32 GetParsedLength(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility", meta = (AutoCreateRefTerm = "Parameters"))
	static void CallFunctionByName(UObject* TargetObject, const FString& FunctionName, const FString& Parameters);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static FString GetCharactersWithParsedLength(const FString& Text, int32 Len);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", Duration = "0.2", Keywords = "sleep"))
	static void MultiThreadDelay(UObject* WorldContextObject, float Duration, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static UClass* ConstructClassFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetScenarioDataAsset(const FString& ObjectPath, UShidenScenario*& Scenario, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Utility")
	static FString GetObjectPathFromClass(const UClass* InClass);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetSoundTypeFromSoundBase(const USoundBase* SoundBase, EShidenSoundType& SoundType, bool& bSuccess);

	UFUNCTION()
	static void InitCommandDefinitions();

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static const TMap<FString, FShidenCommandDefinition>& GetCommandDefinitionsCache();

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static FString GetCommandArgument(const FShidenCommand& Command, const FString& ArgName);
};
