// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenCore.h"
#include "Config/ShidenProjectConfig.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "ShidenNamespace"

void FShidenCoreModule::StartupModule()
{
#if WITH_EDITOR
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"ShidenVisualNovelEditor",
			LOCTEXT("ShidenVisualNovelEditorName", "Shiden Visual Novel Editor"),
			LOCTEXT("ShidenVisualNovelEditorDescription", "Shiden Visual Novel Editor"),
			GetMutableDefault<UShidenProjectConfig>()
		);
	}
#endif
}

void FShidenCoreModule::ShutdownModule()
{
#if WITH_EDITOR
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings(
			"Project",
			"Plugins",
			"ShidenVisualNovelEditor"
		);
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShidenCoreModule, ShidenCore)
