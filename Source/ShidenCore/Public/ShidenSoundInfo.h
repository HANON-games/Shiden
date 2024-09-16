// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Components/AudioComponent.h"
#include "ShidenSoundInfo.generated.h"

UENUM(BlueprintType)
enum class EShidenSoundType : uint8
{
	None = 0,
	BGM,
	SE,
	Voice
};

USTRUCT(BlueprintType)
struct FShidenSoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	int32 TrackId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	EShidenSoundType Type = EShidenSoundType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	FString SoundPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	float StartVolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	float EndVolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	float StartTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	EAudioFaderCurve AudioFaderCurve = EAudioFaderCurve::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	float FadeDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Sound")
	bool bAutoStopDisabled = false;
};
