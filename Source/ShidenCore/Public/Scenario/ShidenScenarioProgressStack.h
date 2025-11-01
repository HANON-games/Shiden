// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenScenarioProgressStack.generated.h"

UENUM(BlueprintType)
enum class EShidenCancelType : uint8
{
	None = 0,
	Immediately,
	OnCurrentCommandEnd
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenScenarioProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FGuid ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 CurrentIndex = 0;
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCancelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bIsCancelRequested = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString CancelReason;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	EShidenCancelType CancelType = EShidenCancelType::Immediately;
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenScenarioProgressStack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TArray<FShidenScenarioProgress> Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenCancelInfo CancelInfo;
	
	void UpdateCurrentScenarioIndex(const int32 Index)
	{
		if (!IsEmpty())
		{
			Stack.Last().CurrentIndex = Index;
		}
	}

	int32 GetCurrentScenarioIndex() const noexcept
	{
		return IsEmpty() ? INDEX_NONE : Stack.Last().CurrentIndex;
	}

	bool IsEmpty() const noexcept
	{
		return Stack.Num() == 0;
	}
};
