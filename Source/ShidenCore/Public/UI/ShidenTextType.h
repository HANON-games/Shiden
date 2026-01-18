// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenTextType.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenTextType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	bool bShouldShowClickWaitingGlyph = false;
};
