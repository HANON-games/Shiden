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
	WidgetClass = Cast<UClass>(FSoftObjectPath(TEXT("/Shiden/Samples/WBP_ShidenWidgetSample.WBP_ShidenWidgetSample_C")).TryLoad());
	Presets = TMap<FString, FShidenPreset>();
	AdditionalTextTypes = TMap<FString, FShidenTextType>();
	CommandDefinitions = {
		FSoftObjectPath(TEXT("/Shiden/Commands/DA_ShidenPrimaryCommandDefinitions.DA_ShidenPrimaryCommandDefinitions"))
	};
	UserVariableDefinitions = TArray<FShidenVariableDefinition>();
	SystemVariableDefinitions = TArray<FShidenVariableDefinition>();
}

SHIDENCORE_API void UShidenProjectConfig::AddScenarioPath(const FGuid& ScenarioId, const FString& ScenarioPath)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths.Add(ScenarioId, ScenarioPath);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& Paths)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths = Paths;
	ShidenProjectConfig->ScenarioPaths.KeySort([](const FGuid& A, const FGuid& B)
	{
		return A < B;
	});
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetMacroDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->MacroDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::AddPreset(const FString& Name, const FShidenPreset& Preset)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Add(Name, Preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::GetPreset(const FString& Name, FShidenPreset& Preset, bool& bSuccess)
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->Presets.Contains(Name))
	{
		Preset = ShidenProjectConfig->Presets[Name];
		bSuccess = true;
		return;
	}
	bSuccess = false;
}

SHIDENCORE_API void UShidenProjectConfig::RemovePreset(const FString& Name)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Remove(Name);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetUserVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->UserVariableDefinitions = Definitions;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}

SHIDENCORE_API void UShidenProjectConfig::SetSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->SystemVariableDefinitions = Definitions;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
}
