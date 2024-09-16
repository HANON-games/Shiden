// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InputMappingContext.h"
#include "ShidenSoundInfo.h"
#include "ShidenWidget.h"
#include "ShidenScenarioManagerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UShidenScenarioManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenScenarioManagerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void EnableShidenInput(const UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void FindShidenDigitalInput(const UInputAction* InputAction, bool& bValue, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void PlaySound2d(const FShidenSoundInfo SoundInfo, float& Duration, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void StopSound2d(const int32 TrackId, const EShidenSoundType Type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void StopVoices(const bool ForceStopAll = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Sound")
	void AdjustBgmVolume(const int32 TrackId, const float VolumeDuration, const float VolumeLevel, const EAudioFaderCurve FadeCurve, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void PlayForceFeedback(const FString& ForceFeedbackEffectPath, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void CallMacroAsParallel(const FString& NewProcessName, const UObject* CallerObject);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void SetScenarioWidget(const UShidenWidget* Widget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void Destroy();
};