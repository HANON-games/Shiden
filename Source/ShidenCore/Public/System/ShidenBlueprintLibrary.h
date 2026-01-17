// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenSubsystem.h"
#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "System/ShidenBacklogItem.h"
#include "Command/ShidenCommand.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ShidenTextType.h"
#include "ShidenBlueprintLibrary.generated.h"

struct FShidenCommandDefinition;
enum class EShidenSoundType : uint8;
enum class EShidenSoundFadeType : uint8;
class USoundBase;

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
	 * @param bSaveScenarioProperty Whether to save the scenario property immediately
	 * @return True if the fade was successfully started
	 */
	static bool TryStartScreenFade(const TObjectPtr<const UObject> WorldContextObject, const FString& LayerName, float FadeDuration,
	                               EEasingFunc::Type FadeFunction, FLinearColor TargetColor, bool bIsFadeOut,
	                               int32 Steps = 2, float BlendExp = 2.0f, int32 ZOrder = 250, bool bSaveScenarioProperty = true);

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

	/**
	 * Plays a global BGM that persists across level transitions.
	 * This function uses the ShidenSubsystem for audio management.
	 *
	 * @param WorldContextObject World context object
	 * @param TrackId The track ID for the BGM
	 * @param Sound The sound asset to play (must be of BGM type)
	 * @param FadeType The type of fade
	 * @param Volume The target volume level
	 * @param Pitch The pitch multiplier
	 * @param StartTime The start time of the sound
	 * @param FadeDuration The duration of the fade effect
	 * @param FadeCurve The fade curve to use
	 * @param Duration [out] The duration of the sound
	 * @param bSaveScenarioProperty Whether to save the scenario property immediately
	 * @return True if the BGM was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (WorldContext = "WorldContextObject", DisplayName = "Play Global BGM", Volume = "1.0", Pitch = "1.0", bSaveScenarioProperty = "true"))
	static UPARAM(DisplayName = "Success") bool TryPlayGlobalBGM(const UObject* WorldContextObject, int32 TrackId, USoundBase* Sound, EShidenSoundFadeType FadeType, float Volume, float Pitch, float StartTime,
																 float FadeDuration, EAudioFaderCurve FadeCurve, bool bSaveScenarioProperty, float& Duration);

	/**
	 * Stops a global BGM.
	 *
	 * @param TrackId The track ID of the BGM to stop
	 * @param bSaveScenarioProperty Whether to remove the scenario property
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (DisplayName = "Stop Global BGM"))
	static void StopGlobalBGM(int32 TrackId,   bool bSaveScenarioProperty = false);

	/**
	 * Stops all global BGMs.
	 *
	 * @param bSaveScenarioProperty Whether to remove the scenario properties
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (DisplayName = "Stop Global BGMs"))
	static void StopGlobalBGMs(bool bSaveScenarioProperty = false);
	
	/**
	 * Adjusts the volume of a global BGM.
	 *
	 * @param TrackId The track ID of the BGM to adjust
	 * @param VolumeDuration The duration of the volume adjustment
	 * @param VolumeLevel The target volume level
	 * @param FadeCurve The fade curve to use
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (DisplayName = "Adjust Global BGM Volume"))
	static void AdjustGlobalBGMVolume(int32 TrackId, float VolumeDuration, float VolumeLevel, EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);

	/**
	 * Pauses or unpauses a specific global BGM.
	 *
	 * @param TrackId The track ID of the BGM to pause/unpause
	 * @param bPause True to pause, false to unpause
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (DisplayName = "Pause Global BGM"))
	static void PauseGlobalBGM(int32 TrackId, bool bPause);

	/**
	 * Pauses or unpauses all global BGMs.
	 *
	 * @param bPause True to pause, false to unpause
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (DisplayName = "Pause All Global BGMs"))
	static void PauseAllGlobalBGMs(bool bPause);

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

	// Internal function to reset screen fade layers for InitializeSubsystemState in UShidenSubsystem
	UFUNCTION()
	static void ResetScreenFadeLayersCore(UShidenSubsystem* ShidenSubsystem);

private:
	static FRegexPattern& GetSelfClosingTagPattern();

	static FRegexPattern& GetNonSelfClosingTagPattern();

	static FRegexPattern& GetWaitTimePattern();
};
