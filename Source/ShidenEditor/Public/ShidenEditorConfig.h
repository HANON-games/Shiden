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

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetEditScenarioPath(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetPreviewSound(const bool bBool);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetShidenDebuggerRefreshInterval(const float Interval);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetAutoSaveScenario(const bool bBool);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|EditorUtility")
	static void SetDefaultCommand(const FString& CommandName);
};
