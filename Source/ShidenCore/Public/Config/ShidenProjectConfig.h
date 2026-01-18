// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Variable/ShidenPredefinedSystemVariable.h"
#include "Command/ShidenPreset.h"
#include "UI/ShidenTextType.h"
#include "UI/ShidenWidget.h"
#include "Variable/ShidenVariableDefinition.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "ShidenProjectConfig.generated.h"

class UShidenUserSaveGame;
class UShidenSystemSaveGame;

UCLASS(config = Game, defaultconfig, Category = "Shiden Visual Novel|Config")
class SHIDENCORE_API UShidenProjectConfig : public UObject
{
	GENERATED_UCLASS_BODY()

private:
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "The master sound class that controls overall volume. This should be the parent of all other sound classes."))
	TSoftObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "The sound class used for background music (BGM) volume control."))
	TSoftObjectPtr<USoundClass> BGMSoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "The sound class used for sound effects (SE) volume control."))
	TSoftObjectPtr<USoundClass> SESoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "The sound class used for voice audio volume control."))
	TSoftObjectPtr<USoundClass> VoiceSoundClass;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "The sound mix asset used for managing audio class volumes at runtime."))
	TSoftObjectPtr<USoundMix> SoundClassMix;

public:
	UPROPERTY(VisibleAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "Mapping of scenario GUIDs to their file paths. This is automatically managed by the system."))
	TMap<FGuid, FString> ScenarioPaths;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (ConfigRestartRequired = false, ToolTip = "Predefined system variables with their initial values."))
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (ConfigRestartRequired = true, ToolTip = "The content browser directory path where scenario data assets are stored (e.g., /Game/Shiden/Scenarios/)."))
	FString ScenarioDirectoryPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (ConfigRestartRequired = true, ToolTip = "The content browser directory path where macro data assets are stored (e.g., /Game/Shiden/Macros/)."))
	FString MacroDirectoryPath;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ConfigRestartRequired = true,
		ToolTip = "Named presets for command arguments. Each preset is associated with a specific command and contains predefined argument values that can be applied when using that command."))
	TMap<FString, FShidenPreset> Presets;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (AllowedClasses = "/Script/ShidenCore.ShidenCommandDefinitions", ConfigRestartRequired = true,
			ToolTip = "Data assets that define available commands for scenarios. Multiple assets can be specified and their commands will be merged together."))
	TArray<FSoftObjectPath> CommandDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "Additional text types beyond the default (e.g., Narration, Thoughts)."))
	TMap<FString, FShidenTextType> AdditionalTextTypes;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ToolTip = "The default widget class."))
	TSubclassOf<UShidenWidget> DefaultWidgetClass;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ToolTip = "Definitions for user variables that persist in save data."))
	TArray<FShidenVariableDefinition> UserVariableDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ToolTip = "Definitions for system variables that persist across all save data."))
	TArray<FShidenVariableDefinition> SystemVariableDefinitions;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config",
		Meta = (ToolTip = "Automatically save game data when the mobile application is about to be suspended or sent to background."))
	bool bAutoSaveOnMobileAppWillDeactivate;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ToolTip = "Custom class for user save game data."))
	TSubclassOf<UShidenUserSaveGame> UserSaveGameClass;

	UPROPERTY(EditAnywhere, GlobalConfig, BlueprintReadWrite, Category = "Shiden Visual Novel|Config", Meta = (ToolTip = "Custom class for system save game data."))
	TSubclassOf<UShidenSystemSaveGame> SystemSaveGameClass;

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

	template<typename T>
	static T* LoadSoftObjectIfNeeded(const TSoftObjectPtr<T>& SoftObject);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
