// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "System/ShidenSubsystem.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetRegistry/AssetData.h"
#include "System/ShidenLoadingAssetInfo.h"
#include "ShidenScenarioBlueprintLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenScenarioBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Checks if any scenario is currently being played.
	 * 
	 * @return True if a scenario is currently playing
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario")
	static bool IsScenarioPlaying();

	/**
	 * Sets the current index for a specified process.
	 * 
	 * @param ProcessName The name of the process to update
	 * @param CurrentIndex The new index to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void SetCurrentScenarioIndex(const FString& ProcessName, const int32 CurrentIndex);

	/**
	 * Requests cancellation of a specific process.
	 * 
	 * @param ProcessName The name of the process to cancel
	 * @param Reason The reason for cancellation
	 * @param CancelType The type of cancellation to perform
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void RequestCancelScenario(const FString& ProcessName, const FString& Reason, EShidenCancelType CancelType);

	/**
	 * Requests cancellation of all running processes.
	 * 
	 * @param Reason The reason for cancellation
	 * @param CancelType The type of cancellation to perform
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (Reason = ""))
	static void RequestCancelScenarioAll(const FString& Reason, const EShidenCancelType CancelType);

	/**
	 * Checks if a cancel request has been made for a specific scenario process.
	 * 
	 * @param ProcessName The name of the process to check
	 * @param bIsCancelRequested [out] True if cancellation was requested
	 * @param CancelReason [out] The reason provided for cancellation
	 * @param CancelType [out] The type of cancellation requested
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void IsCancelRequested(const FString& ProcessName, bool& bIsCancelRequested, FString& CancelReason, EShidenCancelType& CancelType);

	/**
	 * Marks the current scenario line as read.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void MarkAsRead();

	/**
	 * Checks if the current scenario line has been read.
	 * 
	 * @return True if the current line has been read before
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static bool IsRead();

	/**
	 * Checks if the current command can be skipped.
	 * 
	 * @return True if the command can be skipped
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario")
	static bool CanSkipCommand();

	/**
	 * Registers a scenario property for a specific command.
	 * 
	 * @param CommandName The name of the command
	 * @param Key The property key
	 * @param Value The property value
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario Property")
	static void RegisterScenarioProperty(const FString& CommandName, const FString& Key, const FString& Value);

	/**
	 * Removes a scenario property from a specific command.
	 * 
	 * @param CommandName The name of the command
	 * @param Key The property key to remove
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario Property")
	static void RemoveScenarioProperty(const FString& CommandName, const FString& Key);

	/**
	 * Clears all scenario properties for all commands.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario Property")
	static void ClearAllScenarioProperties();

	/**
	 * Clears all scenario properties for a specific command.
	 * 
	 * @param CommandName The name of the command whose properties should be cleared
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario Property")
	static void ClearScenarioProperties(const FString& CommandName);

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void PopScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress, bool& bIsLastElement);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void PushScenario(const FString& ProcessName, const UShidenScenario* Scenario);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void RemoveScenario(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void PeekScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress, FShidenCancelInfo& CancelInfo,
	                         bool& bIsLastElement, bool& bIsMacro, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void ToNext(const FString& ProcessName, int& NextIndex);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static bool IsEndOfScenario(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void GetScenarioFromCache(const FGuid& ScenarioId, UShidenScenario*& Scenario, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void GetScenarioByIdOrObjectPath(const FString& ScenarioIdOrObjectPath, FGuid& ScenarioId, UShidenScenario*& Scenario, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void ConstructCommand(const FString& ProcessName, const FShidenCommand& OriginalCommand, FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void ConstructCommandForLoad(const FShidenVariable& TempLocalVariables, const FShidenCommand& OriginalCommand, FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetCommandFromCache(UObject* Outer, const FString& ProcessName, const FSoftObjectPath& CommandSoftObjectPath,
	                                UShidenCommandObject*& Command, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void LoadScenarioAssetPaths(UObject* CallerObject, const UShidenScenario* Scenario, TArray<FShidenLoadingAssetInfo>& AssetInfo,
	                                   bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void LoadScenarioAssets(UObject* Outer, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void InitFromSaveData(UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
	                             bool& bSuccess, FString& ErrorMessage);
};
