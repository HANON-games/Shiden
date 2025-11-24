// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Scenario/ShidenManager.h"
#include "AudioDevice.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Config/ShidenProjectConfig.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Save/ShidenSaveBlueprintLibrary.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Scenario/ShidenParallelProcessManagerInterface.h"
#include "DSP/VolumeFader.h"
#include "Engine/TimerHandle.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenBlueprintLibrary.h"

AShidenManager::AShidenManager(): ShidenWidget(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
	FCoreDelegates::ApplicationWillDeactivateDelegate.AddUObject(this, &AShidenManager::OnApplicationWillDeactivate);
}

void AShidenManager::FindShidenDigitalInput_Implementation(const UInputAction* InputAction, bool& bValue, bool& bSuccess)
{
	if (InputAction)
	{
		bValue = GetInputActionValue(InputAction).Get<bool>();
		bSuccess = true;
		return;
	}

	bValue = false;
	bSuccess = false;
}

void AShidenManager::FindShidenAxis1DInput_Implementation(const UInputAction* InputAction, float& X, bool& bSuccess)
{
	if (InputAction)
	{
		X = GetInputActionValue(InputAction).Get<FInputActionValue::Axis1D>();
		bSuccess = true;
		return;
	}

	X = 0.0f;
	bSuccess = false;
}

void AShidenManager::FindShidenAxis2DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, bool& bSuccess)
{
	if (InputAction)
	{
		const FInputActionValue::Axis2D Value = GetInputActionValue(InputAction).Get<FInputActionValue::Axis2D>();
		X = Value.X;
		Y = Value.Y;
		bSuccess = true;
		return;
	}

	X = Y = 0.0f;
	bSuccess = false;
}

void AShidenManager::FindShidenAxis3DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, float& Z, bool& bSuccess)
{
	if (InputAction)
	{
		const FInputActionValue::Axis3D Value = GetInputActionValue(InputAction).Get<FInputActionValue::Axis3D>();
		X = Value.X;
		Y = Value.Y;
		Z = Value.Z;
		bSuccess = true;
		return;
	}

	X = Y = Z = 0.0f;
	bSuccess = false;
}

FInputActionValue AShidenManager::GetInputActionValue(const UInputAction* InputAction) const
{
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		return FInputActionValue(InputAction->ValueType, FVector::ZeroVector);
	}

	const TObjectPtr<const UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!Subsystem)
	{
		return FInputActionValue(InputAction->ValueType, FVector::ZeroVector);
	}

	return Subsystem->GetPlayerInput()->GetActionValue(InputAction);
}

void AShidenManager::PlaySound_Implementation(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess)
{
	Duration = 0.0f;
	bSuccess = false;

	if (SoundInfo.SoundPath.IsEmpty() || SoundInfo.SoundPath.Equals(TEXT("None"), ESearchCase::IgnoreCase))
	{
		if (!bRegisterSound)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlaySound with empty path must have bRegisterSound=true."));
			return;
		}

		switch (SoundInfo.Type)
		{
		case EShidenSoundType::BGM:
			if (const TObjectPtr<UAudioComponent> Component = BGMComponents.FindRef(SoundInfo.TrackId); Component)
			{
				Component->FadeOut(SoundInfo.FadeDuration, 0.0f, SoundInfo.AudioFaderCurve);
				BGMComponents.Remove(SoundInfo.TrackId);
			}
			Duration = 0.0f;
			bSuccess = true;
			return;
		case EShidenSoundType::SE:
			UE_LOG(LogTemp, Warning, TEXT("PlaySound with empty path for SE is not implemented."));
			return;
		case EShidenSoundType::Voice:
			if (const TObjectPtr<UAudioComponent> Component = VoiceComponents.FindRef(SoundInfo.TrackId); Component)
			{
				Component->FadeOut(SoundInfo.FadeDuration, 0.0f, SoundInfo.AudioFaderCurve);
				VoiceComponents.Remove(SoundInfo.TrackId);
			}
			Duration = 0.0f;
			bSuccess = true;
			return;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown sound type: %d"), static_cast<int32>(SoundInfo.Type));
			return;
		}
	}

	UObject* Asset;
	if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(SoundInfo.SoundPath, Asset))
	{
		return;
	}

	const TObjectPtr<USoundBase> SoundAsset = Cast<USoundBase>(Asset);
	if (!SoundAsset)
	{
		return;
	}

	EShidenSoundType SoundType;
	if (!UShidenBlueprintLibrary::TryGetSoundTypeFromSoundBase(SoundAsset, SoundType)
		|| SoundInfo.Type != SoundType)
	{
		return;
	}

	switch (SoundType)
	{
	case EShidenSoundType::BGM:
	case EShidenSoundType::Voice:
		PlayBGMOrVoice(SoundInfo, SoundAsset, bRegisterSound);
		break;
	case EShidenSoundType::SE:
		PlaySE(SoundInfo, SoundAsset, bRegisterSound);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown sound type: %d"), static_cast<int32>(SoundType));
		return;
	}

	Duration = SoundAsset->GetDuration();
	bSuccess = true;
}

