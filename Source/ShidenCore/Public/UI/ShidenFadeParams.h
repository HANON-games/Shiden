// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Kismet/KismetMathLibrary.h"
#include "ShidenFadeParams.generated.h"

USTRUCT(BlueprintType)
struct FShidenFadeParams
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    float FadeDuration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    FLinearColor StartColor = FLinearColor::Black;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    FLinearColor TargetColor = FLinearColor::Black;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    bool bIsFadeOut = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    float EasingAlpha = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    TEnumAsByte<EEasingFunc::Type> FadeFunction = EEasingFunc::Linear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    float BlendExp = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    int32 Steps = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    FString OwnerProcessName;
};
