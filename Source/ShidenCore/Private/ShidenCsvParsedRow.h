// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCsvParsedRow.generated.h"

USTRUCT(BlueprintType)
struct FShidenCsvParsedRow
{
    GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Utility")
	TArray<FString> Row;
};