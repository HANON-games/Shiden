// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenLoadingAssetInfo.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenLoadingAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FGuid ScenarioGuid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 ScenarioIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString ObjectPath;

	bool operator==(const FShidenLoadingAssetInfo& Other) const noexcept
	{
		return ObjectPath == Other.ObjectPath;
	}
};
