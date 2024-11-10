// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenCommand.generated.h"

USTRUCT(BlueprintType)
struct FShidenCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FGuid CommandId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString CommandName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap <FString, FString> Args;
};
