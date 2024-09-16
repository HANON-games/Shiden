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
	static void SetPreviewSound(const bool bBool);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetShidenDebuggerRefreshInterval(const float Interval);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetEditScenarioPath(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetAutoSaveScenario(const bool bBool);
};
