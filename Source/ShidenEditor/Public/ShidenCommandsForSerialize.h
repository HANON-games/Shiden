// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommand.h"
#include "ShidenCommandsForSerialize.generated.h"

USTRUCT(BlueprintType)
struct FShidenCommandsForSerialize
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TArray<FShidenCommand> Commands;
};
