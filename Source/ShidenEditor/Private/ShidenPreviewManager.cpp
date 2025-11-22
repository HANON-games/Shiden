// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenPreviewManager.h"

#include "System/ShidenBlueprintLibrary.h"

UShidenPreviewManager::UShidenPreviewManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShidenPreviewManager::FindShidenDigitalInput_Implementation(const UInputAction* InputAction, bool& bValue, bool& bSuccess)
{
	bValue = false;
	bSuccess = false;
}

void UShidenPreviewManager::FindShidenAxis1DInput_Implementation(const UInputAction* InputAction, float& X, bool& bSuccess)
{
	X = 0.0f;
	bSuccess = false;
}

void UShidenPreviewManager::FindShidenAxis2DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, bool& bSuccess)
{
	X = 0.0f;
	Y = 0.0f;
	bSuccess = false;
}

void UShidenPreviewManager::FindShidenAxis3DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, float& Z, bool& bSuccess)
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
	bSuccess = false;
}

void UShidenPreviewManager::PlaySound_Implementation(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess)
{
	if (SoundInfo.SoundPath.IsEmpty() || SoundInfo.SoundPath.Equals(TEXT("None"), ESearchCase::IgnoreCase))
	{
		if (!bRegisterSound)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlaySound with empty path must have bRegisterSound=true."));
			Duration = 0.0f;
			bSuccess = false;
			return;
		}

		StopSound_Implementation(SoundInfo.TrackId, SoundInfo.Type);
		Duration = 0.0f;
		bSuccess = true;
		return;
	}

	UObject* Asset;
	if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(SoundInfo.SoundPath, Asset))
	{
		Duration = 0.0f;
		bSuccess = false;
		return;
	}

	const TObjectPtr<USoundBase> SoundAsset = Cast<USoundBase>(Asset);
	if (!SoundAsset)
	{
		Duration = 0.0f;
		bSuccess = false;
		return;
	}

	EShidenSoundType SoundType;
	UShidenBlueprintLibrary::TryGetSoundTypeFromSoundBase(SoundAsset, SoundType);
	if (SoundInfo.Type != SoundType)
	{
		Duration = 0.0f;
		bSuccess = false;
		return;
	}

	switch (SoundType)
	{
	case EShidenSoundType::BGM:
		CachedBGMInfo.Add(SoundInfo.TrackId, SoundInfo);
		break;
	case EShidenSoundType::SE:
		CachedSEInfo.Add(SoundInfo);
		break;
	case EShidenSoundType::Voice:
		CachedVoiceInfo.Add(SoundInfo.TrackId, SoundInfo);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown sound type: %d"), static_cast<int32>(SoundType));
		Duration = 0.0f;
		bSuccess = false;
		return;
	}

	Duration = SoundAsset->Duration;
	bSuccess = true;
}

void UShidenPreviewManager::StopSound_Implementation(const int32& TrackId, const EShidenSoundType Type)
{
	switch (Type)
	{
	case EShidenSoundType::BGM:
		CachedBGMInfo.Remove(TrackId);
		break;
	case EShidenSoundType::Voice:
		CachedVoiceInfo.Remove(TrackId);
		break;
	default:
		break;
	}
}

void UShidenPreviewManager::StopVoices_Implementation()
{
	TArray<int32> KeysToRemove;
	for (const TPair<int32, FShidenSoundInfo>& Pair : CachedVoiceInfo)
	{
		KeysToRemove.Add(Pair.Key);
	}

	for (const int32 Key : KeysToRemove)
	{
		StopSound_Implementation(Key, EShidenSoundType::Voice);
	}
}

void UShidenPreviewManager::AdjustBGMVolume_Implementation(const int32& TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FaderCurve)
{
	if (FShidenSoundInfo* BGMInfo = CachedBGMInfo.Find(TrackId))
	{
		BGMInfo->StartVolumeMultiplier = BGMInfo->EndVolumeMultiplier;
		BGMInfo->EndVolumeMultiplier = VolumeLevel;
		BGMInfo->AudioFaderCurve = FaderCurve;
		BGMInfo->FadeDuration = VolumeDuration;
	}
}

void UShidenPreviewManager::PauseAllSounds_Implementation(const bool bPause)
{
	UE_LOG(LogTemp, Error, TEXT("PauseAllSounds is not implemented."));
}

void UShidenPreviewManager::PlayForceFeedback_Implementation(const FString& ForceFeedbackEffectPath, bool& bSuccess)
{
	UE_LOG(LogTemp, Error, TEXT("PlayForceFeedback is not implemented."));
	bSuccess = false;
}

void UShidenPreviewManager::CallMacroAsParallel_Implementation(const FString& NewProcessName, UObject* CallerObject)
{
	UE_LOG(LogTemp, Error, TEXT("CallMacroAsParallel is not implemented."));
}

void UShidenPreviewManager::Initialize_Implementation(const UShidenWidget* ShidenWidget)
{
	UE_LOG(LogTemp, Error, TEXT("Initialize is not implemented."));
}

void UShidenPreviewManager::Destroy_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("Destroy is not implemented."));
}

void UShidenPreviewManager::GetSoundInfo(TMap<int32, FShidenSoundInfo>& BGMInfo, TArray<FShidenSoundInfo>& SEInfo, TMap<int32, FShidenSoundInfo>& VoiceInfo) const
{
	BGMInfo = this->CachedBGMInfo;
	SEInfo = this->CachedSEInfo;
	VoiceInfo = this->CachedVoiceInfo;
}

void UShidenPreviewManager::ClearSoundInfo()
{
	CachedBGMInfo.Empty();
	CachedSEInfo.Empty();
	CachedVoiceInfo.Empty();
}

const FText UShidenPreviewManager::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}
