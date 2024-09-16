// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenCommandFunctionLibrary.h"
#include "Engine/Engine.h"

SHIDENCORE_API void UShidenCommandFunctionLibrary::RegisterScenarioProperty(const FString CommandName, const FString Key, const FString Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

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

SHIDENCORE_API void UShidenCommandFunctionLibrary::RemoveScenarioProperty(const FString CommandName, const FString Key)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);
	
	if (!ShidenSubsystem->CurrentScenarioProperties.Contains(CommandName)) {
		return;
	}

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->CurrentScenarioProperties;

	if (CurrentScenarioProperties.Contains(CommandName)) {
		CurrentScenarioProperties[CommandName].ScenarioProperties.Remove(Key);
	}
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::ClearScenarioProperties(const FString CommandName)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->CurrentScenarioProperties.Remove(CommandName);
}
