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

SHIDENCORE_API void UShidenProjectConfig::AddScenarioPath(const FGuid& ScenarioId, const FString ScenarioPath) {
	TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths.Add(ScenarioId, ScenarioPath);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& Paths) {
	TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths = Paths;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioDirectoryPath(const FString Path) {
	TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetMacroDirectoryPath(const FString Path) {
	TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->MacroDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::AddPreset(const FShidenPreset& Preset) {
	TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Add(Preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::RemovePreset(const FShidenPreset& Preset) {
	TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Remove(Preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}