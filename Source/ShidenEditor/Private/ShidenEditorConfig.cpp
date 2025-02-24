// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorConfig.h"

SHIDENEDITOR_API UShidenEditorConfig::UShidenEditorConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , EditScenarioPath("")
	  , bPreviewSound(false)
	  , ShidenDebuggerRefreshInterval(0.1f)
	  , bAutoSaveScenario(false)
	  , DefaultCommand("Text")
{
}

SHIDENEDITOR_API void UShidenEditorConfig::SetEditScenarioPath(const FString& Path)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->EditScenarioPath = Path;
	ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	ShidenEditorConfig->TryUpdateDefaultConfigFile();
}

SHIDENEDITOR_API void UShidenEditorConfig::SetPreviewSound(const bool bBool)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->bPreviewSound = bBool;
	ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	ShidenEditorConfig->TryUpdateDefaultConfigFile();
}

SHIDENEDITOR_API void UShidenEditorConfig::SetShidenDebuggerRefreshInterval(const float Interval)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->ShidenDebuggerRefreshInterval = Interval;
	ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	ShidenEditorConfig->TryUpdateDefaultConfigFile();
}

SHIDENEDITOR_API void UShidenEditorConfig::SetAutoSaveScenario(const bool bBool)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->bAutoSaveScenario = bBool;
	ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	ShidenEditorConfig->TryUpdateDefaultConfigFile();
}

SHIDENEDITOR_API void UShidenEditorConfig::SetDefaultCommand(const FString& CommandName)
{
	const TObjectPtr<UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault<UShidenEditorConfig>();
	ShidenEditorConfig->DefaultCommand = CommandName;
	ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	ShidenEditorConfig->TryUpdateDefaultConfigFile();
}
