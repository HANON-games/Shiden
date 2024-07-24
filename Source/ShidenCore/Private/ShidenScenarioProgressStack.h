// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCommand.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	FGuid ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	int CurrentIndex = 0;
};

USTRUCT(BlueprintType)
struct FShidenCancelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	bool bIsRequested;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	FString Reason;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	EShidenCancelType Type;

	FShidenCancelInfo()
	{
		bIsRequested = false;
		Reason = "";
		Type = EShidenCancelType::None;
	}
};

USTRUCT(BlueprintType)
struct FShidenScenarioProgressStack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TArray<FShidenScenarioProgress> Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	FShidenCancelInfo CancelInfo;

	FShidenScenarioProgressStack()
	{
		Stack = TArray<FShidenScenarioProgress>();
		CancelInfo = FShidenCancelInfo();
	}
};