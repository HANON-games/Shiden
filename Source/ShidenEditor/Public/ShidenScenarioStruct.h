// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommand.h"
#include "Scenario/ShidenScenario.h"
#include "ShidenScenarioStruct.generated.h"

/// <summary>
/// This struct is used to convert UShidenScenario to Json.
/// </summary>
USTRUCT()
struct SHIDENEDITOR_API FShidenScenarioStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid ScenarioId;

	UPROPERTY()
	FString Note;

	UPROPERTY()
	TArray<FShidenCommand> Commands;

	UPROPERTY()
	TArray<FShidenMacroParameter> MacroParameterDefinitions;

	UPROPERTY()
	TArray<FShidenVariableDefinition> LocalVariableDefinitions;

	UShidenScenario* ToShidenScenario()
	{
		const TObjectPtr<UShidenScenario> Scenario = NewObject<UShidenScenario>();
		Scenario->ScenarioId = ScenarioId;
		Scenario->Note = Note;
		Scenario->Commands = Commands;
		Scenario->MacroParameterDefinitions = MacroParameterDefinitions;
		Scenario->LocalVariableDefinitions = LocalVariableDefinitions;
		return Scenario;
	}

	explicit FShidenScenarioStruct(const UShidenScenario* Scenario)
	{
		ScenarioId = Scenario->ScenarioId;
		Note = Scenario->Note;
		Commands = Scenario->Commands;
		MacroParameterDefinitions = Scenario->MacroParameterDefinitions;
		LocalVariableDefinitions = Scenario->LocalVariableDefinitions;
	}

	FShidenScenarioStruct()
	{
	}
};
