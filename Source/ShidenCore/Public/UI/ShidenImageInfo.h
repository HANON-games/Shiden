// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenImageInfo.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenImageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString ImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FLinearColor ImageColor = FLinearColor::White;
};
