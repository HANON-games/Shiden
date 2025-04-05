// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenLoadingAssetInfo.generated.h"

USTRUCT(BlueprintType)
struct FShidenLoadingAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 ScenarioIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString ObjectPath;

	bool operator==(const FShidenLoadingAssetInfo& That) const noexcept
	{
		return ObjectPath == That.ObjectPath;
	}
};
