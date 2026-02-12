// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Config/ShidenEditorConfig.h"
#include "System/ShidenScenarioSyncManager.h"
#include "UObject/UnrealType.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"

UShidenEditorConfig::UShidenEditorConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , DefaultCommand("Text")
	  , bExpandPresets(false)
	  , PluginVersion(FShidenPluginVersion())
	  , bEnableScenarioSync(false)
	  , ScenarioSyncFormat(EShidenScenarioSyncFormat::CSV)
	  , ScenarioSyncDirectoryPath("")
{
}

FName UShidenEditorConfig::GetContainerName() const
{
	return TEXT("Editor");
}

FName UShidenEditorConfig::GetCategoryName() const
{
	return TEXT("Plugins");
}

FName UShidenEditorConfig::GetSectionName() const
{
	return TEXT("ShidenVisualNovelEditor");
}

#define LOCTEXT_NAMESPACE "FShidenEditorModule"

FText UShidenEditorConfig::GetSectionText() const
{
	return LOCTEXT("ShidenVisualNovelEditorName", "Shiden Visual Novel Editor");
}

FText UShidenEditorConfig::GetSectionDescription() const
{
	return LOCTEXT("ShidenVisualNovelEditorDescription", "Shiden Visual Novel Editor Project Settings");
}

#undef LOCTEXT_NAMESPACE

void UShidenEditorConfig::SetDefaultCommand(const FString& CommandName)
{
	if (const TObjectPtr<UShidenEditorConfig> EditorConfig = GetMutableDefault<UShidenEditorConfig>())
	{
		EditorConfig->DefaultCommand = CommandName;
		EditorConfig->SaveConfig();
		EditorConfig->TryUpdateDefaultConfigFile();
	}
}

void UShidenEditorConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	// Handle scenario sync settings changes
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, bEnableScenarioSync))
	{
		if (bEnableScenarioSync)
		{
			if (ScenarioSyncDirectoryPath.IsEmpty())
			{
				const FText WarningTitle = NSLOCTEXT("ShidenNamespace", "ScenarioSyncDirectoryEmptyTitle", "Sync Directory Not Set");
				const FText WarningMessage = NSLOCTEXT("ShidenNamespace", "ScenarioSyncDirectoryEmptyMessage",
				                                       "Please set the Scenario Sync Directory Path before enabling scenario sync.");
				FMessageDialog::Open(EAppMsgType::Ok, WarningMessage, WarningTitle);
				bEnableScenarioSync = false;
				return;
			}

			// Export all scenarios when sync is enabled
			const FText InfoTitle = NSLOCTEXT("ShidenNamespace", "ScenarioSyncEnabledTitle", "Scenario Sync Enabled");
			const FText InfoMessage = NSLOCTEXT("ShidenNamespace", "ScenarioSyncEnabledMessage",
			                                    "Scenario sync has been enabled. All scenarios will be exported to the sync directory.");
			FMessageDialog::Open(EAppMsgType::Ok, InfoMessage, InfoTitle);

			UShidenScenarioSyncManager::ExportAllScenarios();
			UShidenScenarioSyncManager::StartWatchingDirectory();
		}
		else
		{
			// Stop watching when sync is disabled
			UShidenScenarioSyncManager::StopWatchingDirectory();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, ScenarioSyncDirectoryPath) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, ScenarioSyncFormat))
	{
		if (bEnableScenarioSync)
		{
			// Re-export all scenarios when format changes
			UShidenScenarioSyncManager::StopWatchingDirectory();
			UShidenScenarioSyncManager::ExportAllScenarios();
			UShidenScenarioSyncManager::StartWatchingDirectory();
		}
	}
}
