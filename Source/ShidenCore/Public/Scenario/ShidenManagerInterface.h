// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InputMappingContext.h"
#include "Audio/ShidenSoundInfo.h"
#include "UI/ShidenWidget.h"
#include "Engine/Texture2D.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void FindShidenDigitalInput(const UInputAction* InputAction, bool& bValue, bool& bSuccess);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound", meta = (bRegisterSound = true))
	void PlaySound(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void StopSound(const int32& TrackId, const EShidenSoundType Type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void StopVoices();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void AdjustBgmVolume(const int32& TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FadeCurve);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void PauseAllSounds(const bool bPause);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void PlayForceFeedback(const FString& ForceFeedbackEffectPath, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void CallMacroAsParallel(const FString& NewProcessName, const UObject* CallerObject);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void Initialize(const UShidenWidget* Widget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void Destroy();
};
