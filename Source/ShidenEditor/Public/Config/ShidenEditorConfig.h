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

UENUM()
enum class EShidenPreviewPosition : uint8
{
	Left,
	Right
};

UCLASS(config = Editor, defaultconfig, Category = "Shiden Visual Novel|Editor Config")
class SHIDENEDITOR_API UShidenEditorConfig : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
	
	virtual FName GetContainerName() const override;
	
	virtual FName GetCategoryName() const override;
	
	virtual FName GetSectionName() const override;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	
	virtual FText GetSectionDescription() const override;
#endif
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel",
		Meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandRedirector", ConfigRestartRequired = true, ToolTip = "Data assets that define command redirections."))
	TArray<FSoftObjectPath> CommandRedirectors;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel",
		Meta = (ToolTip = "The default command name used when creating new rows in the Scenario Editor."))
	FString DefaultCommand;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel",
		Meta = (ToolTip = "Expand preset arguments when exporting scenarios to CSV or JSON."))
	bool bExpandPresets;

	UPROPERTY(Config, BlueprintReadWrite, Category = "Shiden Visual Novel")
	FShidenPluginVersion PluginVersion;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel|Advanced|Scenario Sync",
		Meta = (ToolTip = "Enable bidirectional synchronization between scenario data assets and external CSV/JSON files. When enabled: (1) External file changes are monitored and automatically imported to update scenario assets. (2) Scenario assets can be exported to external files for external editing. This allows seamless collaboration between Unreal Editor and external tools."))
	bool bEnableScenarioSync;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel|Advanced|Scenario Sync",
		Meta = (ToolTip = "File format for scenario synchronization. Determines the extension (.csv or .json) and format used when exporting scenarios and monitoring external file changes."))
	EShidenScenarioSyncFormat ScenarioSyncFormat;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel|Advanced|Scenario Sync",
		Meta = (ToolTip = "Directory path for scenario file synchronization relative to the project directory (e.g., Saved/OriginalScenarios). The directory structure of scenario assets will be mirrored in this location."))
	FString ScenarioSyncDirectoryPath;

	/**
	 * Sets the default command to use when creating new scenario rows.
	 *
	 * @param CommandName The name of the command to use as default
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetDefaultCommand(const FString& CommandName);

private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
