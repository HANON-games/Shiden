// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Command/ShidenCommandFunctionLibrary.h"
#include "Engine/Engine.h"
#include "System/ShidenSubsystem.h"

SHIDENCORE_API void UShidenCommandFunctionLibrary::RegisterScenarioProperty(const FString& CommandName, const FString& Key, const FString& Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	if (!CurrentScenarioProperties.Contains(CommandName))
	{
		FShidenScenarioProperties Properties;
		Properties.ScenarioProperties = TMap<FString, FString>();
		Properties.ScenarioProperties.Add(Key, Value);
		CurrentScenarioProperties.Add(CommandName, Properties);
	}
	else
	{
		CurrentScenarioProperties[CommandName].ScenarioProperties.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::RemoveScenarioProperty(const FString& CommandName, const FString& Key)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProperties.Contains(CommandName))
	{
		return;
	}

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	if (CurrentScenarioProperties.Contains(CommandName))
	{
		CurrentScenarioProperties[CommandName].ScenarioProperties.Remove(Key);
	}
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::ClearScenarioProperties(const FString& CommandName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->ScenarioProperties.Remove(CommandName);
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::IsTrue(const FString& CommandArgument, bool& bResult)
{
	bResult = CommandArgument.Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
}

SHIDENCORE_API void UShidenCommandFunctionLibrary::IsEmptyAssetPath(const FString& CommandArgument, bool& bResult)
{
	bResult = CommandArgument.IsEmpty() || CommandArgument.Compare(TEXT("None"), ESearchCase::CaseSensitive) == 0;
}
