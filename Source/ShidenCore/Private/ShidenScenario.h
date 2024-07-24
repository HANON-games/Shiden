// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShidenCommand.h"
#include "Engine/DataAsset.h"
#include "ShidenVariableType.h"
#include "ShidenScenario.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenScenario : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGuid ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Note;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray< FShidenCommand > Commands;

	// Arguments for macro
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg1Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Arg1Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg1DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg2Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Arg2Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg2DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg3Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Arg3Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg3DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg4Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Arg4Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg4DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg5Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Arg5Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Arg5DefaultValue;

	UShidenScenario() {
		ScenarioId = FGuid::NewGuid();
		Commands = TArray< FShidenCommand >();
	}

	virtual void PostDuplicate(bool bDuplicateForPIE) override;
};
