// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenPluginVersion.h"
#include "ShidenEditorConfig.generated.h"

UCLASS(config = Editor, defaultconfig, Category = "Shiden Visual Novel|Editor Config")
class SHIDENEDITOR_API UShidenEditorConfig : public UObject
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandRedirector"),
		Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TArray<FSoftObjectPath> CommandRedirectors;

	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FString EditScenarioPath;

	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	bool bPreviewSound;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	float ShidenDebuggerRefreshInterval;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference",
		Meta = (ConfigRestartRequired = true))
	bool bAutoSaveScenario;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FString DefaultCommand;

	UPROPERTY(GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FShidenPluginVersion PluginVersion;

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
	static void UpdateConfig(TObjectPtr<UShidenEditorConfig> Config);
};
