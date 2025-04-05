// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenCommand.generated.h"

USTRUCT(BlueprintType)
struct FShidenCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FGuid CommandId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString CommandName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap<FString, FString> Args;

	FString GetArg(const FString& Key) const
	{
		return Args.FindRef(Key);
	}

	int32 GetArgAsInt(const FString& Key) const
	{
		return FCString::Atoi(*Args.FindRef(Key));
	}

	float GetArgAsFloat(const FString& Key) const
	{
		return FCString::Atof(*Args.FindRef(Key));
	}

	bool GetArgAsBool(const FString& Key) const
	{
		return Args.FindRef(Key).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
	}

	FVector2D GetArgAsVector2D(const FString& Key) const
	{
		FVector2D Result;
		Result.InitFromString(Args.FindRef(Key));
		return Result;
	}

	FVector GetArgAsVector(const FString& Key) const
	{
		FVector Result;
		Result.InitFromString(Args.FindRef(Key));
		return Result;
	}
};
