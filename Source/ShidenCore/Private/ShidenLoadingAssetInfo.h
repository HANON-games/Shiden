// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenLoadingAssetInfo.generated.h"

USTRUCT(BlueprintType)
struct FShidenLoadingAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int ScenarioIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString ObjectPath;

	bool operator==(const FShidenLoadingAssetInfo& that) const noexcept
	{
		return ObjectPath == that.ObjectPath;
	}
};
