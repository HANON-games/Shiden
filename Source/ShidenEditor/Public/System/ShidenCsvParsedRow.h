// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenCsvParsedRow.generated.h"

USTRUCT(BlueprintType)
struct SHIDENEDITOR_API FShidenCsvParsedRow
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SvnInternal")
	TArray<FString> Row;
};
