// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditor.h"
#include "ISettingsModule.h"
#include "ShidenEditorConfig.h"
#include "EditorSubsystem.h"
#include "Editor/EditorEngine.h"

#define LOCTEXT_NAMESPACE "FShidenEditorModule"

void FShidenEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Editor",
			"Plugins",
			"ShidenVisualNovelEditor",
			LOCTEXT("ShidenVisualNovelEditorName", "Shiden Visual Novel Editor"),
			LOCTEXT("ShidenVisualNovelEditorDescription", "Shiden Visual Novel Editor"),
			GetMutableDefault<UShidenEditorConfig>()
		);
	}
}

void FShidenEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings(
			"Editor",
			"Plugins",
			"ShidenVisualNovelEditor"
		);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FShidenEditorModule, ShidenEditor)