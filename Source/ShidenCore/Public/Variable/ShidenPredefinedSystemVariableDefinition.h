// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableDefinition.h"
#include "ShidenVariableType.h"
#include "ShidenPredefinedSystemVariableDefinition.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenPredefinedSystemVariableDefinition : public FShidenVariableDefinition
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

	void SetVariable(const FString& Value) const
	{
		if (Setter)
		{
			Setter(Value);
		}
	}

	FShidenPredefinedSystemVariableDefinition()
		: FShidenVariableDefinition(), Getter(nullptr), Setter(nullptr)
	{
	}

	FShidenPredefinedSystemVariableDefinition(const FString& Name, const EShidenVariableType Type, const EShidenAssetPathType AssetPathType,
	                                          const FString& DefaultValue, const bool bIsReadOnly,
	                                          const TFunction<FString()>& Getter, const TFunction<void(FString)>& Setter)
		: Getter(Getter), Setter(Setter)
	{
		this->Name = Name;
		this->Type = Type;
		this->AssetPathType = AssetPathType;
		this->DefaultValue = DefaultValue;
		this->bIsReadOnly = bIsReadOnly;
	}

private:
	TFunction<FString()> Getter;

	TFunction<void(FString)> Setter;
};
