// Copyright (c) 2025 HANON. All Rights Reserved.

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

	FString GetArg(const FString& Key) const;

	int32 GetArgAsInt(const FString& Key) const;

	float GetArgAsFloat(const FString& Key) const;

	bool GetArgAsBool(const FString& Key) const;

	FVector2D GetArgAsVector2D(const FString& Key) const;

	FVector GetArgAsVector(const FString& Key) const;
};
