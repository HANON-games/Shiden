// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/EditorSettings.h"
#include "Blueprint/UserWidget.h"
#include "ShidenEditorConfig.generated.h"

UCLASS(config = Editor, defaultconfig, Category = "Shiden Visual Novel|Editor Config")
class SHIDENEDITOR_API UShidenEditorConfig : public UObject
{
	GENERATED_UCLASS_BODY()

public: 
	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference")
	FString EditScenarioPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference", Meta = (ConfigRestartRequired = true))
	bool bPreviewSound;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference", Meta = (AllowedClasses = "/Script/UMGEditor.WidgetBlueprint", ExactClass = false, ConfigRestartRequired = true))
	FSoftObjectPath PreviewWidget;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference", Meta = (ConfigRestartRequired = true))
	float ShidenDebuggerRefreshInterval;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor Preference", Meta = (ConfigRestartRequired = true))
	bool bAutoSaveScenario;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetPreviewSound(const bool bBool) {
		TObjectPtr <UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault <UShidenEditorConfig>();
		ShidenEditorConfig->bPreviewSound = bBool;
		ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	}

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetShidenDebuggerRefreshInterval(const float interval) {
		TObjectPtr <UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault <UShidenEditorConfig>();
		ShidenEditorConfig->ShidenDebuggerRefreshInterval = interval;
		ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	}

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetEditScenarioPath(const FString& path) {
		TObjectPtr <UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault <UShidenEditorConfig>();
		ShidenEditorConfig->EditScenarioPath = path;
		ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	}

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetAutoSaveScenario(const bool bBool) {
		TObjectPtr <UShidenEditorConfig> ShidenEditorConfig = GetMutableDefault <UShidenEditorConfig>();
		ShidenEditorConfig->bAutoSaveScenario = bBool;
		ShidenEditorConfig->SaveConfig(CPF_Config, *ShidenEditorConfig->GetDefaultConfigFilename());
	}
};
