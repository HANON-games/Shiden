// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableType.h"
#include "ShidenVariableDescriptor.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenVariableDescriptor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal|Variables")
	FString ScenarioKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal|Variables")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal|Variables")
	EShidenVariableType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal|Variables")
	FString Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bIsReadOnly = false;
	
	FShidenVariableDescriptor()
	{
		ScenarioKey = "";
		Name = "";
		Type = EShidenVariableType::Boolean;
		Value = "";
		DefaultValue = "";
		bIsReadOnly = false;
	}

	FShidenVariableDescriptor(const FString& Name, const EShidenVariableType VariableType, const FString& Value, const FString& DefaultValue, const bool bIsReadOnly)
		: Name(Name), Type(VariableType), Value(Value), DefaultValue(DefaultValue), bIsReadOnly(bIsReadOnly)
	{
	}

	FShidenVariableDescriptor(const FString& ScenarioKey, const FString& Name, const EShidenVariableType VariableType, const FString& Value, const FString& DefaultValue, const bool bIsReadOnly)
		: ScenarioKey(ScenarioKey), Name(Name), Type(VariableType), Value(Value), DefaultValue(DefaultValue), bIsReadOnly(bIsReadOnly)
	{
	}
};
