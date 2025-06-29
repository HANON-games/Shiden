// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenReadLines.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenReadLines
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TSet<FGuid> ReadLineHashes;
};
