// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Config/ShidenProjectConfig.h"

SHIDENCORE_API UShidenProjectConfig::UShidenProjectConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PredefinedSystemVariable(FShidenPredefinedSystemVariable())
	, ScenarioDirectoryPath("/Game/Shiden/Scenarios/")
	, MacroDirectoryPath("/Game/Shiden/Macros/")
	, TargetFrameRate(60)
	, bAutoSaveOnMobileAppWillDeactivate(true)
{
	WidgetClass = TSoftClassPtr<UShidenWidget>(FSoftObjectPath(TEXT("/Shiden/Samples/WBP_ShidenWidgetSample.WBP_ShidenWidgetSample_C")));
	Presets = TArray<FShidenPreset>();
	AdditionalTextTypes = TMap<FString, FShidenTextType>();
	CommandDefinitions = {
		FSoftObjectPath(TEXT("/Shiden/Commands/DA_ShidenPrimaryCommandDefinitions.DA_ShidenPrimaryCommandDefinitions"))
	};
}

SHIDENCORE_API void UShidenProjectConfig::AddScenarioPath(const FGuid& ScenarioId, const FString ScenarioPath) {
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths.Add(ScenarioId, ScenarioPath);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& Paths) {
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths = Paths;
	ShidenProjectConfig->ScenarioPaths.KeySort([](const FGuid& A, const FGuid& B) {
		return A < B;
	});
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioDirectoryPath(const FString Path) {
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetMacroDirectoryPath(const FString Path) {
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->MacroDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::AddPreset(const FShidenPreset& Preset) {
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Add(Preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::RemovePreset(const FShidenPreset& Preset) {
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault <UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Remove(Preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}