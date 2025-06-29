// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenScenarioProperty.h"
#include "ShidenScenarioProperties.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenScenarioProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FShidenScenarioProperty> ScenarioProperties;
};
