// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenTextType.generated.h"

USTRUCT(BlueprintType)
struct FShidenTextType
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	bool bShouldShowClickWaitingGlyph = false;
};
