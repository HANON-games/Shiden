// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Variable/ShidenVariableType.h"
#include "Variable/ShidenVariableKind.h"
#include "Internationalization/Regex.h"
#include "Engine/Engine.h"
#include "System/ShidenSubsystem.h"
#include "Utility/ShidenCoreFunctionLibrary.h"
#include "ShidenVariableFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenVariableFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void UpdateUserBoolean(const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void UpdateUserInteger(const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void UpdateUserFloat(const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateUserString(const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateUserVector2(const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateUserVector3(const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void ContainsUserVariable(const FString& Name, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void GetUserVariableNames(TArray<FString>& Names);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static int32 UserVariableNum();
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void GetUserVariableType(const FString& Name, EShidenVariableType& Type, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void ResetUserVariable(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindUserVariable(const FString& Name, EShidenVariableType& VariableType, bool& bBooleanValue,
	                             FString& StringValue, int32& IntegerValue, float& FloatValue,
	                             FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void ResetUserVariables();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void FindUserVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void UpdateSystemBoolean(const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void UpdateSystemInteger(const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void UpdateSystemFloat(const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateSystemString(const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateSystemVector2(const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables", meta = (AutoCreateRefTerm = "Value"))
	static void UpdateSystemVector3(const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void GetSystemVariableType(const FString& Name, EShidenVariableType& Type, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void ResetSystemVariable(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void ContainsSystemVariable(const FString& Name, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void GetSystemVariableNames(TArray<FString>& Names);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static int32 SystemVariableNum();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void FindSystemVariable(const FString& Name, EShidenVariableType& VariableType,
	                               bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
	                               float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void ResetSystemVariables();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|System Variables")
	static void FindSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", AutoCreateRefTerm = "Arguments"))
	static void InitLocalVariable(const FString& ProcessName, const UShidenScenario* Scenario, const TMap<FString, FString>& Arguments);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void UpdateLocalBoolean(const FString& ProcessName, const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void UpdateLocalInteger(const FString& ProcessName, const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void UpdateLocalFloat(const FString& ProcessName, const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", AutoCreateRefTerm = "Value"))
	static void UpdateLocalString(const FString& ProcessName, const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", AutoCreateRefTerm = "Value"))
	static void UpdateLocalVector2(const FString& ProcessName, const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default", AutoCreateRefTerm = "Value"))
	static void UpdateLocalVector3(const FString& ProcessName, const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables", meta = (ProcessName = "Default"))
	static void GetLocalVariableType(const FString& ProcessName, const FString& Name, EShidenVariableType& Type, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariable(const FString& ProcessName, const FString& Name,
	                              EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
	                              float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariableAsString(const FString& ProcessName, const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void ResetLocalVariable(const FString& ProcessName, const FString& Name);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveLocalVariablesInCurrentScope(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveLocalVariablesInProcess(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Local Variables")
	static void ResetAllLocalVariables();
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void UpdatePredefinedSystemVariableByString(const UObject* WorldContextObject, const FString& Name, const FString& Value, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables")
	static void FindPredefinedSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetMasterVolumeRate(const UObject* WorldContextObject, float InValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetBgmVolumeRate(const UObject* WorldContextObject, float InValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetSeVolumeRate(const UObject* WorldContextObject, float InValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void SetVoiceVolumeRate(const UObject* WorldContextObject, float InValue);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables")
	static TArray<FString> GetVariableNamesFromText(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables")
	static FShidenCommand ReplaceVariablesForLoad(FShidenVariable TempLocalVariable, const FShidenCommand& Command);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static FShidenCommand ReplaceAllVariable(const FString& ProcessName, const FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|Predefined System Variables", meta = (WorldContext="WorldContextObject"))
	static void InitPredefinedSystemVariables(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables|Predefined System Variables")
	static FShidenPredefinedSystemVariable GetPredefinedSystemVariable();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables|User Variables")
	static void GetPredefinedSystemVariableType(const FString& Name, EShidenVariableType& Type, bool& bResult);

	// internal functions
	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables")
	static void ConvertToVariableKind(const FString& VariableKind, EShidenVariableKind& Result, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Variables")
	static void ConvertToVariableType(const FString& VariableType, EShidenVariableType& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static void GetVariableDefinition(const FString& ProcessName, EShidenVariableKind Kind, const FString& Name,
								FShidenVariableDefinition& Definition, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default", AutoCreateRefTerm = "StringValue,Vector2Value,Vector3Value"))
	static void UpdateVariable(const FString& ProcessName, EShidenVariableKind Kind, EShidenVariableType Type,
	                           const FString& Name, bool bBooleanValue, const FString& StringValue,
	                           int32 IntegerValue, float FloatValue, const FVector2D& Vector2Value,
	                           const FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (ProcessName = "Default"))
	static void FindVariable(const FString& ProcessName, EShidenVariableKind Kind,
	                         const FString& Name, EShidenVariableType& VariableType, bool& bBooleanValue,
	                         FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value,
	                         FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Variables", meta = (AutoCreateRefTerm = "AStringValue,AVector2Value,AVector3Value,BStringValue,BVector2Value,BVector3Value"))
	static void EvaluateCondition(EShidenVariableType Type, const FString& Operator, bool bABooleanValue,
	                              const FString& AStringValue, int32 AIntegerValue, float AFloatValue,
	                              const FVector2D& AVector2Value, const FVector& AVector3Value,
	                              bool bBBooleanValue, const FString& BStringValue, int32 BIntegerValue,
	                              float BFloatValue, const FVector2D& BVector2Value, const FVector& BVector3Value,
	                              bool& bResult, bool& bSuccess, FString& ErrorMessage);

private:
	static FString ReplaceVariables(const FString& ProcessName, const FString& Text);

	static bool TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey);

	static FString MakeUpdateErrorMessage(TObjectPtr<UShidenSubsystem> ShidenSubsystem, const FString& Name, const EShidenVariableType& Type);

	static void ApplyVolumeRate(const UObject* WorldContextObject, USoundMix* TargetSoundMix, USoundClass* TargetSoundClass, float TargetVolumeRate);
};
