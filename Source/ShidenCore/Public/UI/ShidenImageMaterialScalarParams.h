// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShidenImageMaterialScalarParams.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenImageMaterialScalarParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TObjectPtr<UImage> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FName ParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float MoveDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TEnumAsByte<EEasingFunc::Type> MoveFunction = EEasingFunc::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float EasingAlpha = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float StartValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float EndValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float BlendExp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 Steps = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString OwnerProcessName;
};
