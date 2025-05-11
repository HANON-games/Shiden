// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenAssetPathType.h"
#include "ShidenVariableType.h"
#include "ShidenVariableDefinition.generated.h"

USTRUCT(BlueprintType)
struct FShidenVariableDefinition
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

	bool operator==(const FShidenVariableDefinition& That) const noexcept
	{
		return Name == That.Name;
	}

	bool operator==(const FString& That) const noexcept
	{
		return Name == That;
	}
};
