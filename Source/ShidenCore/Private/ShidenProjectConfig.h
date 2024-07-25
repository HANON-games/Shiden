// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenPredefinedSystemVariable.h"
#include "ShidenPreset.h"
#include "ShidenCommandDefinitions.h"
#include "ShidenProjectConfig.generated.h"

UCLASS(config = Game, defaultconfig, Category = "Shiden Visual Novel|Config")
class SHIDENCORE_API UShidenProjectConfig : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TMap <FGuid, FString> ScenarioPaths;

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

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddScenarioPath(const FGuid& scenarioId, const FString& scenarioPath);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioPaths(const TMap<FGuid, FString>& paths);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioDirectoryPath(const FString& path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetMacroDirectoryPath(const FString& path);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddPreset(const FShidenPreset& preset);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void RemovePreset(const FShidenPreset& preset);
};
