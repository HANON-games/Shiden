// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Variable/ShidenVariableFunctionLibrary.h"
#include "Scenario/ShidenScenarioFunctionLibrary.h"

SHIDENCORE_API FString UShidenVariableFunctionLibrary::MakeUpdateErrorMessage(const TObjectPtr<UShidenSubsystem> ShidenSubsystem, const FString& Name, const EShidenVariableType& Type)
{
	FShidenVariableDefinition Definition;
	if (!ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition))
	{
		return FString::Printf(TEXT("Variable %s is not defined."), *Name);
	}
	if (Definition.IsReadOnly)
	{
		return FString::Printf(TEXT("Variable %s is read only."), *Name);
	}
	const EShidenVariableType& TypeA = Definition.Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Definition.Type;
	const EShidenVariableType& TypeB = Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Type;
	if (TypeA != TypeB)
	{
		return FString::Printf(TEXT("Variable %s is not %s."), *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
	}
	return FString::Printf(TEXT("Unknown error."));
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateUserBoolean(const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, bValue);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateUserInteger(const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateUserFloat(const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateUserString(const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateUserVector3(const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateUserVector2(const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ContainsUserVariable(const FString& Name, bool& bResult)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bResult = ShidenSubsystem->UserVariable.Contains(Name);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetUserVariableNames(TArray<FString>& Names)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->UserVariable.GetNames(Names);
}

SHIDENCORE_API int32 UShidenVariableFunctionLibrary::UserVariableNum()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->UserVariable.Num();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetUserVariableType(const FString& Name, EShidenVariableType& Type, bool& bResult)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	bResult = ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition);
	Type = Definition.Type;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ResetUserVariable(const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->UserVariable.TryReset(Name);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindUserVariable(const FString& Name, EShidenVariableType& VariableType,
                                                                     bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue,
                                                                     FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	bReturnValue = ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition);
	VariableType = Definition.Type;

	if (bReturnValue)
	{
		switch (VariableType)
		{
		case EShidenVariableType::Boolean:
			ShidenSubsystem->UserVariable.TryGet(Name, bBooleanValue);
			break;
		case EShidenVariableType::Integer:
			ShidenSubsystem->UserVariable.TryGet(Name, IntegerValue);
			break;
		case EShidenVariableType::Float:
			ShidenSubsystem->UserVariable.TryGet(Name, FloatValue);
			break;
		case EShidenVariableType::String:
		case EShidenVariableType::AssetPath:
			ShidenSubsystem->UserVariable.TryGet(Name, StringValue);
			break;
		case EShidenVariableType::Vector2:
			ShidenSubsystem->UserVariable.TryGet(Name, Vector2Value);
			break;
		case EShidenVariableType::Vector3:
			ShidenSubsystem->UserVariable.TryGet(Name, Vector3Value);
			break;
		}
	}
	else
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ResetUserVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindUserVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bReturnValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bReturnValue = ShidenSubsystem->UserVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateSystemBoolean(const FString& Name, bool bValue, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, bValue);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateSystemInteger(const FString& Name, int32 Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateSystemFloat(const FString& Name, float Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateSystemString(const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateSystemVector2(const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateSystemVector3(const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
	}
}

void UShidenVariableFunctionLibrary::GetSystemVariableType(const FString& Name, EShidenVariableType& Type, bool& bResult)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	bResult = ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition);
	Type = Definition.Type;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ContainsSystemVariable(const FString& Name, bool& bResult)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bResult = ShidenSubsystem->SystemVariable.Contains(Name);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSystemVariableNames(TArray<FString>& Names)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable.GetNames(Names);
}

SHIDENCORE_API int32 UShidenVariableFunctionLibrary::SystemVariableNum()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->SystemVariable.Num();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ResetSystemVariable(const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable.TryReset(Name);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindSystemVariable(const FString& Name, EShidenVariableType& VariableType,
                                                                       bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue,
                                                                       FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	bReturnValue = ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition);
	VariableType = Definition.Type;

	if (bReturnValue)
	{
		switch (VariableType)
		{
		case EShidenVariableType::Boolean:
			ShidenSubsystem->SystemVariable.TryGet(Name, bBooleanValue);
			break;
		case EShidenVariableType::Integer:
			ShidenSubsystem->SystemVariable.TryGet(Name, IntegerValue);
			break;
		case EShidenVariableType::Float:
			ShidenSubsystem->SystemVariable.TryGet(Name, FloatValue);
			break;
		case EShidenVariableType::String:
		case EShidenVariableType::AssetPath:
			ShidenSubsystem->SystemVariable.TryGet(Name, StringValue);
			break;
		case EShidenVariableType::Vector2:
			ShidenSubsystem->SystemVariable.TryGet(Name, Vector2Value);
			break;
		case EShidenVariableType::Vector3:
			ShidenSubsystem->SystemVariable.TryGet(Name, Vector3Value);
			break;
		}
	}
	else
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ResetSystemVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bReturnValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bReturnValue = ShidenSubsystem->SystemVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (const FShidenScenarioProgressStack* Stack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName))
	{
		const int32 LastIndex = Stack->Stack.Num() - 1;
		ScenarioKey = FString::Printf(TEXT("%s$%d"), *ProcessName, LastIndex);
		return true;
	}
#if WITH_EDITOR
	if (const TObjectPtr<UGameViewportClient> GameViewport = GEngine->GameViewport)
	{
		if (const TObjectPtr<UWorld> World = GameViewport->GetWorld())
		{
			if (World->WorldType == EWorldType::PIE)
			{
				UE_LOG(LogTemp, Warning, TEXT("Scenario not found: Process Name \"%s\""), *ProcessName);
			}
		}
	}
#else
	UE_LOG(LogTemp, Warning, TEXT("Scenario not found: Process Name \"%s\""), *ProcessName);
#endif
	ScenarioKey = FString();
	return false;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::InitLocalVariable(const FString& ProcessName, const UShidenScenario* Scenario, const TMap<FString, FString>& Arguments)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.InitLocalVariable(ScopeKey, Scenario);
		for (const FShidenVariableDefinition& Definition : Scenario->MacroParameterDefinitions)
		{
			if (Arguments.Contains(Definition.Name))
			{
				switch (Definition.Type)
				{
				case EShidenVariableType::Boolean:
					{
						const bool bBooleanValue = Arguments[Definition.Name].Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
						ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Definition.Name, bBooleanValue, true);
						break;
					}
				case EShidenVariableType::String:
				case EShidenVariableType::AssetPath:
					{
						ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Definition.Name, Arguments[Definition.Name], true);
						break;						
					}
				case EShidenVariableType::Integer:
					{
						const int32 IntegerValue = FCString::Atoi(*Arguments[Definition.Name]);
						ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Definition.Name, IntegerValue, true);
						break;
					}
				case EShidenVariableType::Float:
					{
						const float FloatValue = FCString::Atof(*Arguments[Definition.Name]);
						ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Definition.Name, FloatValue, true);
						break;
					}
				case EShidenVariableType::Vector2:
					{
						FVector2d Vector2Value;
						Vector2Value.InitFromString(Arguments[Definition.Name]);
						ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Definition.Name, Vector2Value, true);
						break;
					}
				case EShidenVariableType::Vector3:
					{
						FVector Vector3Value;
						Vector3Value.InitFromString(Arguments[Definition.Name]);
						ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Definition.Name, Vector3Value, true);
						break;
					}
				}
			}
		}
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ResetLocalVariable(const FString& ProcessName, const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.TryReset(ScopeKey, Name);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateLocalBoolean(const FString& ProcessName, const FString& Name, const bool bValue, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, bValue);
		if (!bSuccess)
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
		}
	}
	else
	{
		bSuccess = false;
		ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateLocalInteger(const FString& ProcessName, const FString& Name, const int32 Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value);
		if (!bSuccess)
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
		}
	}
	else
	{
		bSuccess = false;
		ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateLocalFloat(const FString& ProcessName, const FString& Name, const float Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value);
		if (!bSuccess)
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
		}
	}
	else
	{
		bSuccess = false;
		ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateLocalString(const FString& ProcessName, const FString& Name, const FString& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value);
		if (!bSuccess)
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
		}
	}
	else
	{
		bSuccess = false;
		ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateLocalVector2(const FString& ProcessName, const FString& Name, const FVector2D& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value);
		if (!bSuccess)
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
		}
	}
	else
	{
		bSuccess = false;
		ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateLocalVector3(const FString& ProcessName, const FString& Name, const FVector& Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value);
		if (!bSuccess)
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
		}
	}
	else
	{
		bSuccess = false;
		ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	}
}

