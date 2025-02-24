// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableDefinition.h"
#include "ShidenVariableType.h"
#include "ShidenPredefinedSystemVariableDefinition.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenPredefinedSystemVariableDefinition: public FShidenVariableDefinition
{
	GENERATED_BODY()
	
	FString GetVariable() const
	{
		if (Getter)
		{
			return Getter();
		}
		return DefaultValue;
	}

	void SetVariable(const FString& InValue) const
	{
		if (Setter)
		{
			Setter(InValue);
		}
	}
	
	FShidenPredefinedSystemVariableDefinition()
		: FShidenVariableDefinition(), Getter(nullptr), Setter(nullptr)
	{
	}

	FShidenPredefinedSystemVariableDefinition(FString InName, const EShidenVariableType InType, FString InDefaultValue, const bool InIsReadOnly, const TFunction<FString()>& InGetter, const TFunction<void(FString)>& InSetter)
	{
		Name = InName;
		Type = InType;
		DefaultValue = InDefaultValue;
		IsReadOnly = InIsReadOnly;
		Getter = InGetter;
		Setter = InSetter;
	}

private:
	TFunction<FString()> Getter;

	TFunction<void(FString)> Setter;
};
