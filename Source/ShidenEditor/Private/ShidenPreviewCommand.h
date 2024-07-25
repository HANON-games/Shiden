// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCommand.h"
#include "ShidenPreviewCommand.generated.h"

USTRUCT(BlueprintType)
struct FShidenPreviewCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	int Index = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FShidenCommand Command;
};
