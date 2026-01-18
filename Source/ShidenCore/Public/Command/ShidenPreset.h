// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenPreset.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString CommandName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap<FString, FString> Args;
};
