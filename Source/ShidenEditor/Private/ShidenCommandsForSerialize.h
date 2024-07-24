// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCommand.h"
#include "ShidenCommandsForSerialize.generated.h"

USTRUCT(BlueprintType)
struct FShidenCommandsForSerialize
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	TArray <FShidenCommand> Commands;
};
