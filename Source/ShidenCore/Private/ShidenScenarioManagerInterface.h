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
	void GetShidenInput(TMap <FString, bool>& DigitalInputs);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void PlaySound2d(const FShidenSoundInfo SoundInfo, float& Duration, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void StopSound2d(const int32 TrackId, const EShidenSoundType Type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void StopVoiceAuto();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void PlayForceFeedback(const FString& ForceFeedbackEffectPath, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void CallMacroAsParallel(const FString& NewProcessName, const UObject* HookedObject);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void SetScenarioWidget(const UShidenWidget* Widget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Misc")
	void Destroy();
};
