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
#include "Save/ShidenSaveSlotsSaveGame.h"
#include "Save/ShidenSaveTexture.h"
#include "UI/ShidenTextType.h"
#include "Tasks/Pipe.h"
#include "ShidenCoreFunctionLibrary.generated.h"

using namespace UE::Tasks;

DECLARE_DELEGATE_OneParam(FAsyncSaveDataDelegate, bool)

UCLASS()
class SHIDENCORE_API UShidenCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static FPipe SaveGamePipe;

	static void WaitUntilEmpty();

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static UTexture2D* ConvertSaveTextureToTexture2D(const FShidenSaveTexture SaveTexture);
	
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static void GetOrLoadAsset(const FString& ObjectPath, UObject*& Asset, bool& bSuccess);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void UnloadAssets(const bool bForceGC);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static FString MakeErrorMessage(const FGuid ScenarioId, const int32 Index, const FString& CommandName, const FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (AutoCreateRefTerm = "SlotMetadata"))
	static void SaveUserData(const FString SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void SaveSystemData(bool& bSuccess);

	static void AsyncSaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, FAsyncSaveDataDelegate SavedDelegate = FAsyncSaveDataDelegate());

	static void AsyncSaveSystemData(FAsyncSaveDataDelegate SavedDelegate = FAsyncSaveDataDelegate());

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static TMap<FString, FShidenSaveSlot> AcquireSaveSlots();

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

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog", meta = (AutoCreateRefTerm = "AdditionalProperties"))
	static void AddBacklogItem(const FShidenCommand Command, const TMap<FString, FString> AdditionalProperties);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog", meta = (AutoCreateRefTerm = "AdditionalProperties"))
	static void UpdateBacklogItem(int32 Index, const FShidenCommand Command, const TMap<FString, FString> AdditionalProperties);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static TArray<FShidenBacklogItem> GetBacklogItems();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static void ClearBacklogItems();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	static TMap<FString, FShidenTextType> GetShidenTextTypes();
	
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

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static TMap<FString, FShidenCommandDefinition> GetCommandDefinitionsCache();

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void ClearAllTempValues();

	UFUNCTION()
	static FString MakeLocalVariableKeyInternal(const UShidenSubsystem* ShidenSubsystem, const FString& ProcessName, bool& bSuccess);

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static FString GetCommandArgument(const FShidenCommand& Command, const FString& ArgName);
};