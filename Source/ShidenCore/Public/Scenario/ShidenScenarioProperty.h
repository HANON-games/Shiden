// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenScenarioProperty.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenScenarioProperty
{
	GENERATED_BODY()

	FShidenScenarioProperty()
	{
	}

	explicit FShidenScenarioProperty(const FString& Value)
		: PropertyValue(Value)
	{
	}

	bool TryConvertToStringArray(TArray<FString>& Values) const;

	bool TryConvertToStringMap(TMap<FString, FString>& Values) const;

	FString GetValueAsString() const;

private:
	UPROPERTY()
	FString PropertyValue;
};
