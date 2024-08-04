// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Serialization/Csv/CsvParser.h"
#include "ShidenCsvParsedRow.h"
#include "Engine/Blueprint.h"
#include "ShidenVariableType.h"
#include "ShidenVariableKind.h"
#include "ShidenCoreFunctionLibrary.h"
#include "ShidenVariableFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenVariableFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void AddUserBoolean(const FString Key, const bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void AddUserInteger(const FString Key, const int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void AddUserFloat(const FString Key, const float Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void AddUserString(const FString Key, const FString Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void AddUserVector3(const FString Key, const FVector Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void AddUserVector2(const FString Key, const FVector2D Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void ContainsUserVariable(const FString Key, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static bool IsEmptyUserVariable();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static bool IsNotEmptyUserVariable();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void UserVariableKeys(TArray<FString>& Keys);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static int32 UserVariableLength();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void RemoveUserVariable(const FString Key);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void FindUserVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void ClearUserVariables();
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|User Variables")
	static void FindUserVariableAsString(const FString Key, FString& Result, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void AddSystemBoolean(const FString Key, const bool bValue);
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void AddSystemInteger(const FString Key, const int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void AddSystemFloat(const FString Key, const float Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void AddSystemString(const FString Key, const FString Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void AddSystemVector3(const FString Key, const FVector Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void AddSystemVector2(const FString Key, const FVector2D Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void RemoveSystemVariable(const FString Key);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void ContainsSystemVariable(const FString Key, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static bool IsEmptySystemVariable();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static bool IsNotEmptySystemVariable();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void SystemVariableKeys(TArray<FString>& Keys);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static int32 SystemVariableLength();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void FindSystemVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void ClearSystemVariables();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System Variables")
	static void FindSystemVariableAsString(const FString Key, FString& Result, bool& bReturnValue);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void MakeLocalVariableKey(const FString ProcessName, FString& LocalKey, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void AddLocalBoolean(const FString ProcessName, const FString Key, const bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void AddLocalInteger(const FString ProcessName, const FString Key, const int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void AddLocalFloat(const FString ProcessName, const FString Key, const float Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void AddLocalString(const FString ProcessName, const FString Key, const FString Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void AddLocalVector3(const FString ProcessName, const FString Key, const FVector Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void AddLocalVector2(const FString ProcessName, const FString Key, const FVector2D Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariable(const FString ProcessName, const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void FindLocalVariableAsString(const FString ProcessName, const FString Key, FString& Result, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveLocalVariable(const FString ProcessName, const FString Key);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveCurrentLocalVariables(const FString ProcessName);
		
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables", meta = (ProcessName = "Default"))
	static void RemoveProcessLocalVariables(const FString ProcessName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Local Variables")
	static void ClearAllLocalVariables();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Predefined System Variables")
	static void FindPredefinedSystemVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Predefined System Variables")
	static void FindPredefinedSystemVariableAsString(const FString Key, FString& Result, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static TArray<FString> GetVariableNamesFromText(const FString text);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static FShidenCommand ReplaceAllTextWithVariable(const FString ProcessName, const FShidenCommand Command);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void InitPredefindSystemVariable();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetSkipSpeedRate(float& SkipSpeedRate);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetVoiceVolumeRate(float& VoiceVolumeRate);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetSeVolumeRate(float& SeVolumeRate);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetBgmVolumeRate(float& BgmVolumeRate);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetShowMouseCursorWhenStarting(bool& bShowMouseCursorWhenStarting);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetLanguageIndex(int& LanguageIndex);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetSecondsToWaitForEachLetter(float& SecondsToWaitForEachLetter);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetWaitTimeOnAutoMode(float& WaitTimeOnAutoMode);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetStopVoiceWhenNextDialogueStart(bool& bStopVoiceWhenNextDialogueStart);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetMappingContext(UInputMappingContext*& MappingContext);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetUseAddToPlayerScreenInsteadOfAddToViewport(bool& bUseAddToPlayerScreenInsteadOfAddToViewport);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetAutoRemoveWidgetOnEnd(bool& bAutoRemoveWidgetOnEnd);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetSkipUnread(bool& bSkipUnread);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetTargetFrameRate(int& TargetFrameRate);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Variables")
	static void GetMarkedReadLines(TMap<FGuid, FShidenReadLines>& MarkedReadLines);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetSkipSpeedRate(const float SkipSpeedRate);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetVoiceVolumeRate(const float VoiceVolumeRate);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetSeVolumeRate(const float SeVolumeRate);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetBgmVolumeRate(const float BgmVolumeRate);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetShowMouseCursorWhenStarting(const bool bShowMouseCursorWhenStarting);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetLanguageIndex(const int LanguageIndex);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetSecondsToWaitForEachLetter(const float SecondsToWaitForEachLetter);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetWaitTimeOnAutoMode(const float WaitTimeOnAutoMode);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetStopVoiceWhenNextDialogueStart(const bool bStopVoiceWhenNextDialogueStart);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetUseAddToPlayerScreenInsteadOfAddToViewport(const bool bUseAddToPlayerScreenInsteadOfAddToViewport);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetAutoRemoveWidgetOnEnd(const bool bAutoRemoveWidgetOnEnd);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetSkipUnread(const bool bSkipUnread);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetTargetFrameRate(const int TargetFrameRate);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Variables")
	static void SetMarkedReadLines(const TMap<FGuid, FShidenReadLines>& MarkedReadLines);

	// internal functions
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Internal|Variables")
	static void ConvertToVariableKind(const FString VariableKind, EShidenVariableKind& Result, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Internal|Variables")
	static void ConvertToVariableType(const FString VariableType, EShidenVariableType& Result, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Variables")
	static void AddVariable(const FString ProcessName, const EShidenVariableKind Kind, const EShidenVariableType Type, const FString Key, const bool bBooleanValue, const FString StringValue, const int IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Variables")
	static void FindVariable(const FString ProcessName, const EShidenVariableKind Kind, const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Variables")
	static void EvaluateCondition(const EShidenVariableType Type, const FString Operator, const bool bABooleanValue, const FString AStringValue, const int AIntegerValue, const float AFloatValue, const FVector2D AVector2Value, const FVector AVector3Value, 
		const bool bBBooleanValue, const FString BStringValue, const int BIntegerValue, const float BFloatValue, const FVector2D BVector2Value, const FVector BVector3Value, bool& bResult, bool& bSuccess, FString& ErrorMessage);
};
