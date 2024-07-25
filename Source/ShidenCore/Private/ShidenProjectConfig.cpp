// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenProjectConfig.h"

SHIDENCORE_API UShidenProjectConfig::UShidenProjectConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PredefinedSystemVariable()
	, ScenarioDirectoryPath("/Game/Shiden/Scenarios/")
	, MacroDirectoryPath("/Game/Shiden/Macros/")
{
	Presets = TArray<FShidenPreset>();
	CommandDefinitions = {
		FSoftObjectPath(TEXT("/Shiden/Commands/DA_ShidenPrimaryCommandDefinitions.DA_ShidenPrimaryCommandDefinitions"))
	};
}

SHIDENCORE_API void UShidenProjectConfig::AddScenarioPath(const FGuid& scenarioId, const FString scenarioPath) {
	TObjectPtr <UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths.Add(scenarioId, scenarioPath);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& paths) {
	TObjectPtr <UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths = paths;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioDirectoryPath(const FString path) {
	TObjectPtr <UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioDirectoryPath = path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetMacroDirectoryPath(const FString path) {
	TObjectPtr <UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->MacroDirectoryPath = path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::AddPreset(const FShidenPreset& preset) {
	TObjectPtr <UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Add(preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::RemovePreset(const FShidenPreset& preset) {
	TObjectPtr <UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Remove(preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}