// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenBacklogItem.generated.h"

USTRUCT(BlueprintType)
struct FShidenBacklogItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString Text;
};
