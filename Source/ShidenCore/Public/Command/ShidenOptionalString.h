// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenOptionalString.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenOptionalString
{
	GENERATED_BODY()

	FShidenOptionalString() = default;

	// ReSharper disable once CppNonExplicitConvertingConstructor
	FShidenOptionalString(FString InValue) : bIsSet(true), Value(MoveTemp(InValue))
	{
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	FShidenOptionalString(const TCHAR* InValue) : bIsSet(true), Value(InValue)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Command")
	bool bIsSet = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Command", Meta = (EditCondition = "bIsSet"))
	FString Value;

	bool IsSet() const { return bIsSet; }

	FString GetValueOrDefault() const
	{
		return bIsSet ? Value : TEXT("");
	}
};
