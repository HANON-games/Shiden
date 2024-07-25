// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/RetainerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShidenRetainerBoxMaterialScalarParams.generated.h"

USTRUCT(BlueprintType)
struct FShidenRetainerBoxMaterialScalarParams
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    TObjectPtr <URetainerBox> TargetRetainerBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    FName ParameterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    float MoveDuration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    TEnumAsByte <EEasingFunc::Type> MoveFunction = EEasingFunc::Linear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    float EasingAlpha = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    float StartValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    float EndValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    float BlendExp = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    int32 Steps = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
    FString OwnerProcessName;
};