void AShidenManager::StopSound_Implementation(const int32& TrackId, const EShidenSoundType Type)
{
	switch (Type)
	{
	case EShidenSoundType::BGM:
		if (UAudioComponent* BGMComponent = BGMComponents.FindRef(TrackId); BGMComponent)
		{
			BGMComponent->SetActive(false);
			BGMComponents.Remove(TrackId);
		}
		break;
	case EShidenSoundType::SE:
		UE_LOG(LogTemp, Warning, TEXT("StopSound for SE is not implemented."));
		break;
	case EShidenSoundType::Voice:
		if (UAudioComponent* VoiceComponent = VoiceComponents.FindRef(TrackId); VoiceComponent)
		{
			VoiceComponent->SetActive(false);
			VoiceComponents.Remove(TrackId);
		}
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown sound type: %d"), static_cast<int32>(Type));
		break;
	}
}

void AShidenManager::StopVoices_Implementation()
{
	TArray<int32> Keys;
	VoiceComponents.GetKeys(Keys);
	for (const int32& Key : Keys)
	{
		StopSound_Implementation(Key, EShidenSoundType::Voice);
	}
}

void AShidenManager::AdjustBGMVolume_Implementation(const int32& TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FadeCurve)
{
	if (UAudioComponent* BGMComponent = BGMComponents.FindRef(TrackId); BGMComponent)
	{
		AdjustVolumeInternal(BGMComponent, VolumeDuration, VolumeLevel, FadeCurve);
	}
}

void AShidenManager::PauseAllSounds_Implementation(const bool bPause)
{
	for (const TPair<int32, UAudioComponent*>& Pair : BGMComponents)
	{
		if (Pair.Value)
		{
			Pair.Value->SetPaused(bPause);
		}
	}

	for (UAudioComponent*& Component : SEComponents)
	{
		if (Component)
		{
			Component->SetPaused(bPause);
		}
	}

	for (const TPair<int32, UAudioComponent*>& Pair : VoiceComponents)
	{
		if (Pair.Value)
		{
			Pair.Value->SetPaused(bPause);
		}
	}
}

void AShidenManager::PlaySE(const FShidenSoundInfo& SoundInfo, USoundBase* Sound, const bool bRegisterSound)
{
	if (SoundInfo.Type != EShidenSoundType::SE)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySE called with non-SE sound type."));
		return;
	}

	// Is fade out?
	if (SoundInfo.EndVolumeMultiplier <= 0.0f)
	{
		const TObjectPtr<UAudioComponent> Component = UGameplayStatics::SpawnSound2D(this, Sound, 1.0f, SoundInfo.PitchMultiplier, SoundInfo.StartTime);
		AdjustVolumeInternal(Component, 0.0f, SoundInfo.StartVolumeMultiplier, SoundInfo.AudioFaderCurve);
		if (bRegisterSound)
		{
			RegisterSound(SoundInfo, Component);
		}
		AdjustVolumeInternal(Component, SoundInfo.FadeDuration, 0.0f, SoundInfo.AudioFaderCurve);
		return;
	}

	// Is fade in?
	if (SoundInfo.StartVolumeMultiplier <= 0.0f)
	{
		const TObjectPtr<UAudioComponent> Component = UGameplayStatics::SpawnSound2D(this, Sound, 1.0f, SoundInfo.PitchMultiplier, SoundInfo.StartTime);
		if (bRegisterSound)
		{
			RegisterSound(SoundInfo, Component);
		}
		Component->FadeIn(SoundInfo.FadeDuration, SoundInfo.EndVolumeMultiplier, SoundInfo.StartTime, SoundInfo.AudioFaderCurve);
		return;
	}

	const TObjectPtr<UAudioComponent> Component = UGameplayStatics::SpawnSound2D(this, Sound, 1.0f, SoundInfo.PitchMultiplier, SoundInfo.StartTime);
	AdjustVolumeInternal(Component, 0.0f, SoundInfo.StartVolumeMultiplier, SoundInfo.AudioFaderCurve);
	if (bRegisterSound)
	{
		RegisterSound(SoundInfo, Component);
	}
	AdjustVolumeInternal(Component, SoundInfo.FadeDuration, SoundInfo.EndVolumeMultiplier, SoundInfo.AudioFaderCurve);
}

