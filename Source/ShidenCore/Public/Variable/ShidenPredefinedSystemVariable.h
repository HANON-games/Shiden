// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Audio/ShidenVoiceStopCondition.h"
#include "System/ShidenReadLines.h"
#include "Kismet/GameplayStatics.h"
#include "ShidenPredefinedSystemVariable.generated.h"

USTRUCT(BlueprintType)
struct FShidenPredefinedSystemVariable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float SkipSpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float VoiceVolumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float SeVolumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float BgmVolumeRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	int32 LanguageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float SecondsToWaitForEachLetter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float WaitTimeOnAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	EShidenVoiceStopCondition VoiceStopCondition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bSkipUnread;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	FString ClickWaitingGlyph;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	FString PlatformName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	TMap<FGuid, FShidenReadLines> ScenarioReadLines;

	FShidenPredefinedSystemVariable()
	{
		SkipSpeedRate = 10.0f;
		VoiceVolumeRate = 1.0f;
		SeVolumeRate = 1.0f;
		BgmVolumeRate = 1.0f;
		LanguageIndex = 0;
		SecondsToWaitForEachLetter = 0.08f;
		WaitTimeOnAutoMode = 3.0f;
		VoiceStopCondition = EShidenVoiceStopCondition::None;
		bSkipUnread = false;
		ClickWaitingGlyph = FString();
		PlatformName = UGameplayStatics::GetPlatformName();
		ScenarioReadLines = TMap<FGuid, FShidenReadLines>();
	}
};