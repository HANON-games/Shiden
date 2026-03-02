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

	TOptional<FString> GetOptionalArg(const FString& Key) const;
	
	FString GetArg(const FString& Key) const;
	
	TOptional<int32> GetOptionalArgAsInt(const FString& Key) const;

	int32 GetArgAsInt(const FString& Key) const;
	
	TOptional<float> GetOptionalArgAsFloat(const FString& Key) const;

	float GetArgAsFloat(const FString& Key) const;

	TOptional<bool> GetOptionalArgAsBool(const FString& Key) const;
	
	bool GetArgAsBool(const FString& Key) const;

	TOptional<FVector2D> GetOptionalArgAsVector2D(const FString& Key) const;

	FVector2D GetArgAsVector2D(const FString& Key) const;
	
	TOptional<FVector> GetOptionalArgAsVector3D(const FString& Key) const;

	FVector GetArgAsVector3D(const FString& Key) const;
};