void AShidenManager::PlayBGMOrVoice(const FShidenSoundInfo& SoundInfo, USoundBase* Sound, const bool bRegisterSound)
{
	if (SoundInfo.Type == EShidenSoundType::SE)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayBGMOrVoice called with SE sound type."));
		return;
	}

	UAudioComponent* CurrentComponent = SoundInfo.Type == EShidenSoundType::BGM
		                                    ? BGMComponents.FindRef(SoundInfo.TrackId)
		                                    : VoiceComponents.FindRef(SoundInfo.TrackId);

	// Is fade out?
	if (SoundInfo.EndVolumeMultiplier <= 0.0f)
	{
		const bool bSamePath = CurrentComponent && CurrentComponent->Sound && UKismetSystemLibrary::GetPathName(CurrentComponent->Sound).Compare(UKismetSystemLibrary::GetPathName(Sound), ESearchCase::CaseSensitive) == 0;
		UAudioComponent* Component = bSamePath
			                             ? CurrentComponent
			                             : UGameplayStatics::SpawnSound2D(this, Sound, 1.0f, SoundInfo.PitchMultiplier, SoundInfo.StartTime);

		if (!bSamePath)
		{
			AdjustVolumeInternal(Component, 0.0f, SoundInfo.StartVolumeMultiplier, SoundInfo.AudioFaderCurve);
		}

		if (CurrentComponent && !bSamePath)
		{
			StopSound_Implementation(SoundInfo.TrackId, SoundInfo.Type);
		}

		if (bRegisterSound)
		{
			RegisterSound(SoundInfo, Component);
		}

		AdjustVolumeInternal(Component, SoundInfo.FadeDuration, 0.0f, SoundInfo.AudioFaderCurve);
		return;
	}

	// Is fade in?
	if (SoundInfo.StartVolumeMultiplier <= 0.0f)
	{
		StopSound_Implementation(SoundInfo.TrackId, SoundInfo.Type);
		const TObjectPtr<UAudioComponent> Component = UGameplayStatics::SpawnSound2D(this, Sound, 1.0f, SoundInfo.PitchMultiplier, SoundInfo.StartTime);
		if (bRegisterSound)
		{
			RegisterSound(SoundInfo, Component);
		}
		Component->FadeIn(SoundInfo.FadeDuration, SoundInfo.EndVolumeMultiplier, SoundInfo.StartTime, SoundInfo.AudioFaderCurve);
		return;
	}

	const bool bSamePath = CurrentComponent && CurrentComponent->Sound && UKismetSystemLibrary::GetPathName(CurrentComponent->Sound).Compare(UKismetSystemLibrary::GetPathName(Sound), ESearchCase::CaseSensitive) == 0;
	const TObjectPtr<UAudioComponent> Component = bSamePath
		                                              ? CurrentComponent
		                                              : UGameplayStatics::SpawnSound2D(this, Sound, 1.0f, SoundInfo.PitchMultiplier,
		                                                                               SoundInfo.StartTime);

	if (!bSamePath)
	{
		AdjustVolumeInternal(Component, 0.0f, SoundInfo.StartVolumeMultiplier, SoundInfo.AudioFaderCurve);
		if (CurrentComponent)
		{
			StopSound_Implementation(SoundInfo.TrackId, SoundInfo.Type);
		}
	}

	if (bRegisterSound)
	{
		RegisterSound(SoundInfo, Component);
	}

	AdjustVolumeInternal(Component, SoundInfo.FadeDuration, SoundInfo.EndVolumeMultiplier, SoundInfo.AudioFaderCurve);
}

