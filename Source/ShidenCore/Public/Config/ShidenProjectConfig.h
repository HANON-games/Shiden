// Copyright (c) 2025 HANON. All Rights Reserved.

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

private:
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config")
	TSoftObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config")
	TSoftObjectPtr<USoundClass> BGMSoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config")
	TSoftObjectPtr<USoundClass> SESoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config")
	TSoftObjectPtr<USoundClass> VoiceSoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config")
	TSoftObjectPtr<USoundMix> SoundClassMix;

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
	TMap<FString, FShidenPreset> Presets;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandDefinitions"),
		Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true))
	TArray<FSoftObjectPath> CommandDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TMap<FString, FShidenTextType> AdditionalTextTypes;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TSubclassOf<UShidenWidget> WidgetClass;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TArray<FShidenVariableDefinition> UserVariableDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	TArray<FShidenVariableDefinition> SystemVariableDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config")
	bool bAutoSaveOnMobileAppWillDeactivate;

	/**
	 * Adds a scenario path mapping for a specific scenario ID.
	 * 
	 * @param ScenarioId The unique identifier for the scenario
	 * @param ScenarioPath The file path to the scenario asset
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddScenarioPath(const FGuid& ScenarioId, const FString& ScenarioPath);

	/**
	 * Sets the complete mapping of scenario IDs to their file paths.
	 * 
	 * @param Paths Map of scenario IDs to their corresponding file paths
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioPaths(const TMap<FGuid, FString>& Paths);

	/**
	 * Sets the directory path where scenario files are located.
	 * 
	 * @param Path The directory path for scenario files
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetScenarioDirectoryPath(const FString& Path);

	/**
	 * Sets the directory path where macro files are located.
	 * 
	 * @param Path The directory path for macro files
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetMacroDirectoryPath(const FString& Path);

	/**
	 * Adds a preset configuration with the specified name.
	 * 
	 * @param Name The name identifier for the preset
	 * @param Preset The preset configuration to add
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void AddPreset(const FString& Name, const FShidenPreset& Preset);

	/**
	 * Attempts to retrieve a preset configuration by name.
	 * 
	 * @param Name The name of the preset to retrieve
	 * @param Preset [out] The retrieved preset configuration
	 * @return True if the preset was found successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config", meta = (DisplayName = "Get Preset"))
	static UPARAM(DisplayName = "Success") bool TryGetPreset(const FString& Name, FShidenPreset& Preset);

	/**
	 * Removes a preset configuration by name.
	 * 
	 * @param Name The name of the preset to remove
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void RemovePreset(const FString& Name);

	/**
	 * Sets the definitions for user-defined variables.
	 * 
	 * @param Definitions Array of variable definitions to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetUserVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions);

	/**
	 * Sets the definitions for system variables.
	 * 
	 * @param Definitions Array of variable definitions to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Config")
	static void SetSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions);

	/**
	 * Gets the master sound class used for volume control.
	 * 
	 * @return The master sound class asset
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Config")
	static USoundClass* GetMasterSoundClass();

	/**
	 * Gets the BGM sound class used for BGM volume control.
	 * 
	 * @return The BGM sound class asset
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Config", DisplayName = "Get BGM Sound Class")
	static USoundClass* GetBGMSoundClass();

	/**
	 * Gets the SE sound class used for SE volume control.
	 * 
	 * @return The SE sound class asset
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Config", DisplayName = "Get SE Sound Class")
	static USoundClass* GetSESoundClass();

	/**
	 * Gets the Voice sound class used for voice volume control.
	 * 
	 * @return The voice sound class asset
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Config")
	static USoundClass* GetVoiceSoundClass();

	/**
	 * Gets the sound mix used for managing audio class volumes.
	 * 
	 * @return The sound mix asset
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Config")
	static USoundMix* GetSoundClassMix();

private:
	static void SaveProjectConfigChanges(TObjectPtr<UShidenProjectConfig> Config);
};
