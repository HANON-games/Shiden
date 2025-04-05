// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenScenarioProperties.generated.h"

USTRUCT(BlueprintType)
struct FShidenScenarioProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FString> ScenarioProperties;
};
