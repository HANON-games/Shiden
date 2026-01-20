// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/ShidenPluginVersion.h"
#include "ShidenEditorConfig.generated.h"

UENUM()
enum class EShidenScenarioSyncFormat : uint8
{
	CSV,
	JSON
};

UCLASS(config = Editor, defaultconfig, Category = "Shiden Visual Novel|Editor Config")
class SHIDENEDITOR_API UShidenEditorConfig : public UObject
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandRedirector", ConfigRestartRequired = true, ToolTip = "Data assets that define command redirections."))
	TArray<FSoftObjectPath> CommandRedirectors;

	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FString ScenarioFilterPath;

	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FString EditScenarioPath;

	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	bool bPreviewSound;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ToolTip = "The size of the preview window in pixels (Width, Height)."))
	FVector2D PreviewSize;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ToolTip = "The refresh interval in seconds for the Shiden Debugger window. Lower values provide more responsive updates but may impact editor performance."))
	float ShidenDebuggerRefreshInterval;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ConfigRestartRequired = true, ToolTip = "Automatically save scenario data assets when changes are made in the Scenario Editor."))
	bool bAutoSaveScenario;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ToolTip = "The default command name used when creating new rows in the Scenario Editor."))
	FString DefaultCommand;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ToolTip = "Enable Advanced Mode to show additional editor inputs."))
	bool bUseAdvancedMode;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ToolTip = "Show all command arguments regardless of their VisibilityCondition settings."))
	bool bShowAllVisibilityConditionItems;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ToolTip = "Expand preset arguments when exporting scenarios to CSV or JSON."))
	bool bExpandPresets;
	
	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FShidenPluginVersion PluginVersion;
	
	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Advanced|Scenario Sync" /*EditAnywhere, GlobalConfig, BlueprintReadWrite,
		Meta = (ToolTip = "Enable automatic synchronization between scenario data assets and CSV/JSON files. When enabled, all scenarios will be exported to the sync directory.")*/)
	bool bEnableScenarioSync;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Advanced|Scenario Sync" /*EditAnywhere, GlobalConfig, BlueprintReadWrite,
		Meta = (ToolTip = "Choose the file format for scenario synchronization (CSV or JSON).")*/)
	EShidenScenarioSyncFormat ScenarioSyncFormat;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Advanced|Scenario Sync" /*EditAnywhere, GlobalConfig, BlueprintReadWrite,
		Meta = (ToolTip = "The directory path where scenario files will be synced. Can be an absolute path (e.g., C:/MyProject/Scenarios or /home/user/scenarios) or a relative path. The directory structure of scenario assets will be preserved in this location.")*/)
	FDirectoryPath ScenarioSyncDirectoryPath;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Advanced|Scenario Sync" /*EditAnywhere, GlobalConfig, BlueprintReadWrite,
		Meta = (ToolTip = "Enable monitoring of new file additions in the sync directory. When a new CSV/JSON file is added, it will be automatically imported if a matching scenario GUID is found.")*/)
	bool bWatchFileAdditions;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Advanced|Scenario Sync" /*EditAnywhere, GlobalConfig, BlueprintReadWrite,
		Meta = (ToolTip = "Enable monitoring of file deletions in the sync directory. WARNING: It is strongly recommended to use version control (e.g., Git) or maintain regular backups before enabling this feature. Deleted files cannot be recovered automatically.")*/)
	bool bWatchFileDeletions;

	/**
	 * Sets the scenario filter path.
	 * 
	 * @param Path The new scenario filter path
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetScenarioFilterPath(const FString& Path);

	/**
	 * Sets the path of the scenario currently being edited.
	 * 
	 * @param PackageName The file path to the scenario asset
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetEditScenarioPath(const FString& PackageName);

	/**
	 * Enables or disables sound preview functionality in the editor.
	 *
	 * @param bEnabled True to enable sound preview, false to disable
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetPreviewSound(const bool bEnabled);

	/**
	 * Sets the size of the preview window.
	 *
	 * @param Size The preview window size in pixels (Width, Height)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetPreviewSize(const FVector2D& Size);

	/**
	 * Sets the refresh interval for the Shiden debugger.
	 *
	 * @param Interval The refresh interval in seconds
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetShidenDebuggerRefreshInterval(const float Interval);

	/**
	 * Enables or disables automatic saving of scenarios when changes are made.
	 * 
	 * @param bEnabled True to enable auto-save, false to disable
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetAutoSaveScenario(const bool bEnabled);

	/**
	 * Sets the default command to use when creating new scenario rows.
	 *
	 * @param CommandName The name of the command to use as default
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetDefaultCommand(const FString& CommandName);

private:
	template <class T, class TMember>
	static void UpdateConfig(TMember UShidenEditorConfig::* MemberPtr, const T& Value);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
