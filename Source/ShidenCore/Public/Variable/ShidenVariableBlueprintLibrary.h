// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Variable/ShidenVariableType.h"
#include "Variable/ShidenVariableKind.h"
#include "Internationalization/Regex.h"
#include "Engine/Engine.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"
#include "ShidenVariableBlueprintLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenVariableBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Updates a boolean user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param bValue The new boolean value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (DisplayName = "Update User Boolean"))
	static UPARAM(DisplayName = "Success") bool TryUpdateUserBoolean(const FString& Name, const bool bValue, FString& ErrorMessage);

	/**
	 * Updates an integer user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new integer value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (DisplayName = "Update User Integer"))
	static UPARAM(DisplayName = "Success") bool TryUpdateUserInteger(const FString& Name, const int32 Value, FString& ErrorMessage);

	/**
	 * Updates a float user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new float value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (DisplayName = "Update User Float"))
	static UPARAM(DisplayName = "Success") bool TryUpdateUserFloat(const FString& Name, const float Value, FString& ErrorMessage);

	/**
	 * Updates a string user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new string value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value", DisplayName = "Update User String"))
	static UPARAM(DisplayName = "Success") bool TryUpdateUserString(const FString& Name, const FString& Value, FString& ErrorMessage);

	/**
	 * Updates a Vector2D user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector2D value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value", DisplayName = "Update User Vector2"))
	static UPARAM(DisplayName = "Success") bool TryUpdateUserVector2(const FString& Name, const FVector2D& Value, FString& ErrorMessage);

	/**
	 * Updates a Vector3 user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector3 value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value", DisplayName = "Update User Vector3"))
	static UPARAM(DisplayName = "Success") bool TryUpdateUserVector3(const FString& Name, const FVector& Value, FString& ErrorMessage);

	/**
	 * Gets all user variable names.
	 * 
	 * @param Names [out] Array of user variable names
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void GetUserVariableNames(TArray<FString>& Names);

	/**
	 * Gets the number of user variables.
	 * 
	 * @return Number of user variables
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static int32 UserVariableNum();

	/**
	 * Finds a user variable definition by name.
	 * 
	 * @param Name The name of the variable
	 * @param Definition [out] The variable definition
	 * @return True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (DisplayName = "Find User Variable Definition"))
	static UPARAM(DisplayName = "Success") bool TryFindUserVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition);

	/**
	 * Resets a user variable to its default value.
	 * 
	 * @param Name The name of the variable to reset
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void ResetUserVariable(const FString& Name);

	/**
	 * Finds a user variable and gets its value.
	 * 
	 * @param Name The name of the variable to find
	 * @param VariableType [out] The type of the variable
	 * @param bBooleanValue [out] The boolean value (if type is boolean)
	 * @param StringValue [out] The string value (if type is string)
	 * @param IntegerValue [out] The integer value (if type is integer)
	 * @param FloatValue [out] The float value (if type is float)
	 * @param Vector2Value [out] The Vector2D value (if type is vector2)
	 * @param Vector3Value [out] The Vector3 value (if type is vector3)
	 * @param ErrorMessage [out] Error message if the operation failed
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (DisplayName = "Find User Variable"))
	static UPARAM(DisplayName = "Success") bool TryFindUserVariable(const FString& Name, EShidenVariableType& VariableType, bool& bBooleanValue,
	                                                                FString& StringValue, int32& IntegerValue, float& FloatValue,
	                                                                FVector2D& Vector2Value, FVector& Vector3Value, FString& ErrorMessage);

	/**
	 * Resets all user variables to their default values.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void ResetUserVariables();

	/**
	 * Finds a user variable and converts its value to a string.
	 * 
	 * @param Name The name of the variable to find
	 * @param OriginalType [out] The original type of the variable
	 * @param Result [out] The variable value as a string
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (DisplayName = "Find User Variable as String"))
	static UPARAM(DisplayName = "Success") bool TryFindUserVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result);

	/**
	 * Updates a boolean system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param bValue The new boolean value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (DisplayName = "Update System Boolean"))
	static UPARAM(DisplayName = "Success") bool TryUpdateSystemBoolean(const FString& Name, const bool bValue, FString& ErrorMessage);

	/**
	 * Updates an integer system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new integer value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (DisplayName = "Update System Integer"))
	static UPARAM(DisplayName = "Success") bool TryUpdateSystemInteger(const FString& Name, const int32 Value, FString& ErrorMessage);

	/**
	 * Updates a float system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new float value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (DisplayName = "Update System Float"))
	static UPARAM(DisplayName = "Success") bool TryUpdateSystemFloat(const FString& Name, const float Value, FString& ErrorMessage);

	/**
	 * Updates a string system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new string value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value", DisplayName = "Update System String"))
	static UPARAM(DisplayName = "Success") bool TryUpdateSystemString(const FString& Name, const FString& Value, FString& ErrorMessage);

	/**
	 * Updates a Vector2D system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector2D value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value", DisplayName = "Update System Vector2"))
	static UPARAM(DisplayName = "Success") bool TryUpdateSystemVector2(const FString& Name, const FVector2D& Value, FString& ErrorMessage);

	/**
	 * Updates a Vector3 system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector3 value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value", DisplayName = "Update System Vector3"))
	static UPARAM(DisplayName = "Success") bool TryUpdateSystemVector3(const FString& Name, const FVector& Value, FString& ErrorMessage);

	/**
	 * Finds a system variable definition by name.
	 * 
	 * @param Name The name of the variable
	 * @param Definition [out] The variable definition
	 * @return True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (DisplayName = "Find System Variable Definition"))
	static UPARAM(DisplayName = "Success") bool TryFindSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition);

	/**
	 * Resets a system variable to its default value.
	 * 
	 * @param Name The name of the variable to reset
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void ResetSystemVariable(const FString& Name);

	/**
	 * Gets all system variable names.
	 * 
	 * @param Names [out] Array of system variable names
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void GetSystemVariableNames(TArray<FString>& Names);

	/**
	 * Gets the number of system variables.
	 * 
	 * @return Number of system variables
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static int32 SystemVariableNum();

	/**
	 * Finds a system variable and gets its value.
	 * 
	 * @param Name The name of the variable to find
	 * @param VariableType [out] The type of the variable
	 * @param bBooleanValue [out] The boolean value (if type is boolean)
	 * @param StringValue [out] The string value (if type is string)
	 * @param IntegerValue [out] The integer value (if type is integer)
	 * @param FloatValue [out] The float value (if type is float)
	 * @param Vector2Value [out] The Vector2D value (if type is vector2)
	 * @param Vector3Value [out] The Vector3 value (if type is vector3)
	 * @param ErrorMessage [out] Error message if the operation failed
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (DisplayName = "Find System Variable"))
	static UPARAM(DisplayName = "Success") bool TryFindSystemVariable(const FString& Name, EShidenVariableType& VariableType,
	                                                                  bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
	                                                                  float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, FString& ErrorMessage);

	/**
	 * Resets all system variables to their default values.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void ResetSystemVariables();

	/**
	 * Finds a system variable and converts its value to a string.
	 * 
	 * @param Name The name of the variable to find
	 * @param OriginalType [out] The original type of the variable
	 * @param Result [out] The variable value as a string
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (DisplayName = "Find System Variable as String"))
	static UPARAM(DisplayName = "Success") bool TryFindSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result);

	/**
	 * Initializes local variables for a specific process from a scenario.
	 * 
	 * @param ProcessName The name of the process
	 * @param Scenario The scenario containing variable definitions
	 * @param Arguments Initial values for the variables
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Arguments"))
	static void InitLocalVariable(const FString& ProcessName, const UShidenScenario* Scenario, const TMap<FString, FString>& Arguments);

	/**
	 * Updates a boolean local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param bValue The new boolean value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", DisplayName = "Update Local Boolean"))
	static UPARAM(DisplayName = "Success") bool TryUpdateLocalBoolean(const FString& ProcessName, const FString& Name, const bool bValue, FString& ErrorMessage);

	/**
	 * Updates an integer local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new integer value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", DisplayName = "Update Local Integer"))
	static UPARAM(DisplayName = "Success") bool TryUpdateLocalInteger(const FString& ProcessName, const FString& Name, const int32 Value, FString& ErrorMessage);

	/**
	 * Updates a float local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new float value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", DisplayName = "Update Local Float"))
	static UPARAM(DisplayName = "Success") bool TryUpdateLocalFloat(const FString& ProcessName, const FString& Name, const float Value, FString& ErrorMessage);

	/**
	 * Updates a string local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new string value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Value", DisplayName = "Update Local String"))
	static UPARAM(DisplayName = "Success") bool TryUpdateLocalString(const FString& ProcessName, const FString& Name, const FString& Value, FString& ErrorMessage);

	/**
	 * Updates a Vector2D local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new Vector2D value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Value", DisplayName = "Update Local Vector2"))
	static UPARAM(DisplayName = "Success") bool TryUpdateLocalVector2(const FString& ProcessName, const FString& Name, const FVector2D& Value, FString& ErrorMessage);

	/**
	 * Updates a Vector3 local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new Vector3 value
	 * @param ErrorMessage [out] Error message if the update failed
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Value", DisplayName = "Update Local Vector3"))
	static UPARAM(DisplayName = "Success") bool TryUpdateLocalVector3(const FString& ProcessName, const FString& Name, const FVector& Value, FString& ErrorMessage);

	/**
	 * Finds a local variable definition by name.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable
	 * @param Definition [out] The variable definition
	 * @return True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", DisplayName = "Find Local Variable Definition"))
	static UPARAM(DisplayName = "Success") bool TryFindLocalVariableDefinition(const FString& ProcessName, const FString& Name, FShidenVariableDefinition& Definition);

	/**
	 * Finds a local variable and gets its value.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to find
	 * @param VariableType [out] The type of the variable
	 * @param bBooleanValue [out] The boolean value (if type is boolean)
	 * @param StringValue [out] The string value (if type is string)
	 * @param IntegerValue [out] The integer value (if type is integer)
	 * @param FloatValue [out] The float value (if type is float)
	 * @param Vector2Value [out] The Vector2D value (if type is vector2)
	 * @param Vector3Value [out] The Vector3 value (if type is vector3)
	 * @param ErrorMessage [out] Error message if the operation failed
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", DisplayName = "Find Local Variable"))
	static UPARAM(DisplayName = "Success") bool TryFindLocalVariable(const FString& ProcessName, const FString& Name,
	                                                                 EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
	                                                                 float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, FString& ErrorMessage);

	/**
	 * Finds a local variable and converts its value to a string.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to find
	 * @param OriginalType [out] The original type of the variable
	 * @param Result [out] The variable value as a string
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", DisplayName = "Find Local Variable as String"))
	static UPARAM(DisplayName = "Success") bool TryFindLocalVariableAsString(const FString& ProcessName, const FString& Name,
		                                                                     EShidenVariableType& OriginalType, FString& Result);

	/**
	 * Resets a local variable to its default value.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to reset
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void ResetLocalVariable(const FString& ProcessName, const FString& Name);

	/**
	 * Removes all local variables in the current scope of the specified process.
	 * 
	 * @param ProcessName The name of the process
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveLocalVariablesInCurrentScope(const FString& ProcessName);

	/**
	 * Removes all local variables in the specified process.
	 * 
	 * @param ProcessName The name of the process
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveLocalVariablesInProcess(const FString& ProcessName);

	/**
	 * Resets all local variables in all processes to their default values.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables")
	static void ResetAllLocalVariables();

	/**
	 * Updates a predefined system variable using a string value.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param Name The name of the variable to update
	 * @param Value The new value as a string
	 * @return True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables",
		meta = (WorldContext="WorldContextObject", DisplayName = "Update Predefined System Variable by String"))
	static UPARAM(DisplayName = "Success") bool TryUpdatePredefinedSystemVariableByString(const UObject* WorldContextObject, const FString& Name, const FString& Value);

	/**
	 * Finds a predefined system variable and converts its value to a string.
	 * 
	 * @param Name The name of the variable to find
	 * @param OriginalType [out] The original type of the variable
	 * @param Result [out] The variable value as a string
	 * @return True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (DisplayName = "Find Predefined System Variable as String"))
	static UPARAM(DisplayName = "Success") bool TryFindPredefinedSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result);

	/**
	 * Sets the master volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param Value The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetMasterVolume(const UObject* WorldContextObject, float Value);

	/**
	 * Sets the BGM volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param Value The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"),
		DisplayName = "Set BGM Volume")
	static void SetBGMVolume(const UObject* WorldContextObject, float Value);

	/**
	 * Sets the SE (Sound Effect) volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param Value The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"),
		DisplayName = "Set SE Volume")
	static void SetSEVolume(const UObject* WorldContextObject, float Value);

	/**
	 * Sets the voice volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param Value The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetVoiceVolume(const UObject* WorldContextObject, float Value);

	/**
	 * Initializes all predefined system variables with their default values.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void ResetPredefinedSystemVariables(const UObject* WorldContextObject);

	/**
	 * Gets all predefined system variables.
	 * 
	 * @return The predefined system variable structure
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables|Predefined System Variables")
	static FShidenPredefinedSystemVariable GetPredefinedSystemVariable();

	/**
	 * Finds a predefined system variable definition by name.
	 * 
	 * @param Name The name of the variable
	 * @param Definition [out] The variable definition
	 * @return True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (DisplayName = "Find Predefined System Variable Definition"))
	static UPARAM(DisplayName = "Success") bool TryFindPredefinedSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition);

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables")
	static TArray<FString> GetVariableNamesFromText(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables")
	static FShidenCommand ReplaceVariablesForLoad(FShidenVariable TempLocalVariable, const FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static FShidenCommand ReplaceAllVariable(const FString& ProcessName, const FShidenCommand& Command);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables", meta = (DisplayName = "Convert to Variable Kind"))
	static UPARAM(DisplayName = "Success") bool TryConvertToVariableKind(const FString& VariableKind, EShidenVariableKind& Result);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables", meta = (DisplayName = "Convert to Variable Type"))
	static UPARAM(DisplayName = "Success") bool TryConvertToVariableType(const FString& VariableType, EShidenVariableType& Result);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables", meta = (DisplayName = "Convert to Asset Path Type"))
	static UPARAM(DisplayName = "Success") bool TryConvertToAssetPathType(const FString& AssetPathType, EShidenAssetPathType& Result);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default", DisplayName = "Find Variable Definition"))
	static UPARAM(DisplayName = "Success") bool TryFindVariableDefinition(const FString& ProcessName, EShidenVariableKind Kind, const FString& Name,
	                                                                      FShidenVariableDefinition& Definition, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "StringValue,Vector2Value,Vector3Value",
				WorldContext="WorldContextObject", DisplayName = "Update Variable"))
	static UPARAM(DisplayName = "Success") bool TryUpdateVariable(const UObject* WorldContextObject, const FString& ProcessName, EShidenVariableKind Kind,
	                                                              EShidenVariableType Type, const FString& Name, bool bBooleanValue, const FString& StringValue,
	                                                              int32 IntegerValue, float FloatValue, const FVector2D& Vector2Value, const FVector& Vector3Value, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default", DisplayName = "Find Variable"))
	static UPARAM(DisplayName = "Success") bool TryFindVariable(const FString& ProcessName, EShidenVariableKind Kind, const FString& Name,
		                                                        EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue,
		                                                        int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value,
		                                                        FVector& Vector3Value, FString& ErrorMessage);

	static bool TryEvaluateBoolean(const FString& Operator, const bool bABooleanValue, const bool bBBooleanValue, bool& bResult, FString& ErrorMessage);

	static bool TryEvaluateString(const FString& Operator, const FString& AStringValue, const FString& BStringValue, bool& bResult, FString& ErrorMessage);

	static bool TryEvaluateInteger(const FString& Operator, const int32 AIntegerValue, const int32 BIntegerValue, bool& bResult, FString& ErrorMessage);

	static bool TryEvaluateFloat(const FString& Operator, const float AFloatValue, const float BFloatValue, bool& bResult, FString& ErrorMessage);

	static bool TryEvaluateVector2(const FString& Operator, const FVector2D& AVector2Value, const FVector2D& BVector2Value, bool& bResult, FString& ErrorMessage);

	static bool TryEvaluateVector3(const FString& Operator, const FVector& AVector3Value, const FVector& BVector3Value, bool& bResult, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables",
		meta = (AutoCreateRefTerm = "AStringValue,AVector2Value,AVector3Value,BStringValue,BVector2Value,BVector3Value",
				DisplayName = "Evaluate Condition"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateCondition(EShidenVariableType Type, const FString& Operator, bool bABooleanValue,
	                                                                 const FString& AStringValue, int32 AIntegerValue, float AFloatValue,
	                                                                 const FVector2D& AVector2Value, const FVector& AVector3Value,
                                                                     bool bBBooleanValue, const FString& BStringValue, int32 BIntegerValue,
	                                                                 float BFloatValue, const FVector2D& BVector2Value, const FVector& BVector3Value,
	                                                                 bool& bResult, FString& ErrorMessage);

	static FRegexPattern& GetReplaceTextPattern();

	static FRegexPattern& GetVariablePattern();

private:
	template <class T>
	static bool TryUpdateUserVariableImpl(const FString& Name, const T& Value, EShidenVariableType Type, FString& ErrorMessage);

	template <class T>
	static bool TryUpdateSystemVariableImpl(const FString& Name, const T& Value, EShidenVariableType Type, FString& ErrorMessage);

	template <class T>
	static bool TryUpdateLocalVariableImpl(const FString& ProcessName, const FString& Name, const T& Value, EShidenVariableType Type, FString& ErrorMessage);

	template <class T>
	static bool TryEvaluateComparisonImpl(const FString& Operator, const T& A, const T& B, bool& bResult, FString& ErrorMessage);

	static bool TryUpdatePredefinedVariable(const UObject* WorldContextObject, const EShidenVariableType& Type, const FString& Name,
	                                        const bool bBooleanValue, const FString& StringValue, const int32 IntegerValue, const float FloatValue,
	                                        FString& ErrorMessage);

	static FString ReplaceVariables(const FString& ProcessName, const FString& Text);

	static bool TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey);

	static FString MakeUpdateErrorMessage(TObjectPtr<UShidenSubsystem> ShidenSubsystem, const FString& Name, const EShidenVariableType& Type);

	static void ApplyVolumeRate(const UObject* WorldContextObject, USoundMix* TargetSoundMix, USoundClass* TargetSoundClass, float TargetVolumeRate);
};
