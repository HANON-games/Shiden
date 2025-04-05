// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommand.h"
#include "ShidenCommandsStruct.generated.h"

/// <summary>
/// This struct is used to convert FShidenCommand Array to Json.
/// </summary>
USTRUCT(BlueprintType)
struct SHIDENEDITOR_API FShidenCommandsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TArray<FShidenCommand> Commands;
};
