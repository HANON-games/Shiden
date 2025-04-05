// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShidenImageFadeParams.generated.h"

USTRUCT(BlueprintType)
struct FShidenImageFadeParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TObjectPtr<UImage> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float FadeDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bIsWhiteFade = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bToBeTransparent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TEnumAsByte<EEasingFunc::Type> FadeFunction = EEasingFunc::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float EasingAlpha = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float BlendExp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 Steps = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString OwnerProcessName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool ClearImageOnCompleted = false;
};
