// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Variable/ShidenPredefinedSystemVariable.h"
#include "Command/ShidenPreset.h"
#include "UI/ShidenTextType.h"
#include "UI/ShidenWidget.h"
#include "ShidenProjectConfig.generated.h"

UCLASS(config = Game, defaultconfig, Category = "Shiden Visual Novel|Config")
class SHIDENCORE_API UShidenProjectConfig : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TMap<FGuid, FString> ScenarioPaths;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = false))
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	FString ScenarioDirectoryPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	FString MacroDirectoryPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TArray<FShidenPreset> Presets;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandDefinitions"), Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TArray<FSoftObjectPath> CommandDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TMap<FString, FShidenTextType> AdditionalTextTypes;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TSoftClassPtr<UShidenWidget> WidgetClass;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	int32 TargetFrameRate;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	bool bAutoSaveOnMobileAppWillDeactivate;
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddScenarioPath(const FGuid& ScenarioId, const FString ScenarioPath);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioPaths(const TMap<FGuid, FString>& Paths);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioDirectoryPath(const FString Path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetMacroDirectoryPath(const FString Path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddPreset(const FShidenPreset& Preset);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void RemovePreset(const FShidenPreset& Preset);
};
