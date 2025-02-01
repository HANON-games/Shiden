// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommand.h"
#include "Engine/DataAsset.h"
#include "Variable/ShidenMacroParameter.h"
#include "Variable/ShidenVariableDefinition.h"
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
	TArray<FShidenCommand> Commands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenMacroParameter> MacroParameterDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenVariableDefinition> LocalVariableDefinitions;

	UShidenScenario()
	{
		ScenarioId = FGuid::NewGuid();
		Commands = TArray<FShidenCommand>();
		MacroParameterDefinitions = TArray<FShidenMacroParameter>();
		LocalVariableDefinitions = TArray<FShidenVariableDefinition>();
	}

	virtual void PostDuplicate(bool bDuplicateForPie) override;
};
