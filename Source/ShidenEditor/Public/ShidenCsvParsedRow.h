// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenCsvParsedRow.generated.h"

USTRUCT(BlueprintType)
struct FShidenCsvParsedRow
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SvnInternal")
	TArray<FString> Row;
};
