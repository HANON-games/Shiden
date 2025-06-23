// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenTextType.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenTextType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	bool bShouldShowClickWaitingGlyph = false;
};
