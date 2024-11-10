// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenImageInfo.generated.h"

USTRUCT(BlueprintType)
struct FShidenImageInfo
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    FString ImagePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
    FLinearColor ImageColor = FLinearColor::White;
};