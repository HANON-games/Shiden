// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenAssetPathType.h"
#include "ShidenVariableType.h"
#include "ShidenVariableDefinition.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenVariableDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Type = EShidenVariableType::Boolean;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (EditCondition = "Type == EShidenVariableType::AssetPath"))
	EShidenAssetPathType AssetPathType = EShidenAssetPathType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bIsReadOnly = false;

	static const FShidenVariableDefinition Empty;

	bool operator==(const FShidenVariableDefinition& Other) const noexcept
	{
		return Name == Other.Name;
	}

	bool operator==(const FString& Other) const noexcept
	{
		return Name == Other;
	}
};
