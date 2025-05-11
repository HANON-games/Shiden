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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateUserBoolean(const FString& Name, const bool bValue, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, bValue);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateUserInteger(const FString& Name, const int32 Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateUserFloat(const FString& Name, const float Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateUserString(const FString& Name, const FString& Value, bool& bSuccess,
                                                                      FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateUserVector3(const FString& Name, const FVector& Value, bool& bSuccess,
                                                                       FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateUserVector2(const FString& Name, const FVector2D& Value, bool& bSuccess,
                                                                       FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
	}
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindUserVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition,
                                                                                bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetUserVariable(const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->UserVariable.TryReset(Name);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindUserVariable(const FString& Name, EShidenVariableType& VariableType,
                                                                      bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
                                                                      float& FloatValue,
                                                                      FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess,
                                                                      FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	bSuccess = ShidenSubsystem->UserVariable.TryGetDefinition(Name, Definition);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return;
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
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetUserVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	ShidenSubsystem->UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindUserVariableAsString(const FString& Name, EShidenVariableType& OriginalType, FString& Result,
                                                                              bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->UserVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateSystemBoolean(const FString& Name, const bool bValue, bool& bSuccess,
                                                                         FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, bValue);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Boolean);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateSystemInteger(const FString& Name, const int32 Value, bool& bSuccess,
                                                                         FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Integer);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateSystemFloat(const FString& Name, const float Value, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Float);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateSystemString(const FString& Name, const FString& Value, bool& bSuccess,
                                                                        FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::String);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateSystemVector2(const FString& Name, const FVector2D& Value, bool& bSuccess,
                                                                         FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector2);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateSystemVector3(const FString& Name, const FVector& Value, bool& bSuccess,
                                                                         FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryUpdate(Name, Value);
	if (!bSuccess)
	{
		ErrorMessage = MakeUpdateErrorMessage(ShidenSubsystem, Name, EShidenVariableType::Vector3);
	}
}

void UShidenVariableBlueprintLibrary::FindSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition);
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindSystemVariable(const FString& Name, EShidenVariableType& VariableType,
                                                                        bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
                                                                        float& FloatValue,
                                                                        FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess,
                                                                        FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenVariableDefinition Definition;
	bSuccess = ShidenSubsystem->SystemVariable.TryGetDefinition(Name, Definition);
	VariableType = Definition.Type;

	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return;
	}

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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetSystemVariables()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	ShidenSubsystem->SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType,
                                                                                FString& Result, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->SystemVariable.TryGetAsString(Name, OriginalType, Result);
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ResetLocalVariable(const FString& ProcessName, const FString& Name)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		ShidenSubsystem->LocalVariable.TryReset(ScopeKey, Name);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateLocalBoolean(const FString& ProcessName, const FString& Name, const bool bValue,
                                                                        bool& bSuccess, FString& ErrorMessage)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateLocalInteger(const FString& ProcessName, const FString& Name, const int32 Value,
                                                                        bool& bSuccess, FString& ErrorMessage)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateLocalFloat(const FString& ProcessName, const FString& Name, const float Value,
                                                                      bool& bSuccess, FString& ErrorMessage)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateLocalString(const FString& ProcessName, const FString& Name, const FString& Value,
                                                                       bool& bSuccess, FString& ErrorMessage)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateLocalVector2(const FString& ProcessName, const FString& Name, const FVector2D& Value,
                                                                        bool& bSuccess, FString& ErrorMessage)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateLocalVector3(const FString& ProcessName, const FString& Name, const FVector& Value,
                                                                        bool& bSuccess, FString& ErrorMessage)
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

void UShidenVariableBlueprintLibrary::FindLocalVariableDefinition(const FString& ProcessName, const FString& Name,
                                                                  FShidenVariableDefinition& Definition, bool& bSuccess)
{
	bSuccess = false;
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (FString ScopeKey; TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition);
	}
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindLocalVariable(const FString& ProcessName, const FString& Name,
                                                                       EShidenVariableType& VariableType,
                                                                       bool& bBooleanValue, FString& StringValue, int32& IntegerValue,
                                                                       float& FloatValue,
                                                                       FVector2D& Vector2Value, FVector& Vector3Value, bool& bSuccess,
                                                                       FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FString ScopeKey;
	if (!TryCreateScopeKey(ProcessName, ScopeKey))
	{
		bSuccess = false;
		return;
	}

	FShidenVariableDefinition Definition;
	bSuccess = ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, Name, Definition);
	VariableType = Definition.Type;

	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not defined."), *Name);
		return;
	}

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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindLocalVariableAsString(const FString& ProcessName, const FString& Name,
                                                                               EShidenVariableType& OriginalType, FString& Result, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	FString ScopeKey;
	bSuccess = TryCreateScopeKey(ProcessName, ScopeKey)
		&& ShidenSubsystem->LocalVariable.TryGetAsString(ScopeKey, Name, OriginalType, Result);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(
	const UObject* WorldContextObject, const FString& Name, const FString& Value, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	bSuccess = ShidenSubsystem->PredefinedSystemVariable.TryUpdateByString(Name, Value);
	if (!bSuccess)
	{
		return;
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
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindPredefinedSystemVariableAsString(const FString& Name, EShidenVariableType& OriginalType,
                                                                                          FString& Result, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	bSuccess = ShidenSubsystem->PredefinedSystemVariable.TryGetAsString(Name, OriginalType, Result);
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetMasterVolume(const UObject* WorldContextObject, const float InValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.MasterVolume != InValue)
	{
		ShidenSubsystem->PredefinedSystemVariable.MasterVolume = InValue;
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(), InValue);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetBGMVolume(const UObject* WorldContextObject, const float InValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.BGMVolume != InValue)
	{
		ShidenSubsystem->PredefinedSystemVariable.BGMVolume = InValue;
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(), InValue);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetSEVolume(const UObject* WorldContextObject, const float InValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.SEVolume != InValue)
	{
		ShidenSubsystem->PredefinedSystemVariable.SEVolume = InValue;
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetSESoundClass(), InValue);
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::SetVoiceVolume(const UObject* WorldContextObject, const float InValue)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.VoiceVolume != InValue)
	{
		ShidenSubsystem->PredefinedSystemVariable.VoiceVolume = InValue;
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		ApplyVolumeRate(WorldContextObject, ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(), InValue);
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
			bool bReturnValue;
			FindPredefinedSystemVariableAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type, ReplacementText, bReturnValue);
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ConvertToVariableKind(const FString& VariableKind, EShidenVariableKind& Result, bool& bSuccess)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ConvertToVariableType(const FString& VariableType, EShidenVariableType& Result, bool& bSuccess)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::ConvertToAssetPathType(const FString& AssetPathType, EShidenAssetPathType& Result,
                                                                            bool& bSuccess)
{
	const FString CleanedAssetPathType = AssetPathType.Replace(TEXT(" "), TEXT(""));

	if (CleanedAssetPathType.Equals(TEXT("Any"), ESearchCase::IgnoreCase)
		|| CleanedAssetPathType.Equals(TEXT("AssetPath"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::Any;
		bSuccess = true;
		return;
	}
	if (CleanedAssetPathType.Equals(TEXT("SlateBrush"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::SlateBrush;
		bSuccess = true;
		return;
	}
	if (CleanedAssetPathType.Equals(TEXT("SoundBase"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::SoundBase;
		bSuccess = true;
		return;
	}
	if (CleanedAssetPathType.Equals(TEXT("MediaSource"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::MediaSource;
		bSuccess = true;
		return;
	}
	if (CleanedAssetPathType.Equals(TEXT("ForceFeedbackEffect"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::ForceFeedbackEffect;
		bSuccess = true;
		return;
	}
	if (CleanedAssetPathType.Equals(TEXT("Texture"), ESearchCase::IgnoreCase))
	{
		Result = EShidenAssetPathType::Texture;
		bSuccess = true;
		return;
	}

	bSuccess = false;
	Result = EShidenAssetPathType::None;
}

void UShidenVariableBlueprintLibrary::FindVariableDefinition(const FString& ProcessName, const EShidenVariableKind Kind,
                                                             const FString& Name, FShidenVariableDefinition& Definition, bool& bSuccess,
                                                             FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::InitPredefinedSystemVariables(const UObject* WorldContextObject)
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

void UShidenVariableBlueprintLibrary::FindPredefinedSystemVariableDefinition(const FString& Name, FShidenVariableDefinition& Definition,
                                                                             bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	FShidenPredefinedSystemVariableDefinition PredefinedSystemVariableDefinition;
	bSuccess = ShidenSubsystem->PredefinedSystemVariable.TryGetDefinition(Name, PredefinedSystemVariableDefinition);
	if (bSuccess)
	{
		Definition = static_cast<FShidenVariableDefinition>(PredefinedSystemVariableDefinition);
	}
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
	if (Definition.bIsReadOnly)
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
			UShidenVariableBlueprintLibrary::UpdateLocalBoolean(ProcessName, Name, bBooleanValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableBlueprintLibrary::UpdateLocalString(ProcessName, Name, StringValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			UShidenVariableBlueprintLibrary::UpdateLocalInteger(ProcessName, Name, IntegerValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			UShidenVariableBlueprintLibrary::UpdateLocalFloat(ProcessName, Name, FloatValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			UShidenVariableBlueprintLibrary::UpdateLocalVector2(ProcessName, Name, Vector2Value, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			UShidenVariableBlueprintLibrary::UpdateLocalVector3(ProcessName, Name, Vector3Value, bSuccess, ErrorMessage);
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
			UShidenVariableBlueprintLibrary::UpdateSystemBoolean(Name, bBooleanValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableBlueprintLibrary::UpdateSystemString(Name, StringValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			UShidenVariableBlueprintLibrary::UpdateSystemInteger(Name, IntegerValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			UShidenVariableBlueprintLibrary::UpdateSystemFloat(Name, FloatValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			UShidenVariableBlueprintLibrary::UpdateSystemVector2(Name, Vector2Value, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			UShidenVariableBlueprintLibrary::UpdateSystemVector3(Name, Vector3Value, bSuccess, ErrorMessage);
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
			UShidenVariableBlueprintLibrary::UpdateUserBoolean(Name, bBooleanValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableBlueprintLibrary::UpdateUserString(Name, StringValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			UShidenVariableBlueprintLibrary::UpdateUserInteger(Name, IntegerValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			UShidenVariableBlueprintLibrary::UpdateUserFloat(Name, FloatValue, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			UShidenVariableBlueprintLibrary::UpdateUserVector2(Name, Vector2Value, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			UShidenVariableBlueprintLibrary::UpdateUserVector3(Name, Vector3Value, bSuccess, ErrorMessage);
			break;
		}
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::UpdateVariable(const FString& ProcessName,
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
		UpdateLocalVariable(ProcessName, Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess,
		                    ErrorMessage);
		bSuccess = true;
		break;
	case EShidenVariableKind::SystemVariable:
		UpdateSystemVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess, ErrorMessage);
		bSuccess = true;
		break;
	case EShidenVariableKind::PredefinedSystemVariable:
		UpdatePredefinedVariable(Type, Name, bBooleanValue, StringValue, IntegerValue, FloatValue, bSuccess, ErrorMessage);
		break;
	}
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::FindVariable(const FString& ProcessName,
                                                                  const EShidenVariableKind Kind, const FString& Name,
                                                                  EShidenVariableType& VariableType, bool& bBooleanValue,
                                                                  FString& StringValue, int& IntegerValue,
                                                                  float& FloatValue, FVector2D& Vector2Value,
                                                                  FVector& Vector3Value, bool& bSuccess, FString& ErrorMessage)
{
	switch (Kind)
	{
	case EShidenVariableKind::UserVariable:
		FindUserVariable(Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value,
		                 Vector3Value, bSuccess, ErrorMessage);
		break;
	case EShidenVariableKind::LocalVariable:
		FindLocalVariable(ProcessName, Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue,
		                  Vector2Value, Vector3Value, bSuccess, ErrorMessage);
		break;
	case EShidenVariableKind::SystemVariable:
		FindSystemVariable(Name, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value,
		                   Vector3Value, bSuccess, ErrorMessage);
		break;
	case EShidenVariableKind::PredefinedSystemVariable:
		{
			const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

			check(ShidenSubsystem);

			FString Value;
			bSuccess = ShidenSubsystem->PredefinedSystemVariable.TryGetAsString(Name, VariableType, Value);

			if (bSuccess)
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

SHIDENCORE_API void UShidenVariableBlueprintLibrary::EvaluateBoolean(const FString& Operator, const bool bABooleanValue, const bool bBBooleanValue,
                                                                     bool& bResult, bool& bSuccess, FString& ErrorMessage)
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
	bSuccess = false;
	ErrorMessage = TEXT("Invalid Operator");
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::EvaluateString(const FString& Operator, const FString& AStringValue, const FString& BStringValue,
                                                                    bool& bResult, bool& bSuccess, FString& ErrorMessage)
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
	bSuccess = false;
	ErrorMessage = TEXT("Invalid Operator");
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::EvaluateInteger(const FString& Operator, const int32 AIntegerValue, const int32 BIntegerValue,
                                                                     bool& bResult, bool& bSuccess, FString& ErrorMessage)
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
	bSuccess = false;
	ErrorMessage = TEXT("Invalid Operator");
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::EvaluateFloat(const FString& Operator, const float AFloatValue, const float BFloatValue,
                                                                   bool& bResult, bool& bSuccess, FString& ErrorMessage)
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
	bSuccess = false;
	ErrorMessage = TEXT("Invalid Operator");
}

void UShidenVariableBlueprintLibrary::EvaluateVector2(const FString& Operator, const FVector2D& AVector2Value, const FVector2D& BVector2Value,
                                                      bool& bResult, bool& bSuccess, FString& ErrorMessage)
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
	bSuccess = false;
	ErrorMessage = TEXT("Invalid Operator");
}

SHIDENCORE_API void UShidenVariableBlueprintLibrary::EvaluateVector3(const FString& Operator, const FVector& AVector3Value,
                                                                     const FVector& BVector3Value, bool& bResult, bool& bSuccess,
                                                                     FString& ErrorMessage)
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
	bSuccess = false;
	ErrorMessage = TEXT("Invalid Operator");
}

void UShidenVariableBlueprintLibrary::EvaluateCondition(const EShidenVariableType Type, const FString& Operator, const bool bABooleanValue,
                                                        const FString& AStringValue, const int32 AIntegerValue, const float AFloatValue,
                                                        const FVector2D& AVector2Value, const FVector& AVector3Value, const bool bBBooleanValue,
                                                        const FString& BStringValue, const int32 BIntegerValue, const float BFloatValue,
                                                        const FVector2D& BVector2Value, const FVector& BVector3Value, bool& bResult,
                                                        bool& bSuccess, FString& ErrorMessage)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		EvaluateBoolean(Operator, bABooleanValue, bBBooleanValue, bResult, bSuccess, ErrorMessage);
		return;
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		EvaluateString(Operator, AStringValue, BStringValue, bResult, bSuccess, ErrorMessage);
		return;
	case EShidenVariableType::Integer:
		EvaluateInteger(Operator, AIntegerValue, BIntegerValue, bResult, bSuccess, ErrorMessage);
		return;
	case EShidenVariableType::Float:
		EvaluateFloat(Operator, AFloatValue, BFloatValue, bResult, bSuccess, ErrorMessage);
		return;
	case EShidenVariableType::Vector2:
		EvaluateVector2(Operator, AVector2Value, BVector2Value, bResult, bSuccess, ErrorMessage);
		return;
	case EShidenVariableType::Vector3:
		EvaluateVector3(Operator, AVector3Value, BVector3Value, bResult, bSuccess, ErrorMessage);
	}
}
