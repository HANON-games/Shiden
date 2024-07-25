// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenReadLines.generated.h"

USTRUCT(BlueprintType)
struct FShidenReadLines
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Subsystem|Internal")
	TSet<FGuid> Ids;
};
