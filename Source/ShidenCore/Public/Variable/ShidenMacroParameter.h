// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableDefinition.h"
#include "ShidenMacroParameter.generated.h"

USTRUCT(BlueprintType)
struct FShidenMacroParameter : public FShidenVariableDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bIsEnumParameter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FString> EnumValues;
};
