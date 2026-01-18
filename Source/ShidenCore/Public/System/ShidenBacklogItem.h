// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommand.h"
#include "ShidenBacklogItem.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenBacklogItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Backlog")
	FShidenCommand Command;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Backlog")
	TMap<FString, FString> AdditionalProperties;
};
