// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenBacklogItem.generated.h"

USTRUCT(BlueprintType)
struct FShidenBacklogItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Subsystem|Internal")
	FString SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Subsystem|Internal")
	FString Text;
};
