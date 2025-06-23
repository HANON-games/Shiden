// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenEditorConfig.h"

SHIDENEDITOR_API UShidenEditorConfig::UShidenEditorConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , EditScenarioPath("")
	  , bPreviewSound(false)
	  , ShidenDebuggerRefreshInterval(0.1f)
	  , bAutoSaveScenario(false)
	  , DefaultCommand("Text")
	  , PluginVersion(FShidenPluginVersion())
{
}

SHIDENEDITOR_API void UShidenEditorConfig::SetEditScenarioPath(const FString& Path)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->EditScenarioPath = Path;
	UpdateConfig(ShidenEditorConfig);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetPreviewSound(const bool bEnabled)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->bPreviewSound = bEnabled;
	UpdateConfig(ShidenEditorConfig);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetShidenDebuggerRefreshInterval(const float Interval)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->ShidenDebuggerRefreshInterval = Interval;
	UpdateConfig(ShidenEditorConfig);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetAutoSaveScenario(const bool bEnabled)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->bAutoSaveScenario = bEnabled;
	UpdateConfig(ShidenEditorConfig);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetDefaultCommand(const FString& CommandName)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->DefaultCommand = CommandName;
	UpdateConfig(ShidenEditorConfig);
}

SHIDENEDITOR_API void UShidenEditorConfig::UpdateConfig(const TObjectPtr<UShidenEditorConfig> Config)
{
	Config->SaveConfig(CPF_Config, *Config->GetDefaultConfigFilename());
	Config->TryUpdateDefaultConfigFile();
}
