// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenEditorConfig.h"
#include "ShidenEditorUserConfig.generated.h"

UCLASS(config = EditorPerProjectUserSettings, Category = "Shiden Visual Novel|Editor User Config")
class SHIDENEDITOR_API UShidenEditorUserConfig : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
	virtual FName GetContainerName() const override;

	virtual FName GetCategoryName() const override;

	virtual FName GetSectionName() const override;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;

	virtual FText GetSectionDescription() const override;
#endif

	UPROPERTY(Config, BlueprintReadWrite, Category = "Shiden Visual Novel")
	FString ScenarioFilterPath;

	UPROPERTY(Config, BlueprintReadWrite, Category = "Shiden Visual Novel")
	FString EditScenarioPath;

	UPROPERTY(Config, BlueprintReadWrite, Category = "Shiden Visual Novel")
	bool bPreviewSound;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel", Meta = (ToolTip = "The size of the preview window in pixels (Width, Height)."))
	FVector2D PreviewSize;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel",
		Meta = (ToolTip = "When enabled, the scenario editor and preview window are combined into a single window. When disabled, they are displayed in separate windows."))
	bool bCombineEditorWindows;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel",
		Meta = (EditCondition = "bCombineEditorWindows", ToolTip = "The position of the preview window when editor windows are combined. Left places the preview on the left side, Right places it on the right side."))
	EShidenPreviewPosition PreviewPosition;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel",
		Meta = (ToolTip = "The refresh interval in seconds for the Shiden Debugger window. Lower values provide more responsive updates but may impact editor performance."))
	float ShidenDebuggerRefreshInterval;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel", Meta = (ToolTip = "Automatically save scenario data assets when changes are made in the Scenario Editor."))
	bool bAutoSaveScenario;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel", Meta = (ToolTip = "Enable Advanced Mode to show additional editor inputs."))
	bool bUseAdvancedMode;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Shiden Visual Novel", Meta = (ToolTip = "Show all command arguments regardless of their VisibilityCondition settings."))
	bool bForceShowAllCommandArguments;

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

private:
	template <class T, class TMember>
	static void UpdateConfig(TMember UShidenEditorUserConfig::* MemberPtr, const T& Value);
};
