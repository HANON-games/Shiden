// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenScenarioProgressStack.generated.h"

UENUM(BlueprintType)
enum class EShidenCancelType: uint8
{
	None = 0,
	Immediately,
	OnCurrentCommandEnd
};

USTRUCT(BlueprintType)
struct FShidenScenarioProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FGuid ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 CurrentIndex = 0;
};

USTRUCT(BlueprintType)
struct FShidenCancelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bIsRequested = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString Reason;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	EShidenCancelType Type = EShidenCancelType::Immediately;
};

USTRUCT(BlueprintType)
struct FShidenScenarioProgressStack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TArray<FShidenScenarioProgress> Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenCancelInfo CancelInfo;
};