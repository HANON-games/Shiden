// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "System/ShidenBacklogItem.h"
#include "Command/ShidenCommand.h"
#include "System/ShidenSubsystem.h"
#include "UnrealClient.h"
#include "Widgets/SWindow.h"
#include "AssetRegistry/AssetData.h"
#include "UI/ShidenTextType.h"
#include "ShidenBlueprintLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Checks if auto text mode is enabled.
	 * 
	 * @return True if auto text mode is enabled
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|System")
	static bool IsAutoTextMode();

	/**
	 * Sets the auto text mode state.
	 * 
	 * @param bMode True to enable auto text mode, false to disable
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System")
	static void SetAutoTextMode(const bool bMode);

	/**
	 * Gets an asset by path, loading it if necessary.
	 * 
	 * @param ObjectPath The path to the asset
	 * @param Asset [out] The loaded asset object
	 * @param bSuccess [out] True if the asset was successfully retrieved or loaded
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static void GetOrLoadAsset(const FString& ObjectPath, UObject*& Asset, bool& bSuccess);

	/**
	 * Unloads unused assets from memory.
	 * 
	 * @param bForceGC True to force garbage collection after unloading
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void UnloadAssets(const bool bForceGC);

	/**
	 * Creates a formatted error message for command errors.
	 * 
	 * @param ScenarioId The ID of the scenario where the error occurred
	 * @param ScenarioIndex The index in the scenario where the error occurred
	 * @param CommandName The name of the command that caused the error
	 * @param ErrorMessage The original error message
	 * @return A formatted error message string
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility")
	static FString MakeErrorMessage(const FGuid& ScenarioId, const int32 ScenarioIndex, const FString& CommandName, const FString& ErrorMessage);

	/**
	 * Adds a new item to the backlog.
	 * 
	 * @param Command The command to add to the backlog
	 * @param AdditionalProperties Additional properties to store with the backlog item
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog", meta = (AutoCreateRefTerm = "AdditionalProperties"))
	static void AddBacklogItem(const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties);

	/**
	 * Updates an existing backlog item.
	 * 
	 * @param ScenarioIndex The index of the backlog item to update
	 * @param Command The new command to update the backlog item with
	 * @param AdditionalProperties Additional properties to store with the backlog item
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog", meta = (AutoCreateRefTerm = "AdditionalProperties"))
	static void UpdateBacklogItem(const int32 ScenarioIndex, const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties);

	/**
	 * Gets all backlog items.
	 * 
	 * @return Reference to the array of backlog items
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static TArray<FShidenBacklogItem>& GetBacklogItems();

	/**
	 * Clears all backlog items.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Backlog")
	static void ClearBacklogItems();

	/**
	 * Gets all available text types for the Shiden text widget.
	 * 
	 * @return Map of text type names
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	static TMap<FString, FShidenTextType> GetShidenTextTypes();

	/**
	 * Clears all cached data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void ClearAllCache();

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void CopyToClipboard(const FString& Str);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetClipboardContent(FString& Dest);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static int32 GetParsedLength(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static FString GetCharactersWithParsedLength(const FString& Text, int32 Len);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static bool TryParseWaitTimeFromLastTag(const FString& RawText, int32 Length, float& OutWaitTime);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility",
		meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", Duration = "0.2", Keywords = "sleep"))
	static void MultiThreadDelay(UObject* WorldContextObject, float Duration, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static UClass* ConstructClassFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

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

private:
	static FRegexPattern& GetSelfClosingTagPattern();

	static FRegexPattern& GetNonSelfClosingTagPattern();

	static FRegexPattern& GetWaitTimePattern();
};