void AShidenManager::RegisterSound(const FShidenSoundInfo& SoundInfo, UAudioComponent* AudioComponent)
{
	switch (SoundInfo.Type)
	{
	case EShidenSoundType::BGM:
		BGMComponents.Add(SoundInfo.TrackId, AudioComponent);
		AudioComponent->OnAudioFinishedNative.AddLambda([this](const UAudioComponent* Component)
		{
			if (Component->Sound && Component->Sound->VirtualizationMode == EVirtualizationMode::Disabled)
			{
				UE_LOG(LogTemp, Warning, TEXT("BGM finished playing: %s"), *Component->Sound->GetName());
				RemoveSound(EShidenSoundType::BGM);
			}
		});
		break;
	case EShidenSoundType::SE:
		SEComponents.Add(AudioComponent);
		AudioComponent->OnAudioFinishedNative.AddLambda([this](UAudioComponent* _) { RemoveSound(EShidenSoundType::SE); });
		break;
	case EShidenSoundType::Voice:
		VoiceComponents.Add(SoundInfo.TrackId, AudioComponent);
		AudioComponent->OnAudioFinishedNative.AddLambda([this](UAudioComponent* _) { RemoveSound(EShidenSoundType::Voice); });
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown sound type: %d"), static_cast<int32>(SoundInfo.Type));
		break;
	}
}

void AShidenManager::RemoveSound(const EShidenSoundType SoundType)
{
	switch (SoundType)
	{
	case EShidenSoundType::BGM:
		{
			TArray<int32> KeysToRemove;
			for (const TPair<int32, UAudioComponent*>& Pair : BGMComponents)
			{
				if (!Pair.Value || Pair.Value->GetPlayState() == EAudioComponentPlayState::Stopped)
				{
					KeysToRemove.Add(Pair.Key);
				}
			}
			for (const int32& Key : KeysToRemove)
			{
				BGMComponents.Remove(Key);
			}
			break;
		}
	case EShidenSoundType::SE:
		{
			for (int32 i = SEComponents.Num() - 1; i >= 0; --i)
			{
				if (!SEComponents[i] || SEComponents[i]->GetPlayState() == EAudioComponentPlayState::Stopped)
				{
					SEComponents.RemoveAt(i);
				}
			}
			break;
		}
	case EShidenSoundType::Voice:
		{
			TArray<int32> KeysToRemove;
			for (const TPair<int32, UAudioComponent*>& Pair : VoiceComponents)
			{
				if (!Pair.Value || Pair.Value->GetPlayState() == EAudioComponentPlayState::Stopped)
				{
					KeysToRemove.Add(Pair.Key);
				}
			}
			for (const int32& Key : KeysToRemove)
			{
				VoiceComponents.Remove(Key);
			}
			break;
		}
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown sound type: %d"), static_cast<int32>(SoundType));
		break;
	}
}

void AShidenManager::PlayForceFeedback_Implementation(const FString& ForceFeedbackEffectPath, bool& bSuccess)
{
	UObject* LoadedObject;
	if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(ForceFeedbackEffectPath, LoadedObject))
	{
		bSuccess = false;
		return;
	}

	if (UForceFeedbackEffect* ForceFeedbackEffect = Cast<UForceFeedbackEffect>(LoadedObject))
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			PlayerController->ClientPlayForceFeedback(ForceFeedbackEffect);
			bSuccess = true;
			return;
		}
	}

	bSuccess = false;
}

void AShidenManager::CallMacroAsParallel_Implementation(const FString& NewProcessName, UObject* CallerObject)
{
	if (!CallerObject)
	{
		UE_LOG(LogTemp, Error, TEXT("CallerObject is null"));
		return;
	}

	UWorld* World = CallerObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("CallerObject does not have a valid World"));
		return;
	}

	const TSubclassOf<AActor> ActorClass = GetParallelProcessManagerClass();
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ParallelProcessManagerClass is not loaded"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(*NewProcessName);
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

	if (AActor* Manager = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
	{
		const TScriptInterface<IShidenParallelProcessManagerInterface> ManagerInterface = TScriptInterface<IShidenParallelProcessManagerInterface>(Manager);
		ManagerInterface->Execute_StartProcessingMacro(Manager, NewProcessName, ShidenWidget, this, CallerObject);
		ParallelProcessManagers.Add(ManagerInterface);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn BP_ShidenParallelProcessManager"));
	}
}

void AShidenManager::Initialize_Implementation(const UShidenWidget* Widget)
{
	this->ShidenWidget = const_cast<UShidenWidget*>(Widget);
}

