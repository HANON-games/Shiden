// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

/**
 * Constants used throughout the ShidenEditor module
 */
namespace ShidenEditorConstants
{
	// Dialog Titles
	static const FText SaveAssetDialogTitle = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
	static const FText OpenFileDialogTitle = LOCTEXT("OpenFileDialogTitle", "Open File Dialog");
	static const FText SaveFileDialogTitle = LOCTEXT("SaveFileDialogTitle", "Save File Dialog");

	// Module Names
	static const FName MainFrameModuleName = TEXT("MainFrame");
	static const FName ContentBrowserModuleName = TEXT("ContentBrowser");
	static const FName AssetRegistryModuleName = TEXT("AssetRegistry");

	// Asset Paths and Class Names
	static const FString EnginePathPrefix = TEXT("/Engine/");
	static const FString ShidenCommandObjectClassName = TEXT("/Script/CoreUObject.Class'/Script/ShidenCore.ShidenCommandObject'");
	static const FString ShidenCommandObjectName = TEXT("ShidenCommandObject");
	static const FString ShidenInputBaseName = TEXT("EUW_ShidenInputBase");

	// Plugin Paths
	static const FString PluginRelativePath = TEXT("Shiden/Shiden.uplugin");

	// UI Text
	static const FText CommandSoftObjectPathDisplayName = FText::FromString(TEXT("Command Soft Object Path"));
	static const FText TemplateWidgetDisplayName = FText::FromString(TEXT("Template Widget"));
}

#undef LOCTEXT_NAMESPACE
