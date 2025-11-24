// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenPlayScenarioStatus.h"
#include "UObject/Interface.h"
#include "UI/ShidenWidget.h"
#include "Scenario/ShidenManagerInterface.h"
#include "ShidenHookInterface.generated.h"

class UShidenUserSaveGame;
class UShidenSystemSaveGame;

UINTERFACE(MinimalAPI, Blueprintable)
class UShidenHookInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenHookInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called before a scenario starts playing.
	 * @param ShidenWidget The Shiden widget
	 * @param ShidenManager The Shiden manager interface
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPrePlayScenario(const UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	/**
	 * Called after a scenario finishes playing.
	 * @param Status The status of scenario completion
	 * @param NextScenarioName The name of the next scenario to play, if any
	 * @param BreakReason The reason for breaking scenario execution
	 * @param ErrorMessage Any error message that occurred during execution
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostPlayScenario(const EShidenPlayScenarioStatus Status, const FString& NextScenarioName,
	                        const FString& BreakReason, const FString& ErrorMessage);

	/**
	 * Called before pre-processing a command.
	 * @param ProcessName The name of the process executing the command
	 * @param Command The command to be pre-processed
	 * @param ShidenWidget The Shiden widget
	 * @param ShidenManager The Shiden manager interface
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPrePreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                            const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	/**
	 * Called after pre-processing a command.
	 * @param ProcessName The name of the process executing the command
	 * @param Command The command that was pre-processed
	 * @param ShidenWidget The Shiden widget
	 * @param ShidenManager The Shiden manager interface
	 * @param Status The status of pre-processing
	 * @param ErrorMessage Any error message that occurred during pre-processing
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostPreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                             const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                             const EShidenPreProcessStatus Status, const FString& ErrorMessage);

	/**
	 * Called before processing a command.
	 * @param ProcessName The name of the process executing the command
	 * @param Command The command to be processed
	 * @param ShidenWidget The Shiden widget
	 * @param ShidenManager The Shiden manager interface
	 * @param DeltaTime Time elapsed since the last frame in seconds
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                         const float DeltaTime);

	/**
	 * Called after processing a command.
	 * @param ProcessName The name of the process executing the command
	 * @param Command The command that was processed
	 * @param ShidenWidget The Shiden widget
	 * @param ShidenManager The Shiden manager interface
	 * @param DeltaTime Time elapsed since the last frame in seconds
	 * @param Status The status of command processing
	 * @param NextScenarioName The name of the next scenario to play, if any
	 * @param BreakReason The reason for breaking command execution
	 * @param ErrorMessage Any error message that occurred during processing
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                          const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                          const float DeltaTime, const EShidenProcessStatus Status, const FString& NextScenarioName,
	                          const FString& BreakReason, const FString& ErrorMessage);

	/**
	 * Called after user data is retrieved from a save slot.
	 * @param SlotName The name of the save slot that was retrieved from
	 * @param UserSaveGame The retrieved user save game instance, or nullptr if retrieval failed
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnUserDataRetrieved(const FString& SlotName, UShidenUserSaveGame* UserSaveGame);

	/**
	 * Called after system data is retrieved.
	 * @param SystemSaveGame The retrieved system save game instance, or nullptr if retrieval failed
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnSystemDataRetrieved(UShidenSystemSaveGame* SystemSaveGame);
};
