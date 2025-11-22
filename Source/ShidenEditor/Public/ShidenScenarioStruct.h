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
	FString PluginVersion;

	UPROPERTY()
	TArray<FShidenCommand> Commands;

	UPROPERTY()
	TArray<FShidenMacroParameter> MacroParameterDefinitions;

	UPROPERTY()
	TArray<FShidenVariableDefinition> LocalVariableDefinitions;

	UShidenScenario* ToShidenScenario();

	explicit FShidenScenarioStruct(const UShidenScenario* InScenario, const FString& InPluginVersion);

	FShidenScenarioStruct() = default;
};
