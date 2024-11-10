// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommand.h"
#include "Engine/DataAsset.h"
#include "Variable/ShidenVariableType.h"
#include "ShidenScenario.generated.h"

USTRUCT(BlueprintType)
struct FShidenMacroArgument
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenVariableType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString DefaultValue;

	FShidenMacroArgument() {
		Name = "";
		Type = EShidenVariableType::String;
		DefaultValue = "";
	}
};

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
	TArray<FShidenCommand> Commands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenMacroArgument> MacroArguments;

	UShidenScenario() {
		ScenarioId = FGuid::NewGuid();
		Commands = TArray<FShidenCommand>();
		MacroArguments = TArray<FShidenMacroArgument>();
	}

	virtual void PostDuplicate(bool bDuplicateForPie) override;
};
