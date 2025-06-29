// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "AudioDevice.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

SHIDENCORE_API FString UShidenVariableBlueprintLibrary::MakeUpdateErrorMessage(const TObjectPtr<UShidenSubsystem> ShidenSubsystem,
                                                                               const FString& Name, const EShidenVariableType& Type)
{
	FShidenVariableDefinition Definition;
	if (!ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition))
	{
		return FString::Printf(TEXT("Variable %s is not defined."), *Name);
	}
	if (Definition.bIsReadOnly)
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

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateUserBoolean(const FString& Name, const bool bValue, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->UserVariable.TryUpdate(Name, bValue))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateUserInteger(const FString& Name, const int32 Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->UserVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateUserFloat(const FString& Name, const float Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->UserVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateUserString(const FString& Name, const FString& Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->UserVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateUserVector3(const FString& Name, const FVector& Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->UserVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateUserVector2(const FString& Name, const FVector2D& Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->UserVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
		return false;
	}
	return true;
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::GetUserVariableNames(TArray<FString>& Names)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->UserVariable.GetNames(Names);
}

SHIDENCORE_API int32 UShidenVariableBlueprintLibrary::UserVariableNum()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->UserVariable.Num();
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindUserVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetUserVariable(const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->UserVariable.TryReset(Name);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindUserVariable(const FString& Name, EShidenVariableType& VariableType,
                                                                      bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
                                                                      float& FloatValue,
                                                                      FVector2D& Vector2Value, FVector& Vector3Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	if (!ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition))
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return false;
	}

	VariableType = Definition.Type;
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
	return true;
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetUserVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	ShidenSubsystem->UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindUserVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->UserVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateSystemBoolean(const FString& Name, const bool bValue, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->SystemVariable.TryUpdate(Name, bValue))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateSystemInteger(const FString& Name, const int32 Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->SystemVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateSystemFloat(const FString& Name, const float Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->SystemVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateSystemString(const FString& Name, const FString& Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->SystemVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateSystemVector2(const FString& Name, const FVector2D& Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->SystemVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
		return false;
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateSystemVector3(const FString& Name, const FVector& Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (!ShidenSubsystem->SystemVariable.TryUpdate(Name, Value))
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
		return false;
	}
	return true;
}

bool UShidenVariableBlueprintLibrary::TryFindSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::GetSystemVariableNames(TArray<FString>& Names)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->SystemVariable.GetNames(Names);
}

SHIDENCORE_API int32 UShidenVariableBlueprintLibrary::SystemVariableNum()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->SystemVariable.Num();
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetSystemVariable(const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->SystemVariable.TryReset(Name);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindSystemVariable(const FString& Name, EShidenVariableType& VariableType,
                                                                        bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
                                                                        float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value,
                                                                        FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	if (!ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition))
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return false;
	}

	VariableType = Definition.Type;
	switch (VariableType)
	{
	case EShidenVariableType::Boolean:
		return ShidenSubsystem->SystemVariable.TryGet(Name, bBooleanValue);
	case EShidenVariableType::Integer:
		return ShidenSubsystem->SystemVariable.TryGet(Name, IntegerValue);
	case EShidenVariableType::Float:
		return ShidenSubsystem->SystemVariable.TryGet(Name, FloatValue);
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		return ShidenSubsystem->SystemVariable.TryGet(Name, StringValue);
	case EShidenVariableType::Vector2:
		return ShidenSubsystem->SystemVariable.TryGet(Name, Vector2Value);
	case EShidenVariableType::Vector3:
		return ShidenSubsystem->SystemVariable.TryGet(Name, Vector3Value);
	}
	
	return false;
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetSystemVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	ShidenSubsystem->SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->SystemVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::InitLocalVariable(const FString& ProcessName, const UShidenScenario* Scenario,
                                                                       const TMap<FString, FString>& Arguments)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.InitLocalVariable(ScopeKey, Scenario);
		for (const FShidenVariableDefinition& Definition : Scenario->MacroParameterDefinitions)
		{
			if (!Arguments.Contains(Definition.Name))
			{
				continue;
			}

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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetLocalVariable(const FString& ProcessName, const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	
	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.TryReset(ScopeKey, Name);
	}
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateLocalBoolean(const FString& ProcessName, const FString& Name, const bool bValue,
                                                                           FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		if (!ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, bValue))
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
			return false;
		}
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateLocalInteger(const FString& ProcessName, const FString& Name, const int32 Value,
                                                                           FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		if (!ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value))
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
			return false;
		}
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateLocalFloat(const FString& ProcessName, const FString& Name, const float Value,
                                                                         FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		if (!ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value))
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
			return false;
		}
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateLocalString(const FString& ProcessName, const FString& Name, const FString& Value,
                                                                          FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		if (!ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value))
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
			return false;
		}
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateLocalVector2(const FString& ProcessName, const FString& Name, const FVector2D& Value,
                                                                           FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		if (!ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value))
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
			return false;
		}
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateLocalVector3(const FString& ProcessName, const FString& Name, const FVector& Value,
                                                                           FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		if (!ShidenSubsystem->LocalVariable.TryUpdate(ScopeKey, Name, Value))
		{
			ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
			return false;
		}
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
	return false;
}

bool UShidenVariableBlueprintLibrary::TryFindLocalVariableDefinition(const FString& ProcessName, const FString& Name,
                                                                     FShidenVariableDefinition& Definition)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	
	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		return ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition);
	}
	return false;
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::RemoveLocalVariablesInCurrentScope(const FString& ProcessName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.RemoveVariablesInCurrentScope(ScopeKey);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::RemoveLocalVariablesInProcess(const FString& ProcessName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->LocalVariable.RemoveVariablesInProcess(ProcessName);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetAllLocalVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->LocalVariable.ResetAll();
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindLocalVariable(const FString& ProcessName, const FString& Name,
                                                                          EShidenVariableType& VariableType,
                                                                          bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
                                                                          float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value,
                                                                          FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FString ScopeKey;
	if (!TryCreateScopeKey(ProcessName, ScopeKey))
	{
		return false;
	}

	FShidenVariableDefinition Definition;
	if (!ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition))
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return false;
	}

	VariableType = Definition.Type;
	
	switch (VariableType)
	{
	case EShidenVariableType::Boolean:
		return ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, bBooleanValue);
	case EShidenVariableType::Integer:
		return ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, IntegerValue);
	case EShidenVariableType::Float:
		return ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, FloatValue);
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		return ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, StringValue);
	case EShidenVariableType::Vector2:
		return ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, Vector2Value);
	case EShidenVariableType::Vector3:
		return ShidenSubsystem->LocalVariable.TryGet(ScopeKey, Name, Vector3Value);
	}

	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindLocalVariableAsString(const FString& ProcessName, const FString& Name,
                                                                               EShidenVariableType& OriginalType, FString& Result)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	FString ScopeKey;
	return TryCreateScopeKey(ProcessName, ScopeKey)
		&& ShidenSubsystem->LocalVariable.TryGetAsString(ScopeKey, Name, OriginalType, Result);
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(
	const UObject* WorldContextObject, const FString& Name, const FString& Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (!ShidenSubsystem->PredefinedSystemVariable.TryUpdateByString(Name, Value))
	{
		return false;
	}

	if (Name.Compare(TEXT("MasterVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.MasterVolume);
	}
	else if (Name.Compare(TEXT("BGMVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.BGMVolume);
	}
	else if (Name.Compare(TEXT("SEVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.SEVolume);
	}
	else if (Name.Compare(TEXT("VoiceVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.VoiceVolume);
	}

	return true;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindPredefinedSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType,
                                                                                             FString& Result)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->PredefinedSystemVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetMasterVolume(const UObject* WorldContextObject, const float Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.MasterVolume != Value)
	{
		ShidenSubsystem->PredefinedSystemVariable.MasterVolume = Value;
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(), Value);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetBGMVolume(const UObject* WorldContextObject, const float Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.BGMVolume != Value)
	{
		ShidenSubsystem->PredefinedSystemVariable.BGMVolume = Value;
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(), Value);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetSEVolume(const UObject* WorldContextObject, const float Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.SEVolume != Value)
	{
		ShidenSubsystem->PredefinedSystemVariable.SEVolume = Value;
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetSESoundClass(), Value);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetVoiceVolume(const UObject* WorldContextObject, const float Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.VoiceVolume != Value)
	{
		ShidenSubsystem->PredefinedSystemVariable.VoiceVolume = Value;
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(), Value);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ApplyVolumeRate(const UObject* WorldContextObject, USoundMix* TargetSoundMix,
                                                                     USoundClass* TargetSoundClass, const float TargetVolumeRate)
{
	if (!GEngine || !GEngine->UseSound())
	{
		return;
	}

	if (!TargetSoundMix || !TargetSoundClass)
	{
		return;
	}

	const TObjectPtr<UWorld> ThisWorld = WorldContextObject->GetWorld();
	if (!ThisWorld || !ThisWorld->bAllowAudioPlayback)
	{
		return;
	}

	if (FAudioDeviceHandle AudioDevice = ThisWorld->GetAudioDevice())
	{
		AudioDevice->SetSoundMixClassOverride(TargetSoundMix, TargetSoundClass, TargetVolumeRate, 1.0, 0.0, true);
		AudioDevice->PushSoundMixModifier(TargetSoundMix);
	}
}

FRegexPattern& UShidenVariableBlueprintLibrary::GetReplaceTextPattern()
{
	static FRegexPattern TextReplacePattern = FRegexPattern(TEXT("((?<!\\\\)\\{ *[^ \\}]+ *(?<!\\\\)\\})"));
	return TextReplacePattern;
}

FRegexPattern& UShidenVariableBlueprintLibrary::GetVariablePattern()
{
	static FRegexPattern VariablePattern = FRegexPattern(TEXT("(?<!\\\\)\\{\\s*([^ \\}]+?)\\s*(?<!\\\\)\\}"));
	return VariablePattern;
}

SHIDENCORE_API TArray<FString> UShidenVariableBlueprintLibrary::GetVariableNamesFromText(const FString& Text)
{
	if (!Text.Contains(TEXT("{")) || !Text.Contains(TEXT("}")))
	{
		return TArray<FString>();
	}

	TArray<FString> Result;
	FRegexMatcher Matcher(GetVariablePattern(), Text);
	while (Matcher.FindNext())
	{
		Result.AddUnique(Matcher.GetCaptureGroup(1));
	}

	return Result;
}

SHIDENCORE_API FString UShidenVariableBlueprintLibrary::ReplaceVariables(const FString& ProcessName, const FString& Text)
{
	if (!Text.Contains(TEXT("{")) || !Text.Contains(TEXT("}")))
	{
		return Text;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FString ResultText = Text;
	FRegexMatcher Matcher(GetReplaceTextPattern(), Text);
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
			if (!TryCreateScopeKey(ProcessName, ScopeKey)
				|| !ShidenSubsystem->LocalVariable.TryGetAsString(ScopeKey, VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText))
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("Predefined"))
		{
			EShidenVariableType Type;
			if (!TryFindPredefinedSystemVariableAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText))
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

SHIDENCORE_API FShidenCommand UShidenVariableBlueprintLibrary::ReplaceVariablesForLoad(FShidenVariable TempLocalVariable,
                                                                                       const FShidenCommand& Command)
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

		FRegexMatcher Matcher(GetReplaceTextPattern(), Value);

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
				if (ShidenSubsystem->UserVariable.TryGetDefinition(TempKey, Definition) && Definition.bIsReadOnly)
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
				if (ShidenSubsystem->SystemVariable.TryGetDefinition(TempKey, Definition) && Definition.bIsReadOnly)
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

SHIDENCORE_API FShidenCommand UShidenVariableBlueprintLibrary::ReplaceAllVariable(const FString& ProcessName, const FShidenCommand& Command)
{
	FShidenCommand Result = Command;
	Result.Args.Empty();
	TArray<FString> Keys;
	Command.Args.GetKeys(Keys);
	for (const FString& Key : Keys)
	{
		const FString Value = Command.Args.FindRef(Key);
		if (Value == TEXT("{EMPTY}"))
		{
			continue;
		}
		const FString NewValue = ReplaceVariables(ProcessName, Value);
		Result.Args.Add(Key, NewValue);
	}
	return Result;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryConvertToVariableKind(const FString& VariableKind, EShidenVariableKind& Result)
{
	const FString CleanedVariableKind = VariableKind.Replace(TEXT(" "), TEXT(""));

	if (CleanedVariableKind.Equals(TEXT("UserVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::UserVariable;
		return true;
	}
	if (CleanedVariableKind.Equals(TEXT("LocalVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::LocalVariable;
		return true;
	}
	if (CleanedVariableKind.Equals(TEXT("SystemVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::SystemVariable;
		return true;
	}
	if (CleanedVariableKind.Equals(TEXT("PredefinedSystemVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::PredefinedSystemVariable;
		return true;
	}
	Result = EShidenVariableKind::UserVariable;
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryConvertToVariableType(const FString& VariableType, EShidenVariableType& Result)
{
	const FString CleanedVariableType = VariableType.Replace(TEXT(" "), TEXT(""));

	if (CleanedVariableType.Equals(TEXT("Boolean"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Boolean;
		return true;
	}
	if (CleanedVariableType.Equals(TEXT("String"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::String;
		return true;
	}
	if (CleanedVariableType.Equals(TEXT("Integer"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Integer;
		return true;
	}
	if (CleanedVariableType.Equals(TEXT("Float"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Float;
		return true;
	}
	if (CleanedVariableType.Equals(TEXT("Vector2"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Vector2;
		return true;
	}
	if (CleanedVariableType.Equals(TEXT("Vector3"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Vector3;
		return true;
	}
	if (CleanedVariableType.Equals(TEXT("AssetPath"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::AssetPath;
		return true;
	}
	
	Result = EShidenVariableType::Boolean;
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryConvertToAssetPathType(const FString& AssetPathType, EShidenAssetPathType& Result)
{
	const FString CleanedAssetPathType = AssetPathType.Replace(TEXT(" "), TEXT(""));

	if (CleanedAssetPathType.Equals(TEXT("Any"), ESearchCase::IgnoreCase)
		|| CleanedAssetPathType.Equals(TEXT("AssetPath"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::Any;
		return true;
	}
	if (CleanedAssetPathType.Equals(TEXT("SlateBrush"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::SlateBrush;
		return true;
	}
	if (CleanedAssetPathType.Equals(TEXT("SoundBase"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::SoundBase;
		return true;
	}
	if (CleanedAssetPathType.Equals(TEXT("MediaSource"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::MediaSource;
		return true;
	}
	if (CleanedAssetPathType.Equals(TEXT("ForceFeedbackEffect"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::ForceFeedbackEffect;
		return true;
	}
	if (CleanedAssetPathType.Equals(TEXT("Texture"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::Texture;
		return true;
	}

	Result = EShidenAssetPathType::None;
	return false;
}

bool UShidenVariableBlueprintLibrary::TryFindVariableDefinition(const FString& ProcessName, const EShidenVariableKind Kind,
                                                                const FString& Name, FShidenVariableDefinition& Definition, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	bool bSuccess = false;
	
	switch (Kind)
	{
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
			ErrorMessage = FString::Printf(TEXT("Failed to create scope key for process name \"%s\"."), *ProcessName);
			return false;
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
	return bSuccess;
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetPredefinedSystemVariables(const UObject* WorldContextObject)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->PredefinedSystemVariable = ShidenProjectConfig->PredefinedSystemVariable;

	ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(),
	                ShidenSubsystem->PredefinedSystemVariable.MasterVolume);
	ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
	                ShidenSubsystem->PredefinedSystemVariable.BGMVolume);
	ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetSESoundClass(),
	                ShidenSubsystem->PredefinedSystemVariable.SEVolume);
	ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(),
	                ShidenSubsystem->PredefinedSystemVariable.VoiceVolume);
}

SHIDENCORE_API FShidenPredefinedSystemVariable UShidenVariableBlueprintLibrary::GetPredefinedSystemVariable()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->PredefinedSystemVariable;
}

bool UShidenVariableBlueprintLibrary::TryFindPredefinedSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	
	FShidenPredefinedSystemVariableDefinition PredefinedSystemVariableDefinition;
	if (ShidenSubsystem->PredefinedSystemVariable.TryGetDefinition(Name, PredefinedSystemVariableDefinition))
	{
		Definition = static_cast<FShidenVariableDefinition>(PredefinedSystemVariableDefinition);
		return true;
	}
	return false;
}

bool UShidenVariableBlueprintLibrary::TryUpdatePredefinedVariable(const UObject* WorldContextObject, const EShidenVariableType& Type,
                                                               const FString& Name, const bool bBooleanValue, const FString& StringValue, const int32 IntegerValue,
                                                               const float FloatValue, FString& ErrorMessage)
{
	ErrorMessage = TEXT("");

	if (Name.IsEmpty())
	{
		ErrorMessage = TEXT("Invalid Key Name");
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenPredefinedSystemVariableDefinition Definition;

	if (!ShidenSubsystem->PredefinedSystemVariable.TryGetDefinition(Name, Definition))
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return false;
	}
	if (Definition.bIsReadOnly)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is read only."), *Name);
		return false;
	}
	if (Definition.Type != Type)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not %s."), *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
		return false;
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
		return false;
	}

	if (!ShidenSubsystem->PredefinedSystemVariable.TryUpdateByString(Name, Value))
	{
		ErrorMessage = TEXT("Unknown error.");
		return false;
	}

	if (Name.Compare(TEXT("MasterVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.MasterVolume);
	}
	else if (Name.Compare(TEXT("BGMVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.BGMVolume);
	}
	else if (Name.Compare(TEXT("SEVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.SEVolume);
	}
	else if (Name.Compare(TEXT("VoiceVolumeRate"), ESearchCase::IgnoreCase) == 0)
	{
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(),
		                ShidenSubsystem->PredefinedSystemVariable.VoiceVolume);
	}
	return true;
}

SHIDENCORE_API bool TryUpdateLocalVariable(const FString& ProcessName, const EShidenVariableType& Type, const FString& Name,
                                           const bool bBooleanValue, const FString& StringValue, const int32 IntegerValue,
                                           const float FloatValue, const FVector2D& Vector2Value, const FVector& Vector3Value,
                                           FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalBoolean(ProcessName, Name, bBooleanValue, ErrorMessage);
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalString(ProcessName, Name, StringValue, ErrorMessage);
		}
	case EShidenVariableType::Integer:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalInteger(ProcessName, Name, IntegerValue, ErrorMessage);
		}
	case EShidenVariableType::Float:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalFloat(ProcessName, Name, FloatValue, ErrorMessage);
		}
	case EShidenVariableType::Vector2:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalVector2(ProcessName, Name, Vector2Value, ErrorMessage);
		}
	case EShidenVariableType::Vector3:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalVector3(ProcessName, Name, Vector3Value, ErrorMessage);
		}
	}

	return false;
}

SHIDENCORE_API bool TryUpdateSystemVariable(const EShidenVariableType& Type, const FString& Name, const bool bBooleanValue,
                                         const FString& StringValue, const int32 IntegerValue, const float FloatValue,
                                         const FVector2D& Vector2Value, const FVector& Vector3Value, FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateSystemBoolean(Name, bBooleanValue, ErrorMessage);
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateSystemString(Name, StringValue, ErrorMessage);
		}
	case EShidenVariableType::Integer:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateSystemInteger(Name, IntegerValue, ErrorMessage);
		}
	case EShidenVariableType::Float:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateSystemFloat(Name, FloatValue, ErrorMessage);
		}
	case EShidenVariableType::Vector2:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateSystemVector2(Name, Vector2Value, ErrorMessage);
		}
	case EShidenVariableType::Vector3:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateSystemVector3(Name, Vector3Value, ErrorMessage);
		}
	}

	return false;
}

SHIDENCORE_API bool TryUpdateUserVariable(const EShidenVariableType& Type, const FString& Name, const bool bBooleanValue,
                                          const FString& StringValue, const int32 IntegerValue, const float FloatValue,
                                          const FVector2D& Vector2Value, const FVector& Vector3Value, FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateUserBoolean(Name, bBooleanValue, ErrorMessage);
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateUserString(Name, StringValue, ErrorMessage);
		}
	case EShidenVariableType::Integer:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateUserInteger(Name, IntegerValue, ErrorMessage);
		}
	case EShidenVariableType::Float:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateUserFloat(Name, FloatValue, ErrorMessage);
		}
	case EShidenVariableType::Vector2:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateUserVector2(Name, Vector2Value, ErrorMessage);
		}
	case EShidenVariableType::Vector3:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateUserVector3(Name, Vector3Value, ErrorMessage);
		}
	}

	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryUpdateVariable(const UObject* WorldContextObject, const FString& ProcessName,
                                                                       const EShidenVariableKind Kind, const EShidenVariableType Type,
                                                                       const FString& Name, const bool bBooleanValue, const FString& StringValue,
                                                                       const int32 IntegerValue, const float FloatValue,
                                                                       const FVector2D& Vector2Value, const FVector& Vector3Value,
                                                                       FString& ErrorMessage)
{
	switch (Kind)
	{
	case EShidenVariableKind::UserVariable:
		return TryUpdateUserVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, ErrorMessage);
	case EShidenVariableKind::LocalVariable:
		return TryUpdateLocalVariable(ProcessName, Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, ErrorMessage);
	case EShidenVariableKind::SystemVariable:
		return TryUpdateSystemVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, ErrorMessage);
	case EShidenVariableKind::PredefinedSystemVariable:
		return TryUpdatePredefinedVariable(WorldContextObject, Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, ErrorMessage);
	}
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryFindVariable(const FString& ProcessName,
                                                                     const EShidenVariableKind Kind, const FString& Name,
                                                                     EShidenVariableType& VariableType, bool& bBooleanValue,
                                                                     FString& StringValue, int& IntegerValue,
                                                                     float& FloatValue, FVector2D& Vector2Value,
                                                                     FVector& Vector3Value, FString& ErrorMessage)
{
	switch (Kind)
	{
	case EShidenVariableKind::UserVariable:
		return TryFindUserVariable(Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value,
		                 Vector3Value, ErrorMessage);
	case EShidenVariableKind::LocalVariable:
		return TryFindLocalVariable(ProcessName, Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue,
		                  Vector2Value, Vector3Value, ErrorMessage);
	case EShidenVariableKind::SystemVariable:
		return TryFindSystemVariable(Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value,
		                   Vector3Value, ErrorMessage);
	case EShidenVariableKind::PredefinedSystemVariable:
		{
			const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
			check(ShidenSubsystem);

			FString Value;
			if (ShidenSubsystem->PredefinedSystemVariable.TryGetAsString(Name, VariableType, Value))
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
					return false;
				}
				return true;
			}

			ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		}
	}
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryEvaluateBoolean(const FString& Operator, const bool bABooleanValue, const bool bBBooleanValue,
                                                                     bool& bResult, FString& ErrorMessage)
{
	if (Operator == TEXT("=="))
	{
		bResult = bABooleanValue == bBBooleanValue;
		return true;
	}
	if (Operator == TEXT("!="))
	{
		bResult = bABooleanValue != bBBooleanValue;
		return true;
	}
	ErrorMessage = TEXT("Invalid Operator");
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryEvaluateString(const FString& Operator, const FString& AStringValue, const FString& BStringValue,
                                                                    bool& bResult, FString& ErrorMessage)
{
	if (Operator == TEXT("=="))
	{
		bResult = AStringValue.Equals(BStringValue, ESearchCase::IgnoreCase);
		return true;
	}
	if (Operator == TEXT("!="))
	{
		bResult = !AStringValue.Equals(BStringValue, ESearchCase::IgnoreCase);
		return true;
	}
	ErrorMessage = TEXT("Invalid Operator");
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryEvaluateInteger(const FString& Operator, const int32 AIntegerValue, const int32 BIntegerValue,
                                                                     bool& bResult, FString& ErrorMessage)
{
	if (Operator == TEXT("=="))
	{
		bResult = AIntegerValue == BIntegerValue;
		return true;
	}
	if (Operator == TEXT("!="))
	{
		bResult = AIntegerValue != BIntegerValue;
		return true;
	}
	if (Operator == TEXT(">"))
	{
		bResult = AIntegerValue > BIntegerValue;
		return true;
	}
	if (Operator == TEXT("<"))
	{
		bResult = AIntegerValue < BIntegerValue;
		return true;
	}
	if (Operator == TEXT(">="))
	{
		bResult = AIntegerValue >= BIntegerValue;
		return true;
	}
	if (Operator == TEXT("<="))
	{
		bResult = AIntegerValue <= BIntegerValue;
		return true;
	}
	ErrorMessage = TEXT("Invalid Operator");
	return false;
}

SHIDENCORE_API bool UShidenVariableBlueprintLibrary::TryEvaluateFloat(const FString& Operator, const float AFloatValue, const float BFloatValue,
                                                                   bool& bResult, FString& ErrorMessage)
{
	if (Operator == TEXT("=="))
	{
		bResult = AFloatValue == BFloatValue;
		return true;
	}
	if (Operator == TEXT("!="))
	{
		bResult = AFloatValue != BFloatValue;
		return true;
	}
	if (Operator == TEXT(">"))
	{
		bResult = AFloatValue > BFloatValue;
		return true;
	}
	if (Operator == TEXT("<"))
	{
		bResult = AFloatValue < BFloatValue;
		return true;
	}
	if (Operator == TEXT(">="))
	{
		bResult = AFloatValue >= BFloatValue;
		return true;
	}
	if (Operator == TEXT("<="))
	{
		bResult = AFloatValue <= BFloatValue;
		return true;
	}
	ErrorMessage = TEXT("Invalid Operator");
	return false;
}

bool UShidenVariableBlueprintLibrary::TryEvaluateVector2(const FString& Operator, const FVector2D& AVector2Value, const FVector2D& BVector2Value,
                                                      bool& bResult, FString& ErrorMessage)
{
	if (Operator == TEXT("=="))
	{
		bResult = AVector2Value == BVector2Value;
		return true;
	}
	if (Operator == TEXT("!="))
	{
		bResult = AVector2Value != BVector2Value;
		return true;
	}
	ErrorMessage = TEXT("Invalid Operator");
	return false;
}

bool UShidenVariableBlueprintLibrary::TryEvaluateVector3(const FString& Operator, const FVector& AVector3Value,
                                                                     const FVector& BVector3Value, bool& bResult, FString& ErrorMessage)
{
	if (Operator == TEXT("=="))
	{
		bResult = AVector3Value == BVector3Value;
		return true;
	}
	if (Operator == TEXT("!="))
	{
		bResult = AVector3Value != BVector3Value;
		return true;
	}
	ErrorMessage = TEXT("Invalid Operator");
	return false;
}

bool UShidenVariableBlueprintLibrary::TryEvaluateCondition(const EShidenVariableType Type, const FString& Operator, const bool bABooleanValue,
                                                        const FString& AStringValue, const int32 AIntegerValue, const float AFloatValue,
                                                        const FVector2D& AVector2Value, const FVector& AVector3Value, const bool bBBooleanValue,
                                                        const FString& BStringValue, const int32 BIntegerValue, const float BFloatValue,
                                                        const FVector2D& BVector2Value, const FVector& BVector3Value, bool& bResult,
                                                        FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		return TryEvaluateBoolean(Operator, bABooleanValue, bBBooleanValue, bResult, ErrorMessage);
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		return TryEvaluateString(Operator, AStringValue, BStringValue, bResult, ErrorMessage);
	case EShidenVariableType::Integer:
		return TryEvaluateInteger(Operator, AIntegerValue, BIntegerValue, bResult, ErrorMessage);
	case EShidenVariableType::Float:
		return TryEvaluateFloat(Operator, AFloatValue, BFloatValue, bResult, ErrorMessage);
	case EShidenVariableType::Vector2:
		return TryEvaluateVector2(Operator, AVector2Value, BVector2Value, bResult, ErrorMessage);
	case EShidenVariableType::Vector3:
		return TryEvaluateVector3(Operator, AVector3Value, BVector3Value, bResult, ErrorMessage);
	}
	return false;
}
