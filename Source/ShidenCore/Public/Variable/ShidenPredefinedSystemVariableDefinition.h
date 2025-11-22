// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableDefinition.h"
#include "ShidenVariableType.h"
#include "ShidenPredefinedSystemVariableDefinition.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenPredefinedSystemVariableDefinition : public FShidenVariableDefinition
{
	GENERATED_BODY()

	FString GetVariable() const;

	void SetVariable(const FString& Value) const;

	FShidenPredefinedSystemVariableDefinition();

	FShidenPredefinedSystemVariableDefinition(const FString& Name, const EShidenVariableType Type, const EShidenAssetPathType AssetPathType,
	                                          const FString& DefaultValue, const bool bIsReadOnly,
	                                          const TFunction<FString()>& Getter, const TFunction<void(FString)>& Setter);

private:
	TFunction<FString()> Getter;

	TFunction<void(FString)> Setter;
};
