// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "System/ShidenBacklogItem.h"
#include "Command/ShidenCommand.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ShidenTextType.h"
#include "ShidenBlueprintLibrary.generated.h"

struct FShidenCommandDefinition;
enum class EShidenSoundType : uint8;

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
	 * @param bEnabled True to enable auto text mode, false to disable
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System")
	static void SetAutoTextMode(const bool bEnabled);

	/**
	 * Gets an asset by path, loading it if necessary.
	 * 
	 * @param ObjectPath The path to the asset
	 * @param Asset [out] The loaded asset object
	 * @return True if the asset was successfully retrieved or loaded
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Utility", meta = (DisplayName = "Get or Load Asset"))
	static UPARAM(DisplayName = "Success") bool TryGetOrLoadAsset(const FString& ObjectPath, UObject*& Asset);

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

	/**
	 * Fades the screen out to a specified color.
	 * This function uses a global fade widget and does not require ShidenWidget.
	 *
	 * @param WorldContextObject World context object
	 * @param LatentInfo Latent action info
	 * @param LayerName The name of the fade layer
	 * @param FadeDuration The duration of the fade effect in seconds
	 * @param FadeFunction The easing function type for the fade animation
	 * @param TargetColor The color to fade to
	 * @param Steps The number of animation steps for precision
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param ZOrder The Z-order for the fade widget
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", LayerName = "Default"))
	static void FadeOutScreen(const UObject* WorldContextObject, FLatentActionInfo LatentInfo, const FString& LayerName, float FadeDuration,
	                          EEasingFunc::Type FadeFunction, FLinearColor TargetColor, int32 Steps = 2, float BlendExp = 2.0f, int32 ZOrder = 250);

	/**
	 * Fades the screen in from the current fade color.
	 * This function uses a global fade widget and does not require ShidenWidget.
	 *
	 * @param WorldContextObject World context object
	 * @param LatentInfo Latent action info
	 * @param LayerName The name of the fade layer
	 * @param FadeDuration The duration of the fade effect in seconds
	 * @param FadeFunction The easing function type for the fade animation
	 * @param TargetColor The color to fade from, used when creating new widget
	 * @param Steps The number of animation steps for precision
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param ZOrder The Z-order for the fade widget
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", LayerName = "Default"))
	static void FadeInScreen(const UObject* WorldContextObject, FLatentActionInfo LatentInfo, const FString& LayerName, float FadeDuration,
	                         EEasingFunc::Type FadeFunction, FLinearColor TargetColor, int32 Steps = 2, float BlendExp = 2.0f, int32 ZOrder = 250);

	/**
	 * Starts a screen fade animation (unified function for both fade in and fade out).
	 * This function uses a global fade widget and does not require ShidenWidget.
	 *
	 * @param WorldContextObject World context object
	 * @param LayerName The name of the fade layer
	 * @param FadeDuration The duration of the fade effect in seconds
	 * @param FadeFunction The easing function type for the fade animation
	 * @param TargetColor The target color for the fade
	 * @param bIsFadeOut True for fade out, false for fade in
	 * @param Steps The number of animation steps for precision
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param ZOrder The Z-order for the fade widget
	 * @return True if the fade was successfully started
	 */
	static bool TryStartScreenFade(const TObjectPtr<const UObject> WorldContextObject, const FString& LayerName, float FadeDuration,
	                               EEasingFunc::Type FadeFunction, FLinearColor TargetColor, bool bIsFadeOut,
	                               int32 Steps = 2, float BlendExp = 2.0f, int32 ZOrder = 250);

	/**
	 * Checks if the global screen fade has completed.
	 *
	 * @param LayerName The name of the fade layer to check
	 * @return True if the screen fade has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Fade")
	static bool IsScreenFadeCompleted(const FString& LayerName);

	/**
	 * Resets all screen fade layers.
	 * This will stop all ongoing fades and clear all fade layers.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	static void ResetScreenFadeLayers();

#if WITH_EDITOR
	/**
	 * Editor-only: Starts a screen fade animation for preview (unified function for both fade in and fade out).
	 * Uses ShidenWidget's BaseLayer to display the fade.
	 */
	static bool TryStartScreenFadePreview(const TObjectPtr<const class UShidenWidget> ShidenWidget, const FString& LayerName, float FadeDuration,
	                                      EEasingFunc::Type FadeFunction, FLinearColor TargetColor, bool bIsFadeOut,
	                                      int32 Steps = 2, float BlendExp = 2.0f, int32 ZOrder = 250);
#endif

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void CopyToClipboard(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetClipboardContent(FString& ClipboardText);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static int32 GetParsedLength(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static FString GetCharactersWithParsedLength(const FString& Text, int32 Length);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility", meta = (DisplayName = "Parse Wait Time from Last Tag"))
	static UPARAM(DisplayName = "Success") bool TryParseWaitTimeFromLastTag(const FString& RawText, const int32 Length, float& WaitTime);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility",
		meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", Duration = "0.2", Keywords = "sleep"))
	static void MultiThreadDelay(UObject* WorldContextObject, const float Duration, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static UClass* ConstructClassFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Utility")
	static FString GetObjectPathFromClass(const UClass* Class);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility", meta = (DisplayName = "Get Sound Type from Sound Base"))
	static UPARAM(DisplayName = "Success") bool TryGetSoundTypeFromSoundBase(const USoundBase* SoundBase, EShidenSoundType& SoundType);

	UFUNCTION()
	static void InitCommandDefinitions();

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static const TMap<FString, FShidenCommandDefinition>& GetCommandDefinitionsCache();

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static FString GetCommandArgument(const FShidenCommand& Command, const FString& ArgName);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Utility")
	static bool IsValidSoftObjectPath(const FString& ObjectPath);

private:
	static FRegexPattern& GetSelfClosingTagPattern();

	static FRegexPattern& GetNonSelfClosingTagPattern();

	static FRegexPattern& GetWaitTimePattern();
};
