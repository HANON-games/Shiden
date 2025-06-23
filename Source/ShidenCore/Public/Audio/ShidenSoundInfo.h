// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenSoundType.h"
#include "Components/AudioComponent.h"
#include "ShidenSoundInfo.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenSoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 TrackId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	EShidenSoundType Type = EShidenSoundType::BGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString SoundPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float StartVolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float EndVolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float StartTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	EAudioFaderCurve AudioFaderCurve = EAudioFaderCurve::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float FadeDuration = 1.0f;

	FShidenSoundInfo()
	{
	}

	FShidenSoundInfo(const int32 TrackId, const EShidenSoundType Type, const FString& SoundPath,
	                 const float StartVolumeMultiplier, const float EndVolumeMultiplier, const float PitchMultiplier,
	                 const float StartTime, const EAudioFaderCurve AudioFaderCurve, const float FadeDuration)
		: TrackId(TrackId)
		  , Type(Type)
		  , SoundPath(SoundPath)
		  , StartVolumeMultiplier(StartVolumeMultiplier)
		  , EndVolumeMultiplier(EndVolumeMultiplier)
		  , PitchMultiplier(PitchMultiplier)
		  , StartTime(StartTime)
		  , AudioFaderCurve(AudioFaderCurve)
		  , FadeDuration(FadeDuration)
	{
	}
};
