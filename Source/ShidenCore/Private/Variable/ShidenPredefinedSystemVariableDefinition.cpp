// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Variable/ShidenPredefinedSystemVariableDefinition.h"

FString FShidenPredefinedSystemVariableDefinition::GetVariable() const
{
	if (Getter)
	{
		return Getter();
	}
	return DefaultValue;
}

void FShidenPredefinedSystemVariableDefinition::SetVariable(const FString& Value) const
{
	if (Setter)
	{
		Setter(Value);
	}
}

FShidenPredefinedSystemVariableDefinition::FShidenPredefinedSystemVariableDefinition()
	: FShidenVariableDefinition(), Getter(nullptr), Setter(nullptr)
{
}

FShidenPredefinedSystemVariableDefinition::FShidenPredefinedSystemVariableDefinition(const FString& Name, const EShidenVariableType Type, const EShidenAssetPathType AssetPathType,
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
