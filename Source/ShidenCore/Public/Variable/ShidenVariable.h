// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableType.h"
#include "ShidenVariable.generated.h"

USTRUCT(BlueprintType)
struct FShidenVariable
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, bool> BooleanVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, int> IntegerVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, float> FloatVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, FString> StringVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, FVector> Vector3Variables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, FVector2D> Vector2Variables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Variable")
	TMap<FString, EShidenVariableType> InternalKeys;

	FShidenVariable()
	{
		BooleanVariables = TMap<FString, bool>();
		IntegerVariables = TMap<FString, int>();
		FloatVariables = TMap<FString, float>();
		StringVariables = TMap<FString, FString>();
		Vector3Variables = TMap<FString, FVector>();
		Vector2Variables = TMap<FString, FVector2d>();
		InternalKeys = TMap<FString, EShidenVariableType>();
	}
};