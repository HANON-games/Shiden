// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenSoundType.h"
#include "Components/AudioComponent.h"
#include "ShidenSoundInfo.generated.h"

USTRUCT(BlueprintType)
struct FShidenSoundInfo
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
};
