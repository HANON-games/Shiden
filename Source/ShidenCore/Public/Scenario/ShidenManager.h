// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scenario/ShidenManagerInterface.h"
#include "InputMappingContext.h"
#include "ShidenParallelProcessManagerInterface.h"
#include "Audio/ShidenSoundInfo.h"
#include "UI/ShidenWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "Sound/SoundBase.h"
#include "Runtime/Launch/Resources/Version.h"
#include "ShidenManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SHIDENCORE_API AShidenManager : public AActor, public IShidenManagerInterface
{
	GENERATED_BODY()

public:
	AShidenManager();

	virtual void FindShidenDigitalInput_Implementation(const UInputAction* InputAction, bool& bValue, bool& bSuccess) override;

	virtual void FindShidenAxis1DInput_Implementation(const UInputAction* InputAction, float& X, bool& bSuccess) override;

	virtual void FindShidenAxis2DInput_Implementation(const UInputAction* InputAction, float& X, float& Y,  bool& bSuccess) override;

	virtual void FindShidenAxis3DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, float& Z, bool& bSuccess) override;
	
	virtual void PlaySound_Implementation(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess) override;

	virtual void StopSound_Implementation(const int32& TrackId, const EShidenSoundType Type) override;

	virtual void StopVoices_Implementation() override;

	virtual void AdjustBGMVolume_Implementation(const int32& TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FadeCurve) override;

	virtual void PauseAllSounds_Implementation(const bool bPause) override;

	virtual void PlayForceFeedback_Implementation(const FString& ForceFeedbackEffectPath, bool& bSuccess) override;

	virtual void CallMacroAsParallel_Implementation(const FString& NewProcessName, UObject* CallerObject) override;

	virtual void Initialize_Implementation(const UShidenWidget* Widget) override;

	virtual void Destroy_Implementation() override;

	void OnApplicationWillDeactivate() const;

private:
	void PlaySE(const FShidenSoundInfo& SoundInfo, USoundBase* Sound, bool bRegisterSound);

	void PlayBGMOrVoice(const FShidenSoundInfo& SoundInfo, USoundBase* Sound, bool bRegisterSound);

	void RegisterSound(const FShidenSoundInfo& SoundInfo, UAudioComponent* AudioComponent);

	void RemoveSound(EShidenSoundType SoundType);

	static void AdjustVolumeInternal(UAudioComponent* Component, float AdjustVolumeDuration, float AdjustVolumeLevel, EAudioFaderCurve FadeCurve);

	FInputActionValue GetInputActionValue(const UInputAction* InputAction) const;

	static TSubclassOf<AActor> GetParallelProcessManagerClass();

	UPROPERTY()
	UShidenWidget* ShidenWidget;

	UPROPERTY()
	TMap<int32, UAudioComponent*> BGMComponents;

	UPROPERTY()
	TArray<UAudioComponent*> SEComponents;
	
	UPROPERTY()
	TMap<int32, UAudioComponent*> VoiceComponents;
	
	UPROPERTY()
	TArray<TScriptInterface<IShidenParallelProcessManagerInterface>> ParallelProcessManagers;
};