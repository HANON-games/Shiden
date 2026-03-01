// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scenario/ShidenManagerInterface.h"
#include "ShidenParallelProcessManagerInterface.h"
#include "UI/ShidenWidget.h"
#include "ShidenManager.generated.h"

class USoundBase;

UCLASS(BlueprintType, Blueprintable)
class SHIDENCORE_API AShidenManager : public AActor, public IShidenManagerInterface
{
	GENERATED_BODY()

public:
	AShidenManager();

	virtual void FindShidenDigitalInput_Implementation(const UInputAction* InputAction, bool& bValue, bool& bSuccess) override;

	virtual void FindShidenAxis1DInput_Implementation(const UInputAction* InputAction, float& X, bool& bSuccess) override;

	virtual void FindShidenAxis2DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, bool& bSuccess) override;

	virtual void FindShidenAxis3DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, float& Z, bool& bSuccess) override;

	virtual void PlaySound_Implementation(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess) override;

	virtual void StopSound_Implementation(const int32 TrackId, const EShidenSoundType Type) override;

	virtual void StopSounds_Implementation(const EShidenSoundType Type) override;

	virtual void PauseSound_Implementation(const int32 TrackId, const EShidenSoundType Type, const bool bPause) override;

	virtual void AdjustBGMVolume_Implementation(const int32 TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FadeCurve) override;

	virtual void PauseAllSounds_Implementation(const bool bPause) override;

	virtual void PlayForceFeedback_Implementation(const FString& ForceFeedbackEffectPath, bool& bSuccess) override;

	virtual void CallMacroInParallel_Implementation(const FString& NewProcessName, UObject* CallerObject) override;

	virtual void GetVoicePlaybackState_Implementation(const int32 TrackId, EAudioComponentPlayState& PlayState, float& AudioLevel) override;

	virtual void Initialize_Implementation(const UShidenWidget* Widget) override;

	virtual void Destroy_Implementation() override;

	void OnApplicationWillDeactivate() const;

private:
	void PlaySE(const FShidenSoundInfo& SoundInfo, USoundBase* Sound, bool bRegisterSound);

	void PlayBGMOrVoice(const FShidenSoundInfo& SoundInfo, USoundBase* Sound, bool bRegisterSound);

	void RegisterSound(const FShidenSoundInfo& SoundInfo, TObjectPtr<UAudioComponent> AudioComponent);

	void RemoveSound(EShidenSoundType SoundType);

	static void AdjustVolumeInternal(TObjectPtr<UAudioComponent> Component, float AdjustVolumeDuration, float AdjustVolumeLevel, EAudioFaderCurve FadeCurve);

	FInputActionValue GetInputActionValue(const UInputAction* InputAction) const;

	static TSubclassOf<AActor> GetParallelProcessManagerClass();

	UPROPERTY()
	TObjectPtr<UShidenWidget> ShidenWidget;

	UPROPERTY()
	TMap<int32, TObjectPtr<UAudioComponent>> BGMComponents;

	UPROPERTY()
	TArray<TObjectPtr<UAudioComponent>> SEComponents;

	UPROPERTY()
	TMap<int32, TObjectPtr<UAudioComponent>> VoiceComponents;

	UPROPERTY()
	TArray<TScriptInterface<IShidenParallelProcessManagerInterface>> ParallelProcessManagers;
};
