// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenCommand.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCommand
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

	TOptional<FString> GetArg(const FString& Key) const;

	TOptional<int32> GetArgAsInt(const FString& Key) const;

	TOptional<float> GetArgAsFloat(const FString& Key) const;

	TOptional<bool> GetArgAsBool(const FString& Key) const;

	TOptional<FVector2D> GetArgAsVector2D(const FString& Key) const;

	TOptional<FVector> GetArgAsVector(const FString& Key) const;
};
