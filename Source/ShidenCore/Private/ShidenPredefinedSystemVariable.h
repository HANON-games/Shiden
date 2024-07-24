// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ShidenReadLines.h"
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
	bool bShowMouseCursorWhenStarting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	int LanguageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float SecondsToWaitForEachLetter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float WaitTimeOnAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bStopVoiceWhenNextDialogueStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	TObjectPtr <UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bUseAddToPlayerScreenInsteadOfAddToViewport;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bAutoRemoveWidgetOnEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bSkipUnread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	int TargetFrameRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	TMap<FGuid, FShidenReadLines> MarkedReadLines;

	FShidenPredefinedSystemVariable()
	{
		SkipSpeedRate = 10.0f;
		VoiceVolumeRate = 1.0f;
		SeVolumeRate = 1.0f;
		BgmVolumeRate = 1.0f;
		bShowMouseCursorWhenStarting = true;
		LanguageIndex = 0;
		SecondsToWaitForEachLetter = 0.1f;
		WaitTimeOnAutoMode = 3.0f;
		bStopVoiceWhenNextDialogueStart = true;
		bUseAddToPlayerScreenInsteadOfAddToViewport = false;
		bAutoRemoveWidgetOnEnd = true;
		MappingContext = LoadObject<UInputMappingContext>(NULL, TEXT("/Shiden/Misc/EnhancedInput/IMC_ShidenDefault.IMC_ShidenDefault"), NULL, LOAD_None, NULL);
		bSkipUnread = false;
		TargetFrameRate = 60;
		MarkedReadLines = TMap<FGuid, FShidenReadLines>();
	}
};