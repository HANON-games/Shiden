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

template <typename T, typename TMember>
void UShidenEditorConfig::UpdateConfig(TMember UShidenEditorConfig::* MemberPtr, const T& Value)
{
	const TObjectPtr<UShidenEditorConfig> Config = GetMutableDefault<UShidenEditorConfig>();
	Config->*MemberPtr = Value;
	Config->SaveConfig(CPF_Config, *Config->GetDefaultConfigFilename());
	Config->TryUpdateDefaultConfigFile();
}

SHIDENEDITOR_API void UShidenEditorConfig::SetEditScenarioPath(const FString& PackageName)
{
	UpdateConfig(&UShidenEditorConfig::EditScenarioPath, PackageName);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetPreviewSound(const bool bEnabled)
{
	UpdateConfig(&UShidenEditorConfig::bPreviewSound, bEnabled);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetShidenDebuggerRefreshInterval(const float Interval)
{
	UpdateConfig(&UShidenEditorConfig::ShidenDebuggerRefreshInterval, Interval);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetAutoSaveScenario(const bool bEnabled)
{
	UpdateConfig(&UShidenEditorConfig::bAutoSaveScenario, bEnabled);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetDefaultCommand(const FString& CommandName)
{
	UpdateConfig(&UShidenEditorConfig::DefaultCommand, CommandName);
}
