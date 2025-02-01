// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenPreset.generated.h"

USTRUCT(BlueprintType)
struct FShidenPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString CommandName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap<FString, FString> Args;
};
