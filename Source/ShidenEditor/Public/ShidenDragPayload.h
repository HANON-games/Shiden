// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenDragPayload.generated.h"

UCLASS(BlueprintType)
class SHIDENEDITOR_API UShidenDragPayload : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "SVNInternal")
	TArray<int32> SourceIndexes;
};
