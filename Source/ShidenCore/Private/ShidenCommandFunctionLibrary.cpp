// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenCommandFunctionLibrary.h"

SHIDENCORE_API void UShidenCommandFunctionLibrary::RegisterCurrentScenarioProperty(const FString CommandName, const FString Key, const FString Value)
{
	UShidenSubsystem* ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	if (!ShidenSubsystem) {
		return;
	}

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->CurrentScenarioProperties;

	if (!CurrentScenarioProperties.Contains(CommandName)) {
		FShidenScenarioProperties Properties;
		Properties.ScenarioProperties = TMap<FString, FString>();
		Properties.ScenarioProperties.Add(Key, Value);
		CurrentScenarioProperties.Add(CommandName, Properties);
	}
	else {
		CurrentScenarioProperties[CommandName].ScenarioProperties.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::RemoveCurrentScenarioProperty(const FString CommandName, const FString Key)
{
	UShidenSubsystem* ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	if (!ShidenSubsystem) {
		return;
	}
	
	if (!ShidenSubsystem->CurrentScenarioProperties.Contains(CommandName)) {
		return;
	}

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->CurrentScenarioProperties;

	if (CurrentScenarioProperties.Contains(CommandName)) {
		CurrentScenarioProperties[CommandName].ScenarioProperties.Remove(Key);
	}
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::ClearCurrentScenarioProperties(const FString CommandName)
{
	UShidenSubsystem* ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	if (!ShidenSubsystem) {
		return;
	}

	ShidenSubsystem->CurrentScenarioProperties.Remove(CommandName);
}
