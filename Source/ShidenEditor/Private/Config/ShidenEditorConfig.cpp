// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Config/ShidenEditorConfig.h"
#include "System/ShidenScenarioSyncManager.h"
#include "Misc/MessageDialog.h"

SHIDENEDITOR_API UShidenEditorConfig::UShidenEditorConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , ScenarioFilterPath("")
	  , EditScenarioPath("")
	  , bPreviewSound(false)
	  , PreviewSize(960.0f, 540.0f)
	  , ShidenDebuggerRefreshInterval(0.1f)
	  , bAutoSaveScenario(false)
	  , DefaultCommand("Text")
	  , bUseAdvancedMode(true)
	  , bShowAllVisibilityConditionItems(false)
      , bExpandPresets(false)
      , PluginVersion(FShidenPluginVersion())
	  , bEnableScenarioSync(false)
	  , ScenarioSyncFormat(EShidenScenarioSyncFormat::CSV)
	  , ScenarioSyncDirectoryPath(FDirectoryPath())
	  , bWatchFileAdditions(false)
	  , bWatchFileDeletions(false)
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

SHIDENEDITOR_API void UShidenEditorConfig::SetScenarioFilterPath(const FString& Path)
{
	UpdateConfig(&UShidenEditorConfig::ScenarioFilterPath, Path);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetEditScenarioPath(const FString& PackageName)
{
	UpdateConfig(&UShidenEditorConfig::EditScenarioPath, PackageName);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetPreviewSound(const bool bEnabled)
{
	UpdateConfig(&UShidenEditorConfig::bPreviewSound, bEnabled);
}

SHIDENEDITOR_API void UShidenEditorConfig::SetPreviewSize(const FVector2D& Size)
{
	UpdateConfig(&UShidenEditorConfig::PreviewSize, Size);
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

void UShidenEditorConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	// Handle scenario sync settings changes
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, bEnableScenarioSync))
	{
		if (bEnableScenarioSync)
		{
			if (ScenarioSyncDirectoryPath.Path.IsEmpty())
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
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, ScenarioSyncFormat) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, ScenarioSyncDirectoryPath))
	{
		if (bEnableScenarioSync)
		{
			// Re-export all scenarios when format or directory changes
			UShidenScenarioSyncManager::StopWatchingDirectory();
			UShidenScenarioSyncManager::ExportAllScenarios();
			UShidenScenarioSyncManager::StartWatchingDirectory();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, bWatchFileDeletions))
	{
		if (bWatchFileDeletions)
		{
			const FText WarningTitle = NSLOCTEXT("ShidenNamespace", "FileDeletionWatchWarningTitle", "File Deletion Monitoring Warning");
			const FText WarningMessage = NSLOCTEXT("ShidenNamespace", "FileDeletionWatchWarningMessage",
			                                       "You have enabled file deletion monitoring. Please ensure you are using version control (e.g., Git) or maintaining regular backups.\r\n\r\nDeleted files cannot be recovered automatically, and this could lead to permanent data loss if files are accidentally deleted.");
			FMessageDialog::Open(EAppMsgType::Ok, WarningMessage, WarningTitle);
		}
	}
}
