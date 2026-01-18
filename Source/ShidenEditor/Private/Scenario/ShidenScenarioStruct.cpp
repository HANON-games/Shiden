// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Scenario/ShidenScenarioStruct.h"

UShidenScenario* FShidenScenarioStruct::ToShidenScenario()
{
	const TObjectPtr<UShidenScenario> Scenario = NewObject<UShidenScenario>();
	Scenario->ScenarioId = ScenarioId;
	Scenario->Note = Note;
	Scenario->Commands = Commands;
	Scenario->MacroParameterDefinitions = MacroParameterDefinitions;
	Scenario->LocalVariableDefinitions = LocalVariableDefinitions;

	// Convert WidgetClassOverride string to TSubclassOf
	if (!WidgetClassOverride.IsEmpty())
	{
		if (TObjectPtr<UClass> WidgetClass = LoadObject<UClass>(nullptr, *WidgetClassOverride))
		{
			Scenario->WidgetClassOverride = WidgetClass;
		}
	}

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

	// Convert WidgetClassOverride to string
	if (InScenario->WidgetClassOverride)
	{
		WidgetClassOverride = InScenario->WidgetClassOverride->GetPathName();
	}
}