void UShidenVariableFunctionLibrary::GetLocalVariableType(const FString& ProcessName, const FString& Name, EShidenVariableType& Type, bool& bResult)
{
	bResult = false;
	
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		FShidenVariableDefinition Definition;
		bResult = ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition);
		Type = Definition.Type;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveLocalVariablesInCurrentScope(const FString& ProcessName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.RemoveVariablesInCurrentScope(ScopeKey);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveLocalVariablesInProcess(const FString& ProcessName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->LocalVariable.RemoveVariablesInProcess(ProcessName);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ResetAllLocalVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->LocalVariable.ResetAll();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindLocalVariable(const FString& ProcessName, const FString& Name, EShidenVariableType& VariableType,
                                                                      bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue,
                                                                      FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FString ScopeKey;
	if (!TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bReturnValue = false;
		return;
	}

	FShidenVariableDefinition Definition;
	bReturnValue = ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition);
	VariableType = Definition.Type;
	
	if (bReturnValue)
	{
		switch (VariableType)
		{
		case EShidenVariableType::Boolean:
			ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, bBooleanValue);
			break;
		case EShidenVariableType::Integer:
			ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, IntegerValue);
			break;
		case EShidenVariableType::Float:
			ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, FloatValue);
			break;
		case EShidenVariableType::String:
		case EShidenVariableType::AssetPath:
			ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, StringValue);
			break;
		case EShidenVariableType::Vector2:
			ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, Vector2Value);
			break;
		case EShidenVariableType::Vector3:
			ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, Vector3Value);
			break;
		}
	}
	else
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindLocalVariableAsString(const FString& ProcessName, const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bReturnValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FString ScopeKey;
	bReturnValue = TryCreateScopeKey(ProcessName, ScopeKey)
		&& ShidenSubsystem->LocalVariable.TryGetAsString(ScopeKey, Name, OriginalType, Result);
}

