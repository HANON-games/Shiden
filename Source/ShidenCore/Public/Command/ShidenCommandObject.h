// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Scenario/ShidenManagerInterface.h"
#include "UI/ShidenWidget.h"
#include "Command/ShidenCommand.h"
#include "Scenario/ShidenScenarioProperty.h"
#include "ShidenCommandObject.generated.h"

UENUM(BlueprintType)
enum class EShidenInitFromSaveDataStatus : uint8
{
	Complete = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenPreProcessStatus : uint8
{
	Complete = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenPreviewStatus : uint8
{
	Complete = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenProcessStatus : uint8
{
	Next = 0,
	Break,
	Complete,
	Error,
	DelayUntilNextTick
};

UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class SHIDENCORE_API UShidenCommandObject : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Restores command state from saved scenario properties when loading a save game.
	 * @param ScenarioProperties Map of saved scenario properties to restore from
	 * @param ShidenWidget The Shiden widget instance
	 * @param ShidenManager The Shiden manager interface
	 * @param CallerObject The object that initiated this restore operation
	 * @param Status [out] Status of the restore operation
	 * @param ErrorMessage [out] Error details if the operation fails
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void RestoreFromSaveData(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                         UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
	                                                EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	/**
	 * Previews a command in editor.
	 * @param Command The command to preview
	 * @param ShidenWidget The Shiden widget instance
	 * @param ShidenManager The Shiden manager interface
	 * @param bIsCurrentCommand True if this is the currently selected command in the editor
	 * @param Status [out] Status of the preview operation
	 * @param ErrorMessage [out] Error details if the operation fails
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void PreviewCommand(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                    const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                    const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage);

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage);

	/**
	 * Performs pre-processing setup before executing a command, such as loading assets or validating parameters.
	 * @param ProcessName The name of the process being executed (defaults to "Default")
	 * @param Command The command to pre-process
	 * @param ShidenWidget The Shiden widget instance
	 * @param ShidenManager The Shiden manager interface
	 * @param CallerObject The object that initiated this pre-process operation
	 * @param Status [out] Status of the pre-process operation
	 * @param ErrorMessage [out] Error details if the operation fails
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command", meta = (ProcessName = "Default"))
	void PreProcessCommand(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                       const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                       UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage);

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
	                                              EShidenPreProcessStatus& Status, FString& ErrorMessage);

	/**
	 * Executes the main processing logic for a command during each frame update.
	 * @param ProcessName The name of the process being executed (defaults to "Default")
	 * @param Command The command to process
	 * @param ShidenWidget The Shiden widget instance
	 * @param ShidenManager The Shiden manager interface
	 * @param DeltaTime Time elapsed since the last frame in seconds
	 * @param CallerObject The object that initiated this process operation
	 * @param Status [out] Processing status (Next, Break, Complete, Error, DelayUntilNextTick)
	 * @param BreakReason [out] Reason if processing breaks
	 * @param NextScenarioName [out] Next scenario to load if applicable
	 * @param ErrorMessage [out] Error details if the operation fails
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command", meta = (ProcessName = "Default"))
	void ProcessCommand(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                    const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime, UObject* CallerObject,
	                    EShidenProcessStatus& Status, FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage);

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
	                                           UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage);
};
