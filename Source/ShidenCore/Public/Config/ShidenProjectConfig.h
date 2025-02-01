// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Variable/ShidenPredefinedSystemVariable.h"
#include "Command/ShidenPreset.h"
#include "UI/ShidenTextType.h"
#include "UI/ShidenWidget.h"
#include "Variable/ShidenVariableDefinition.h"
#include "ShidenProjectConfig.generated.h"

UCLASS(config = Game, defaultconfig, Category = "Shiden Visual Novel|Config")
class SHIDENCORE_API UShidenProjectConfig : public UObject
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(VisibleAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TMap<FGuid, FString> ScenarioPaths;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = false))
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	FString ScenarioDirectoryPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	FString MacroDirectoryPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TMap<FString, FShidenPreset> Presets;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandDefinitions"), Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TArray<FSoftObjectPath> CommandDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TMap<FString, FShidenTextType> AdditionalTextTypes;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TSubclassOf<UShidenWidget> WidgetClass;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	int32 TargetFrameRate;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TArray<FShidenVariableDefinition> UserVariableDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TArray<FShidenVariableDefinition> SystemVariableDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	bool bAutoSaveOnMobileAppWillDeactivate;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddScenarioPath(const FGuid& ScenarioId, const FString& ScenarioPath);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioPaths(const TMap<FGuid, FString>& Paths);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioDirectoryPath(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetMacroDirectoryPath(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddPreset(const FString& Name, const FShidenPreset& Preset);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void GetPreset(const FString& Name, FShidenPreset& Preset, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void RemovePreset(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetUserVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions);
};
