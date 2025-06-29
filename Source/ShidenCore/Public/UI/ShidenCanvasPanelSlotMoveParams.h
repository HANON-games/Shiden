// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Kismet/KismetMathLibrary.h"
#include "Components/CanvasPanel.h"
#include "ShidenCanvasPanelSlotMoveParams.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCanvasPanelSlotMoveParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TObjectPtr<UCanvasPanelSlot> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float MoveDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TEnumAsByte<EEasingFunc::Type> MoveFunction = EEasingFunc::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float EasingAlpha = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bChangePosition = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FVector2D StartPosition = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FVector2D EndPosition = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bChangeSize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FVector2D StartSize = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FVector2D EndSize = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	float BlendExp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 Steps = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString OwnerProcessName;
};