#define PROPERTY_NAME_OF(Property) GetLowerPropertyName(FString(TEXT(#Property)))

FString GetLowerPropertyName(const FString& FullPropertyName)
{
	FString Result, Dummy;
	FullPropertyName.Split(TEXT("::"), &Dummy, &Result);
	return Result;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdatePredefinedSystemVariableByString(const FString& Name, const FString& Value, bool& bReturnValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	bReturnValue = ShidenSubsystem->PredefinedSystemVariable.TryUpdateByString(Name, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindPredefinedSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result, bool& bReturnValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	bReturnValue = ShidenSubsystem->PredefinedSystemVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API TArray<FString> UShidenVariableFunctionLibrary::GetVariableNamesFromText(const FString& Text)
{
	if (!Text.Contains(TEXT("{")) || !Text.Contains(TEXT("}")))
	{
		return TArray<FString>();
	}

	TArray<FString> Result;

	const FRegexPattern GetVariablePattern =
		FRegexPattern(FString(TEXT("(?<!\\\\)\\{\\s*([^ \\}]+?)\\s*(?<!\\\\)\\}")));
	FRegexMatcher Matcher(GetVariablePattern, Text);

	while (Matcher.FindNext())
	{
		Result.AddUnique(Matcher.GetCaptureGroup(1));
	}

	return Result;
}

FString ReplaceVariables(const FString& ProcessName, const FString& Text)
{
	if (!Text.Contains(TEXT("{")) || !Text.Contains(TEXT("}")))
	{
		return Text;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	FString ResultText = Text;

	const FRegexPattern ReplaceTextPattern = FRegexPattern(FString(TEXT("((?<!\\\\)\\{ *[^ \\}]+ *(?<!\\\\)\\})")));
	FRegexMatcher Matcher(ReplaceTextPattern, Text);

	while (Matcher.FindNext())
	{
		FString Str = Matcher.GetCaptureGroup(1);
		FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();

		FString VariableKind, ReplacementText, VariableKey;
		VariableName.Split(TEXT("::"), &VariableKind, &VariableKey, ESearchCase::CaseSensitive);
		
		if (VariableKey.IsEmpty())
		{
			EShidenVariableType Type;
			if (!ShidenSubsystem->UserVariable.TryGetAsString(VariableName.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText))
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("System"))
		{
			EShidenVariableType Type;
			if (!ShidenSubsystem->SystemVariable.TryGetAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText))
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("Local"))
		{
			EShidenVariableType Type;
			FString ScopeKey;
			if (!UShidenVariableFunctionLibrary::TryCreateScopeKey(ProcessName, ScopeKey)
				|| !ShidenSubsystem->LocalVariable.TryGetAsString(ScopeKey, VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText))
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("Predefined"))
		{
			EShidenVariableType Type;
			bool bReturnValue;
			UShidenVariableFunctionLibrary::FindPredefinedSystemVariableAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText, bReturnValue);
			if (!bReturnValue)
			{
				ReplacementText = TEXT("Error");
			}
		}
		else
		{
			ReplacementText = TEXT("Error");
		}

		ResultText.ReplaceInline(*Str, *ReplacementText, ESearchCase::CaseSensitive);
	}

	return ResultText;
}

SHIDENCORE_API FShidenCommand UShidenVariableFunctionLibrary::ReplaceVariablesForLoad(FShidenVariable TempLocalVariable, const FShidenCommand& Command)
{
	FShidenCommand Result = Command;
	Result.Args.Empty();
	TArray<FString> Keys;
	Command.Args.GetKeys(Keys);

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	for (const FString& Key : Keys)
	{
		FString Value = Command.Args.FindRef(Key);

		if (!Value.Contains(TEXT("{")) || !Value.Contains(TEXT("}")))
		{
			Result.Args.Add(Key, Value);
			continue;
		}

		FString ResultText = Value;

		const FRegexPattern ReplaceTextPattern = FRegexPattern(FString(TEXT("((?<!\\\\)\\{ *[^ \\}]+ *(?<!\\\\)\\})")));
		FRegexMatcher Matcher(ReplaceTextPattern, Value);

		while (Matcher.FindNext())
		{
			FString Str = Matcher.GetCaptureGroup(1);
			FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();

			FString VariableKind, ReplacementText, VariableKey;
			VariableName.Split(TEXT("::"), &VariableKind, &VariableKey, ESearchCase::CaseSensitive);

			EShidenVariableType Type;

			if (VariableKey.IsEmpty())
			{
				// UserVariable
				FString TempKey = VariableName.Replace(TEXT("\\:"), TEXT(":"));
				FShidenVariableDefinition Definition;
				if (ShidenSubsystem->UserVariable.TryGetDefinition(TempKey, Definition) && Definition.IsReadOnly)
				{
					if (!ShidenSubsystem->UserVariable.TryGetAsString(TempKey, Type, ReplacementText))
					{
						continue;
					}
				}
			}
			else if (VariableKind == TEXT("System"))
			{
				// SystemVariable
				FString TempKey = VariableKey.Replace(TEXT("\\:"), TEXT(":"));
				FShidenVariableDefinition Definition;
				if (ShidenSubsystem->SystemVariable.TryGetDefinition(TempKey, Definition) && Definition.IsReadOnly)
				{
					if (!ShidenSubsystem->SystemVariable.TryGetAsString(TempKey, Type, ReplacementText))
					{
						continue;
					}
				}
			}
			else if (VariableKind == TEXT("Local"))
			{
				// LocalVariable
				const FString TempKey = VariableKey.Replace(TEXT("\\:"), TEXT(":"));
				if (!TempLocalVariable.TryGetAsString(TempKey, Type, ReplacementText))
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			ResultText.ReplaceInline(*Str, *ReplacementText, ESearchCase::CaseSensitive);
		}
		
		Result.Args.Add(Key, ResultText);
	}
	return Result;
}

SHIDENCORE_API FShidenCommand UShidenVariableFunctionLibrary::ReplaceAllVariable(const FString& ProcessName, const FShidenCommand& Command)
{
	FShidenCommand Result = Command;
	Result.Args = TMap<FString, FString>();
	TArray<FString> Keys;
	Command.Args.GetKeys(Keys);
	for (const FString& Key : Keys)
	{
		FString Value = Command.Args.FindRef(Key);
		FString NewValue = ReplaceVariables(ProcessName, Value);
		Result.Args.Add(Key, NewValue);
	}
	return Result;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ConvertToVariableKind(const FString& VariableKind, EShidenVariableKind& Result, bool& bSuccess)
{
	const FString CleanedVariableKind = VariableKind.Replace(TEXT(" "), TEXT(""));
	bSuccess = false;

	if (CleanedVariableKind.Equals(TEXT("UserVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::UserVariable;
		bSuccess = true;
		return;
	}
	if (CleanedVariableKind.Equals(TEXT("LocalVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::LocalVariable;
		bSuccess = true;
		return;
	}
	if (CleanedVariableKind.Equals(TEXT("SystemVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::SystemVariable;
		bSuccess = true;
		return;
	}
	if (CleanedVariableKind.Equals(TEXT("PredefinedSystemVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::PredefinedSystemVariable;
		bSuccess = true;
		return;
	}
	Result = EShidenVariableKind::UserVariable;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ConvertToVariableType(const FString& VariableType, EShidenVariableType& Result, bool& bSuccess)
{
	const FString CleanedVariableType = VariableType.Replace(TEXT(" "), TEXT(""));

	if (CleanedVariableType.Equals(TEXT("Boolean"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Boolean;
		bSuccess = true;
		return;
	}
	if (CleanedVariableType.Equals(TEXT("String"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::String;
		bSuccess = true;
		return;
	}
	if (CleanedVariableType.Equals(TEXT("Integer"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Integer;
		bSuccess = true;
		return;
	}
	if (CleanedVariableType.Equals(TEXT("Float"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Float;
		bSuccess = true;
		return;
	}
	if (CleanedVariableType.Equals(TEXT("Vector2"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Vector2;
		bSuccess = true;
		return;
	}
	if (CleanedVariableType.Equals(TEXT("Vector3"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Vector3;
		bSuccess = true;
		return;
	}
	if (CleanedVariableType.Equals(TEXT("AssetPath"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::AssetPath;
		bSuccess = true;
		return;
	}
	
	bSuccess = false;
	Result = EShidenVariableType::Boolean;
}

void UShidenVariableFunctionLibrary::GetVariableDefinition(const FString& ProcessName, const EShidenVariableKind Kind,
                                                           const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	switch (Kind) {
	case EShidenVariableKind::UserVariable:
		bSuccess = ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition);
		break;
	case EShidenVariableKind::LocalVariable:
		if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
		{
			bSuccess = ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition);
		}
		else
		{
			bSuccess = false;
			ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
		}
		break;
	case EShidenVariableKind::SystemVariable:
		bSuccess = ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition);
		break;
	case EShidenVariableKind::PredefinedSystemVariable:
		FShidenPredefinedSystemVariableDefinition PredefinedDefinition;
		bSuccess = ShidenSubsystem->PredefinedSystemVariable.TryGetDefinition(Name, PredefinedDefinition);
		if (bSuccess)
		{
			Definition = static_cast<FShidenVariableDefinition>(PredefinedDefinition);
		}
		break;
	}
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::InitPredefinedSystemVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->PredefinedSystemVariable = ShidenProjectConfig->PredefinedSystemVariable;
}

SHIDENCORE_API const FShidenPredefinedSystemVariable& UShidenVariableFunctionLibrary::GetPredefinedSystemVariable()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->PredefinedSystemVariable;
}

void UShidenVariableFunctionLibrary::GetPredefinedSystemVariableType(const FString& Name, EShidenVariableType& Type, bool& bResult)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenPredefinedSystemVariableDefinition Definition;
	bResult = ShidenSubsystem->PredefinedSystemVariable.TryGetDefinition(Name, Definition);
	Type = Definition.Type;
}

SHIDENCORE_API void UpdatePredefinedVariable(const EShidenVariableType& Type, const FString& Name, const bool bBooleanValue,
                                          const FString& StringValue, const int32 IntegerValue, const float FloatValue,
                                          bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	if (Name.IsEmpty())
	{
		ErrorMessage = TEXT("Invalid Key Name");
		return;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenPredefinedSystemVariableDefinition Definition;
	
	if (!ShidenSubsystem->PredefinedSystemVariable.TryGetDefinition(Name, Definition))
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return;
	}
	if (Definition.IsReadOnly)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is read only."), *Name);
		return;
	}
	if (Definition.Type != Type)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not %s."), *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
		return;
	}

	FString Value;
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		Value = bBooleanValue ? TEXT("true") : TEXT("false");
		break;
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		Value = StringValue;
		break;
	case EShidenVariableType::Integer:
		Value = FString::FromInt(IntegerValue);
		break;
	case EShidenVariableType::Float:
		Value = FString::SanitizeFloat(FloatValue);
		break;
	case EShidenVariableType::Vector2:
	case EShidenVariableType::Vector3:
		ErrorMessage = TEXT("Invalid Type");
		return;
	}

	bSuccess = ShidenSubsystem->PredefinedSystemVariable.TryUpdateByString(Name, Value);

	if (!bSuccess)
	{
		ErrorMessage = TEXT("Unknown error.");
	}
}

SHIDENCORE_API void UpdateLocalVariable(const FString& ProcessName, const EShidenVariableType& Type, const FString& Name,
                                        const bool bBooleanValue, const FString& StringValue, const int32 IntegerValue,
                                        const float FloatValue, const FVector2D& Vector2Value, const FVector& Vector3Value,
                                        bool& bSuccess, FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			UShidenVariableFunctionLibrary::UpdateLocalBoolean(ProcessName, Name, bBooleanValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableFunctionLibrary::UpdateLocalString(ProcessName, Name, StringValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			UShidenVariableFunctionLibrary::UpdateLocalInteger(ProcessName, Name, IntegerValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			UShidenVariableFunctionLibrary::UpdateLocalFloat(ProcessName, Name, FloatValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			UShidenVariableFunctionLibrary::UpdateLocalVector2(ProcessName, Name, Vector2Value, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			UShidenVariableFunctionLibrary::UpdateLocalVector3(ProcessName, Name, Vector3Value, bSuccess, ErrorMessage);
			break;
		}
	}
}

SHIDENCORE_API void UpdateSystemVariable(const EShidenVariableType& Type, const FString& Name, const bool bBooleanValue,
                                         const FString& StringValue, const int32 IntegerValue, const float FloatValue,
                                         const FVector2D& Vector2Value, const FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			UShidenVariableFunctionLibrary::UpdateSystemBoolean(Name, bBooleanValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableFunctionLibrary::UpdateSystemString(Name, StringValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			UShidenVariableFunctionLibrary::UpdateSystemInteger(Name, IntegerValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			UShidenVariableFunctionLibrary::UpdateSystemFloat(Name, FloatValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			UShidenVariableFunctionLibrary::UpdateSystemVector2(Name, Vector2Value, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			UShidenVariableFunctionLibrary::UpdateSystemVector3(Name, Vector3Value, bSuccess, ErrorMessage);
			break;
		}
	}
}

SHIDENCORE_API void UpdateUserVariable(const EShidenVariableType& Type, const FString& Name, const bool bBooleanValue,
                                       const FString& StringValue, const int32 IntegerValue, const float FloatValue,
                                       const FVector2D& Vector2Value, const FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			UShidenVariableFunctionLibrary::UpdateUserBoolean(Name, bBooleanValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableFunctionLibrary::UpdateUserString(Name, StringValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			UShidenVariableFunctionLibrary::UpdateUserInteger(Name, IntegerValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			UShidenVariableFunctionLibrary::UpdateUserFloat(Name, FloatValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			UShidenVariableFunctionLibrary::UpdateUserVector2(Name, Vector2Value, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			UShidenVariableFunctionLibrary::UpdateUserVector3(Name, Vector3Value, bSuccess, ErrorMessage);
			break;
		}
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UpdateVariable(const FString& ProcessName,
                                                                   const EShidenVariableKind Kind,
                                                                   const EShidenVariableType Type, const FString& Name,
                                                                   const bool bBooleanValue, const FString& StringValue,
                                                                   const int32 IntegerValue, const float FloatValue,
                                                                   const FVector2D& Vector2Value,
                                                                   const FVector& Vector3Value, bool& bSuccess,
                                                                   FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	switch (Kind)
	{
	case EShidenVariableKind::UserVariable:
		UpdateUserVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess, ErrorMessage);
		bSuccess = true;
		break;
	case EShidenVariableKind::LocalVariable:
		UpdateLocalVariable(ProcessName, Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess, ErrorMessage);
		bSuccess = true;
		break;
	case EShidenVariableKind::SystemVariable:
		UpdateSystemVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess, ErrorMessage);
		bSuccess = true;
		break;
	case EShidenVariableKind::PredefinedSystemVariable:
		UpdatePredefinedVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue,bSuccess, ErrorMessage);
		break;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindVariable(const FString& ProcessName,
                                                                 const EShidenVariableKind Kind, const FString& Name,
                                                                 EShidenVariableType& VariableType, bool& bBooleanValue,
                                                                 FString& StringValue, int& IntegerValue,
                                                                 float& FloatValue, FVector2D& Vector2Value,
                                                                 FVector& Vector3Value, bool& bReturnValue, FString& ErrorMessage)
{
	switch (Kind)
	{
	case EShidenVariableKind::UserVariable:
		FindUserVariable(Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value,
		                 Vector3Value, bReturnValue, ErrorMessage);
		break;
	case EShidenVariableKind::LocalVariable:
		FindLocalVariable(ProcessName, Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue,
		                  Vector2Value, Vector3Value, bReturnValue, ErrorMessage);
		break;
	case EShidenVariableKind::SystemVariable:
		FindSystemVariable(Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value,
		                   Vector3Value, bReturnValue, ErrorMessage);
		break;
	case EShidenVariableKind::PredefinedSystemVariable:
		{
			const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

			check(ShidenSubsystem);

			FString Value;
			bReturnValue = ShidenSubsystem->PredefinedSystemVariable.TryGetAsString(Name, VariableType, Value);

			if (bReturnValue)
			{
				switch (VariableType)
				{
				case EShidenVariableType::Boolean:
					bBooleanValue = Value.Equals(TEXT("true"), ESearchCase::IgnoreCase);
					break;
				case EShidenVariableType::String:
					StringValue = Value;
					break;
				case EShidenVariableType::Integer:
					IntegerValue = FCString::Atoi(*Value);
					break;
				case EShidenVariableType::Float:
					FloatValue = FCString::Atof(*Value);
					break;
				case EShidenVariableType::Vector2:
					Vector2Value.InitFromString(Value);
					break;
				case EShidenVariableType::Vector3:
					Vector3Value.InitFromString(Value);
					break;
				default:
					break;
				}
			}
			else
			{
				ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
			}
			break;
		}
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::EvaluateCondition(const EShidenVariableType Type, const FString& Operator, const bool bABooleanValue,
                                                                      const FString& AStringValue, const int32 AIntegerValue, const float AFloatValue,
                                                                      const FVector2D& AVector2Value, const FVector& AVector3Value, const bool bBBooleanValue,
                                                                      const FString& BStringValue, const int32 BIntegerValue, const float BFloatValue,
                                                                      const FVector2D& BVector2Value, const FVector& BVector3Value, bool& bResult,
                                                                      bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = bABooleanValue == bBBooleanValue;
				return;
			}
			if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = bABooleanValue != bBBooleanValue;
				return;
			}
			ErrorMessage = TEXT("Invalid Operator");
			return;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AStringValue.Equals(BStringValue, ESearchCase::IgnoreCase);
				return;
			}
			if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = !AStringValue.Equals(BStringValue, ESearchCase::IgnoreCase);
				return;
			}
			ErrorMessage = TEXT("Invalid Operator");
			return;
		}
	case EShidenVariableType::Integer:
		{
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AIntegerValue == BIntegerValue;
				return;
			}
			if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AIntegerValue != BIntegerValue;
				return;
			}
			if (Operator == TEXT(">"))
			{
				bSuccess = true;
				bResult = AIntegerValue > BIntegerValue;
				return;
			}
			if (Operator == TEXT("<"))
			{
				bSuccess = true;
				bResult = AIntegerValue < BIntegerValue;
				return;
			}
			if (Operator == TEXT(">="))
			{
				bSuccess = true;
				bResult = AIntegerValue >= BIntegerValue;
				return;
			}
			if (Operator == TEXT("<="))
			{
				bSuccess = true;
				bResult = AIntegerValue <= BIntegerValue;
				return;
			}
			ErrorMessage = TEXT("Invalid Operator");
			return;
		}
	case EShidenVariableType::Float:
		{
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AFloatValue == BFloatValue;
				return;
			}
			if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AFloatValue != BFloatValue;
				return;
			}
			if (Operator == TEXT(">"))
			{
				bSuccess = true;
				bResult = AFloatValue > BFloatValue;
				return;
			}
			if (Operator == TEXT("<"))
			{
				bSuccess = true;
				bResult = AFloatValue < BFloatValue;
				return;
			}
			if (Operator == TEXT(">="))
			{
				bSuccess = true;
				bResult = AFloatValue >= BFloatValue;
				return;
			}
			if (Operator == TEXT("<="))
			{
				bSuccess = true;
				bResult = AFloatValue <= BFloatValue;
				return;
			}
			ErrorMessage = TEXT("Invalid Operator");
			return;
		}
	case EShidenVariableType::Vector2:
		{
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AVector2Value == BVector2Value;
				return;
			}
			if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AVector2Value != BVector2Value;
				return;
			}
			ErrorMessage = TEXT("Invalid Operator");
			return;
		}
	case EShidenVariableType::Vector3:
		{
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AVector3Value == BVector3Value;
				return;
			}
			if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AVector3Value != BVector3Value;
				return;
			}
			ErrorMessage = TEXT("Invalid Operator");
			return;
		}
	}
}
