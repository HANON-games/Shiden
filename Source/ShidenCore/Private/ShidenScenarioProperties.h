// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenScenarioProperties.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FShidenScenarioProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Subsystem|Internal")
	TMap<FString, FString> ScenarioProperties;
};