void AShidenManager::Destroy_Implementation()
{
	for (const TScriptInterface<IShidenParallelProcessManagerInterface>& Manager : ParallelProcessManagers)
	{
		if (AActor* Actor = Cast<AActor>(Manager.GetObject()))
		{
			Actor->Destroy();
		}
	}

	TArray<int32> Keys;
	BGMComponents.GetKeys(Keys);
	for (const int32& Key : Keys)
	{
		StopSound_Implementation(Key, EShidenSoundType::BGM);
	}

	for (UAudioComponent*& Component : SEComponents)
	{
		if (Component)
		{
			Component->SetActive(false);
		}
	}
	SEComponents.Empty();

	VoiceComponents.GetKeys(Keys);
	for (const int32& Key : Keys)
	{
		StopSound_Implementation(Key, EShidenSoundType::Voice);
	}

	AActor::Destroy();
}

void AShidenManager::OnApplicationWillDeactivate() const
{
	UE_LOG(LogTemp, Log, TEXT("Application will deactivate."));

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	if (!ProjectConfig->bAutoSaveOnMobileAppWillDeactivate)
	{
		return;
	}

	UTexture2D* Texture2D;
	ShidenWidget->CaptureScreenToTexture2D(Texture2D);

	if (!UShidenSaveBlueprintLibrary::TrySaveUserData(TEXT("Auto Save"), Texture2D, ShidenWidget->GetSaveSlotMetadata()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AutoSave failed on application will deactivate."));
		return;
	}

	if (!UShidenSaveBlueprintLibrary::TrySavePredefinedSystemData())
	{
		UE_LOG(LogTemp, Warning, TEXT("AutoSave predefined system data failed on application will deactivate."));
		return;
	}

	UShidenSaveBlueprintLibrary::TrySaveSystemData();
}

TSubclassOf<AActor> AShidenManager::GetParallelProcessManagerClass()
{
	static TSubclassOf<AActor> CachedClass = nullptr;

	if (!CachedClass)
	{
		const FString BlueprintPath = TEXT("/Shiden/Core/BP_ShidenParallelProcessManager.BP_ShidenParallelProcessManager_C");
		CachedClass = LoadClass<AActor>(nullptr, *BlueprintPath);

		if (!CachedClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load BP_ShidenParallelProcessManager blueprint class from path: %s"), *BlueprintPath);
		}
	}

	return CachedClass;
}

void AShidenManager::AdjustVolumeInternal(UAudioComponent* Component, const float AdjustVolumeDuration, const float AdjustVolumeLevel, const EAudioFaderCurve FadeCurve)
{
	if (!FMath::IsNearlyZero(AdjustVolumeLevel))
	{
		if (Component->Sound && Component->Sound->IsLooping() && !Component->IsActive()
			&& (Component->Sound->VirtualizationMode == EVirtualizationMode::Restart || Component->Sound->VirtualizationMode == EVirtualizationMode::Disabled))
		{
			Component->FadeIn(AdjustVolumeDuration, AdjustVolumeLevel, 0.0f, FadeCurve);
			return;
		}
		Component->AdjustVolume(AdjustVolumeDuration, AdjustVolumeLevel, FadeCurve);
		return;
	}

	// The UAudioComponent::AdjustVolume function treats a volume of 0 as a fade-out, after which volume adjustment is no longer possible.
	// To match the behavior of the volume adjustment function in the Config menu, if it is not EVirtualizationMode::Disabled,
	// a very small value close to 0 is specified so that it is not treated as a fade-out.

	constexpr float TargetVolumeLevel = UE_SMALL_NUMBER * 10;
	Component->AdjustVolume(AdjustVolumeDuration, TargetVolumeLevel, FadeCurve);

	FTimerHandle Handle;
	Component->GetWorld()
	         ->GetTimerManager()
	         .SetTimer(Handle, [Component]
	                   {
		                   if (!Component->IsActive())
		                   {
			                   return;
		                   }

		                   FAudioDevice* AudioDevice = Component->GetAudioDevice();
		                   if (!AudioDevice)
		                   {
			                   return;
		                   }

		                   AudioDevice->SendCommandToActiveSounds(
			                   Component->GetAudioComponentID(), [](FActiveSound& ActiveSound)
			                   {
				                   Audio::FVolumeFader& Fader = ActiveSound.ComponentVolumeFader;
				                   if (Fader.GetTargetVolume() <= TargetVolumeLevel)
				                   {
					                   Fader.SetVolume(0.0f);
				                   }
			                   });
	                   }, AdjustVolumeDuration, false
	         );
}
