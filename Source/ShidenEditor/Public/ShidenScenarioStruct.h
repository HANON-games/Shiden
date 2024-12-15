#pragma once

#include "Command/ShidenCommand.h"
#include "Scenario/ShidenScenario.h"
#include "ShidenScenarioStruct.generated.h"

/// <summary>
/// This class is used to convert UShidenScenario to Json.
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
	TArray<FShidenMacroArgument> MacroArguments;

	UShidenScenario* ToShidenScenario()
	{
		UShidenScenario* Scenario = NewObject<UShidenScenario>();
		Scenario->ScenarioId = ScenarioId;
		Scenario->Note = Note;
		Scenario->Commands = Commands;
		Scenario->MacroArguments = MacroArguments;
		return Scenario;
	}

	explicit FShidenScenarioStruct(const UShidenScenario* Scenario)
	{
		ScenarioId = Scenario->ScenarioId;
		Note = Scenario->Note;
		Commands = Scenario->Commands;
		MacroArguments = Scenario->MacroArguments;
	}

	FShidenScenarioStruct()
	{
	}
};