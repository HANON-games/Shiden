// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InputMappingContext.h"
#include "Audio/ShidenSoundInfo.h"
#include "UI/ShidenWidget.h"
#include "ShidenManagerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UShidenManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenManagerInterface
{
	GENERATED_BODY()

public:
	/**
	 * Finds the current state of a digital input action.
	 * @param InputAction The input action to check for digital input state
	 * @param bValue [out] Current boolean state of the input
	 * @param bSuccess [out] True if the input action was successfully found and queried
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void FindShidenDigitalInput(const UInputAction* InputAction, bool& bValue, bool& bSuccess);

	/**
	 * Finds the current state of a 1D axis input action.
	 * @param InputAction The input action to check for 1D axis input state
	 * @param X [out] Current float value of the 1D axis input
	 * @param bSuccess [out] True if the input action was successfully found and queried
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void FindShidenAxis1DInput(const UInputAction* InputAction, float& X, bool& bSuccess);

	/**
	 * Finds the current state of a 2D axis input action.
	 * @param InputAction The input action to check for 2D axis input state
	 * @param X [out] Current float value of the X axis
	 * @param Y [out] Current float value of the Y axis
	 * @param bSuccess [out] True if the input action was successfully found and queried
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void FindShidenAxis2DInput(const UInputAction* InputAction, float& X, float& Y, bool& bSuccess);

	/**
	 * Finds the current state of a 3D axis input action.
	 * @param InputAction The input action to check for 3D axis input state
	 * @param X [out] Current float value of the X axis
	 * @param Y [out] Current float value of the Y axis
	 * @param Z [out] Current float value of the Z axis
	 * @param bSuccess [out] True if the input action was successfully found and queried
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void FindShidenAxis3DInput(const UInputAction* InputAction, float& X, float& Y, float& Z, bool& bSuccess);

	/**
	 * Attempts to play a sound with the specified sound information and settings.
	 * @param SoundInfo The sound information containing audio file path, track ID, and playback settings
	 * @param bRegisterSound True if the sound should be registered for tracking and management
	 * @param Duration [out] Duration of the sound in seconds
	 * @param bSuccess [out] True if the sound was successfully started
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (bRegisterSound = true))
	void PlaySound(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess);

	/**
	 * Stops a specific sound by track ID and sound type.
	 * @param TrackId The unique identifier of the sound track to stop
	 * @param Type The type of sound to stop (BGM or Voice)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void StopSound(const int32& TrackId, const EShidenSoundType Type);

	/**
	 * Stops all currently playing voice sounds.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void StopVoices();

	/**
	 * Adjusts the volume of a background music track with fade effects.
	 * @param TrackId The unique identifier of the BGM track to adjust
	 * @param VolumeDuration The duration of the volume fade transition in seconds
	 * @param VolumeLevel The target volume level
	 * @param FaderCurve The curve type to use for the volume fade transition
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound", DisplayName = "Adjust BGM Volume")
	void AdjustBGMVolume(const int32& TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FaderCurve);

	/**
	 * Pauses or resumes all currently playing sounds.
	 * @param bPause True to pause all sounds, false to resume all paused sounds
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void PauseAllSounds(const bool bPause);

	/**
	 * Attempts to play a force feedback effect for haptic controller feedback.
	 * @param ForceFeedbackEffectPath The file path to the force feedback effect asset
	 * @param bSuccess [out] True if the force feedback effect was successfully started
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void PlayForceFeedback(const FString& ForceFeedbackEffectPath, bool& bSuccess);

	/**
	 * Executes a macro as a parallel process with the specified process name.
	 * @param NewProcessName The name to assign to the new parallel process
	 * @param CallerObject The object that initiated this macro call
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void CallMacroAsParallel(const FString& NewProcessName, UObject* CallerObject);

	/**
	 * Initializes the Shiden manager with the widget.
	 * @param ShidenWidget The Shiden widget
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void Initialize(const UShidenWidget* ShidenWidget);

	/**
	 * Destroys the Shiden manager.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void Destroy();
};
