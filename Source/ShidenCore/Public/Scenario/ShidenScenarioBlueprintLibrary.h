// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenScenarioProperty.h"
#include "System/ShidenSubsystem.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetRegistry/AssetData.h"
#include "System/ShidenLoadingAssetInfo.h"
#include "Serialization/JsonSerializer.h"
#include "ShidenScenarioBlueprintLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenScenarioBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Converts a scenario property to an array of strings.
	 * 
	 * @param Property The scenario property to convert
	 * @param Result [out] The resulting array of strings
	 * @return True if the conversion was successful, false otherwise
	 */
	UFUNCTION(BlueprintPure, meta=(BlueprintAutocast, CompactNodeTitle = "->", DisplayName = "Convert to String Array"), Category = "Shiden Visual Novel|Scenario")
	static bool TryConvertToStringArray(const FShidenScenarioProperty& Property, TArray<FString>& Result);

	/**
	 * Converts a scenario property to a map of strings.
	 * 
	 * @param Property The scenario property to convert
	 * @param Result [out] The resulting map of strings
	 * @return True if the conversion was successful, false otherwise
	 */
	UFUNCTION(BlueprintPure, meta=(BlueprintAutocast, CompactNodeTitle = "->", DisplayName = "Convert to String Map"), Category = "Shiden Visual Novel|Scenario")
	static bool TryConvertToStringMap(const FShidenScenarioProperty& Property, TMap<FString, FString>& Result);

	/**
	 * Converts a scenario property to a string representation.
	 * 
	 * @param Property The scenario property to convert
	 * @return The string representation of the property
	 */
	UFUNCTION(BlueprintPure, meta=(BlueprintAutocast, CompactNodeTitle = "->"), Category = "Shiden Visual Novel|Scenario")
	static FString GetValueAsString(const FShidenScenarioProperty& Property);

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
	 * @param CancelReason The reason for cancellation
	 * @param CancelType The type of cancellation to perform
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void RequestCancelScenario(const FString& ProcessName, const FString& CancelReason, const EShidenCancelType CancelType);

	/**
	 * Requests cancellation of all running processes.
	 * 
	 * @param CancelReason The reason for cancellation
	 * @param CancelType The type of cancellation to perform
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (Reason = ""))
	static void RequestCancelScenarioAll(const FString& CancelReason, const EShidenCancelType CancelType);

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
	 * Finds a scenario property for a specific command.
	 *
	 * @param CommandName The name of the command to check
	 * @param Key The property key to check
	 * @param Property [out] The value associated with the property key
	 * @return True if the property was found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario Property", meta = (DisplayName = "Find Scenario Property"))
	static UPARAM(DisplayName = "Success") bool TryFindScenarioProperty(const FString& CommandName, const FString& Key, FShidenScenarioProperty& Property);

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
	 * Registers a scenario property for a specific command from an array of values.
	 * 
	 * @param CommandName The name of the command
	 * @param Key The property key
	 * @param Values The array of values to register
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario Property")
	static void RegisterScenarioPropertyFromArray(const FString& CommandName, const FString& Key, const TArray<FString>& Values);

	/**
	 * Registers a scenario property for a specific command from a map of values.
	 * 
	 * @param CommandName The name of the command
	 * @param Key The property key
	 * @param Values The map of values to register
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario Property")
	static void RegisterScenarioPropertyFromMap(const FString& CommandName, const FString& Key, const TMap<FString, FString>& Values);
	
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

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default", DisplayName = "Peek Scenario"))
	static UPARAM(DisplayName = "Success") bool TryPeekScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress, FShidenCancelInfo& CancelInfo,
	                                                            bool& bIsLastElement, bool& bIsMacro);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void ToNext(const FString& ProcessName, int32& NextIndex);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static bool IsEndOfScenario(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (DisplayName = "Get Scenario"))
	static UPARAM(DisplayName = "Success") bool TryGetScenario(const FGuid& ScenarioId, UShidenScenario*& Scenario);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (DisplayName = "Get Scenario by Id or Object Path"))
	static UPARAM(DisplayName = "Success") bool TryGetScenarioByIdOrObjectPath(const FString& ScenarioIdOrObjectPath, FGuid& ScenarioId, UShidenScenario*& Scenario);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void ConstructCommand(const FString& ProcessName, const FShidenCommand& OriginalCommand, FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void ConstructCommandForLoad(const FShidenVariable& TempLocalVariables, const FShidenCommand& OriginalCommand, FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility", meta = (DisplayName = "Get Command"))
	static UPARAM(DisplayName = "Success") bool TryGetCommand(UObject* Outer, const FString& ProcessName, const FSoftObjectPath& CommandSoftObjectPath,
	                                                          UShidenCommandObject*& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (DisplayName = "Load Scenario Asset Paths"))
	static UPARAM(DisplayName = "Success") bool TryLoadScenarioAssetPaths(UObject* CallerObject, const UShidenScenario* Scenario, TArray<FShidenLoadingAssetInfo>& AssetInfo,
	                                                                      FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (DisplayName = "Load Scenario Assets"))
	static UPARAM(DisplayName = "Success") bool TryLoadScenarioAssets(UObject* Outer, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (DisplayName = "Init from Save Data"))
	static UPARAM(DisplayName = "Success") bool TryInitFromSaveData(UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
		                                                            UObject* CallerObject, FString& ErrorMessage);
};
