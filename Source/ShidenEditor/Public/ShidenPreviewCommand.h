// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommand.h"
#include "ShidenPreviewCommand.generated.h"

USTRUCT(BlueprintType)
struct FShidenPreviewCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 Index = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenCommand Command;
};