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
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void UpdateUserBoolean(const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates an integer user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new integer value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void UpdateUserInteger(const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a float user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new float value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void UpdateUserFloat(const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a string user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new string value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateUserString(const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a Vector2D user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector2D value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateUserVector2(const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a Vector3 user variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector3 value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateUserVector3(const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage);

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
	 * @param bSuccess [out] True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindUserVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess);

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
	 * @param bSuccess [out] True if the variable was found
	 * @param ErrorMessage [out] Error message if the operation failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindUserVariable(const FString& Name, EShidenVariableType& VariableType, bool& bBooleanValue,
	                             FString& StringValue, int32& IntegerValue, float& FloatValue,
	                             FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

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
	 * @param bSuccess [out] True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindUserVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

	/**
	 * Updates a boolean system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param bValue The new boolean value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void UpdateSystemBoolean(const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates an integer system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new integer value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void UpdateSystemInteger(const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a float system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new float value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void UpdateSystemFloat(const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a string system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new string value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateSystemString(const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a Vector2D system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector2D value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateSystemVector2(const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a Vector3 system variable.
	 * 
	 * @param Name The name of the variable to update
	 * @param Value The new Vector3 value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateSystemVector3(const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Finds a system variable definition by name.
	 * 
	 * @param Name The name of the variable
	 * @param Definition [out] The variable definition
	 * @param bSuccess [out] True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess);

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
	 * @param bSuccess [out] True if the variable was found
	 * @param ErrorMessage [out] Error message if the operation failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void FindSystemVariable(const FString& Name, EShidenVariableType& VariableType,
	                               bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
	                               float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

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
	 * @param bSuccess [out] True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void FindSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

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
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void UpdateLocalBoolean(const FString& ProcessName, const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates an integer local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new integer value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void UpdateLocalInteger(const FString& ProcessName, const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a float local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new float value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void UpdateLocalFloat(const FString& ProcessName, const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a string local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new string value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Value"))
	static void UpdateLocalString(const FString& ProcessName, const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a Vector2D local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new Vector2D value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Value"))
	static void UpdateLocalVector2(const FString& ProcessName, const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Updates a Vector3 local variable.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to update
	 * @param Value The new Vector3 value
	 * @param bSuccess [out] True if the update was successful
	 * @param ErrorMessage [out] Error message if the update failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "Value"))
	static void UpdateLocalVector3(const FString& ProcessName, const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Finds a local variable definition by name.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable
	 * @param Definition [out] The variable definition
	 * @param bSuccess [out] True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariableDefinition(const FString& ProcessName, const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess);

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
	 * @param bSuccess [out] True if the variable was found
	 * @param ErrorMessage [out] Error message if the operation failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariable(const FString& ProcessName, const FString& Name,
	                              EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
	                              float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Finds a local variable and converts its value to a string.
	 * 
	 * @param ProcessName The name of the process
	 * @param Name The name of the variable to find
	 * @param OriginalType [out] The original type of the variable
	 * @param Result [out] The variable value as a string
	 * @param bSuccess [out] True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariableAsString(const FString& ProcessName, const FString& Name, EShidenVariableType& OriginalType, FString& Result,
	                                      bool& bSuccess);

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
	 * @param bSuccess [out] True if the update was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void UpdatePredefinedSystemVariableByString(const UObject* WorldContextObject, const FString& Name, const FString& Value, bool& bSuccess);

	/**
	 * Finds a predefined system variable and converts its value to a string.
	 * 
	 * @param Name The name of the variable to find
	 * @param OriginalType [out] The original type of the variable
	 * @param Result [out] The variable value as a string
	 * @param bSuccess [out] True if the variable was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables")
	static void FindPredefinedSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

	/**
	 * Sets the master volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param InValue The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetMasterVolume(const UObject* WorldContextObject, float InValue);

	/**
	 * Sets the BGM volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param InValue The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"),
		DisplayName = "Set BGM Volume")
	static void SetBGMVolume(const UObject* WorldContextObject, float InValue);

	/**
	 * Sets the SE (Sound Effect) volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param InValue The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"),
		DisplayName = "Set SE Volume")
	static void SetSEVolume(const UObject* WorldContextObject, float InValue);

	/**
	 * Sets the voice volume.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 * @param InValue The new volume rate value (0.0-1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetVoiceVolume(const UObject* WorldContextObject, float InValue);

	/**
	 * Initializes all predefined system variables with their default values.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void InitPredefinedSystemVariables(const UObject* WorldContextObject);

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
	 * @param bSuccess [out] True if the variable exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindPredefinedSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess);

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables")
	static TArray<FString> GetVariableNamesFromText(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables")
	static FShidenCommand ReplaceVariablesForLoad(FShidenVariable TempLocalVariable, const FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static FShidenCommand ReplaceAllVariable(const FString& ProcessName, const FShidenCommand& Command);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables")
	static void ConvertToVariableKind(const FString& VariableKind, EShidenVariableKind& Result, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables")
	static void ConvertToVariableType(const FString& VariableType, EShidenVariableType& Result, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables")
	static void ConvertToAssetPathType(const FString& AssetPathType, EShidenAssetPathType& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static void FindVariableDefinition(const FString& ProcessName, EShidenVariableKind Kind, const FString& Name,
	                                   FShidenVariableDefinition& Definition, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables",
		meta = (ProcessName = "Default", AutoCreateRefTerm = "StringValue,Vector2Value,Vector3Value"))
	static void UpdateVariable(const FString& ProcessName, EShidenVariableKind Kind, EShidenVariableType Type,
	                           const FString& Name, bool bBooleanValue, const FString& StringValue,
	                           int32 IntegerValue, float FloatValue, const FVector2D& Vector2Value,
	                           const FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static void FindVariable(const FString& ProcessName, EShidenVariableKind Kind,
	                         const FString& Name, EShidenVariableType& VariableType, bool& bBooleanValue,
	                         FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value,
	                         FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	static void EvaluateBoolean(const FString& Operator, bool bABooleanValue, bool bBBooleanValue, bool& bResult, bool& bSuccess,
	                            FString& ErrorMessage);

	static void EvaluateString(const FString& Operator, const FString& AStringValue, const FString& BStringValue, bool& bResult, bool& bSuccess,
	                           FString& ErrorMessage);

	static void EvaluateInteger(const FString& Operator, int32 AIntegerValue, int32 BIntegerValue, bool& bResult, bool& bSuccess,
	                            FString& ErrorMessage);

	static void EvaluateFloat(const FString& Operator, float AFloatValue, float BFloatValue, bool& bResult, bool& bSuccess, FString& ErrorMessage);

	static void EvaluateVector2(const FString& Operator, const FVector2D& AVector2Value, const FVector2D& BVector2Value, bool& bResult,
	                            bool& bSuccess, FString& ErrorMessage);

	static void EvaluateVector3(const FString& Operator, const FVector& AVector3Value, const FVector& BVector3Value, bool& bResult, bool& bSuccess,
	                            FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables",
		meta = (AutoCreateRefTerm = "AStringValue,AVector2Value,AVector3Value,BStringValue,BVector2Value,BVector3Value"))
	static void EvaluateCondition(EShidenVariableType Type, const FString& Operator, bool bABooleanValue,
	                              const FString& AStringValue, int32 AIntegerValue, float AFloatValue,
	                              const FVector2D& AVector2Value, const FVector& AVector3Value,
	                              bool bBBooleanValue, const FString& BStringValue, int32 BIntegerValue,
	                              float BFloatValue, const FVector2D& BVector2Value, const FVector& BVector3Value,
	                              bool& bResult, bool& bSuccess, FString& ErrorMessage);

	static FRegexPattern& GetReplaceTextPattern();

	static FRegexPattern& GetVariablePattern();

private:
	static FString ReplaceVariables(const FString& ProcessName, const FString& Text);

	static bool TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey);

	static FString MakeUpdateErrorMessage(TObjectPtr<UShidenSubsystem> ShidenSubsystem, const FString& Name, const EShidenVariableType& Type);

	static void ApplyVolumeRate(const UObject* WorldContextObject, USoundMix* TargetSoundMix, USoundClass* TargetSoundClass, float TargetVolumeRate);
};
