// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenScenarioStruct.h"

UShidenScenario* FShidenScenarioStruct::ToShidenScenario()
{
	const TObjectPtr<UShidenScenario> Scenario = NewObject<UShidenScenario>();
	Scenario->ScenarioId = ScenarioId;
	Scenario->Note = Note;
	Scenario->Commands = Commands;
	Scenario->MacroParameterDefinitions = MacroParameterDefinitions;
	Scenario->LocalVariableDefinitions = LocalVariableDefinitions;
	return Scenario;
}

FShidenScenarioStruct::FShidenScenarioStruct(const UShidenScenario* InScenario, const FString& InPluginVersion)
{
	ScenarioId = InScenario->ScenarioId;
	Note = InScenario->Note;
	Commands = InScenario->Commands;
	MacroParameterDefinitions = InScenario->MacroParameterDefinitions;
	LocalVariableDefinitions = InScenario->LocalVariableDefinitions;
	PluginVersion = InPluginVersion;
}
