// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Variable/ShidenLocalVariable.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

SHIDENCORE_API bool FShidenLocalVariable::TryGetDefinition(const FString& ScopeKey, const FString& Name, FShidenVariableDefinition& Definition)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGetDefinition(Name, Definition);
	}
	return false;
}


SHIDENCORE_API void FShidenLocalVariable::InitLocalVariable(const FString& ScopeKey, const UShidenScenario* Scenario)
{
	if (Scenario)
	{
		TArray<FShidenVariableDefinition> Definitions = static_cast<TArray<FShidenVariableDefinition>>(Scenario->MacroParameterDefinitions);
		Definitions.Append(Scenario->LocalVariableDefinitions);
		Variables.Add(ScopeKey, FShidenVariable(Definitions));
		ScenarioIds.Add(ScopeKey, Scenario->ScenarioId);
	}
}

SHIDENCORE_API void FShidenLocalVariable::UpdateVariableDefinitions()
{
	TArray<FString> ScopeKeys;
	Variables.GetKeys(ScopeKeys);
	for (const FString& ScopeKey : ScopeKeys)
	{
		UShidenScenario* Scenario = nullptr;
		bool bSuccess;
		UShidenScenarioBlueprintLibrary::GetScenarioFromCache(ScenarioIds[ScopeKey], Scenario, bSuccess);
		if (bSuccess)
		{
			TArray<FShidenVariableDefinition> Definitions = static_cast<TArray<FShidenVariableDefinition>>(Scenario->MacroParameterDefinitions);
			Definitions.Append(Scenario->LocalVariableDefinitions);
			Variables[ScopeKey].UpdateVariableDefinitions(Definitions);
		}
	}
}

SHIDENCORE_API bool FShidenLocalVariable::TryUpdate(const FString& ScopeKey, const FString& Name, const bool Value,
                                                    const bool bForceUpdateReadOnly /*= false*/)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryUpdate(Name, Value, bForceUpdateReadOnly);
	}
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryUpdate(const FString& ScopeKey, const FString& Name, const int32 Value,
                                                    const bool bForceUpdateReadOnly /*= false*/)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryUpdate(Name, Value, bForceUpdateReadOnly);
	}
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryUpdate(const FString& ScopeKey, const FString& Name, const float Value,
                                                    const bool bForceUpdateReadOnly /*= false*/)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryUpdate(Name, Value, bForceUpdateReadOnly);
	}
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryUpdate(const FString& ScopeKey, const FString& Name, const FString& Value,
                                                    const bool bForceUpdateReadOnly /*= false*/)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryUpdate(Name, Value, bForceUpdateReadOnly);
	}
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryUpdate(const FString& ScopeKey, const FString& Name, const FVector& Value,
                                                    const bool bForceUpdateReadOnly /*= false*/)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryUpdate(Name, Value, bForceUpdateReadOnly);
	}
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryUpdate(const FString& ScopeKey, const FString& Name, const FVector2D& Value,
                                                    const bool bForceUpdateReadOnly /*= false*/)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryUpdate(Name, Value, bForceUpdateReadOnly);
	}
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::Contains(const FString& ScopeKey, const FString& Name) const
{
	return IsValidScope(ScopeKey) && Variables[ScopeKey].Contains(Name);
}

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, bool& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = false;
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, int32& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = 0;
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, float& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = 0;
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, FString& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = FString();
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, FVector& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = FVector();
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, FVector2d& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = FVector2d();
	return false;
}

SHIDENCORE_API bool FShidenLocalVariable::TryGetAsString(const FString& ScopeKey, const FString& Name, EShidenVariableType& Type, FString& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGetAsString(Name, Type, Value);
	}
	Value = FString();
	return false;
}

SHIDENCORE_API int32 FShidenLocalVariable::Num() const
{
	int32 Count = 0;
	TArray<FString> ScopeKeys;
	Variables.GetKeys(ScopeKeys);
	for (const FString& ScopeKey : ScopeKeys)
	{
		TArray<FString> VariableKeys;

		Count += Variables[ScopeKey].Num();
	}
	return Count;
}

SHIDENCORE_API int32 FShidenLocalVariable::Num(const FString& ScopeKey) const
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].Num();
	}
	return 0;
}

SHIDENCORE_API void FShidenLocalVariable::ResetAll()
{
	Variables.Empty();
}

SHIDENCORE_API bool FShidenLocalVariable::TryReset(const FString& ScopeKey, const FString& Name)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryReset(Name);
	}
	return false;
}

SHIDENCORE_API void FShidenLocalVariable::RemoveVariablesInCurrentScope(const FString& ScopeKey)
{
	Variables.Remove(ScopeKey);
}

SHIDENCORE_API void FShidenLocalVariable::RemoveVariablesInProcess(const FString& ProcessName)
{
	FString SearchKey = ProcessName + TEXT("$");

	TArray<FString> Keys;
	Variables.GetKeys(Keys);

	for (const FString& Key : Keys)
	{
		if (Key.StartsWith(ProcessName, ESearchCase::CaseSensitive))
		{
			Variables.Remove(Key);
		}
	}
}

SHIDENCORE_API void FShidenLocalVariable::ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	VariableDescriptors.Empty();
	TArray<FString> ScopeKeys;
	Variables.GetKeys(ScopeKeys);
	for (const FString& ScopeKey : ScopeKeys)
	{
		TArray<FString> Names;
		FShidenVariable Variable = Variables[ScopeKey];
		Variable.GetNames(Names);
		for (const FString& Name : Names)
		{
			if (FShidenVariableDefinition Definition; Variable.TryGetDefinition(Name, Definition))
			{
				FString Value;
				switch (Definition.Type)
				{
				case EShidenVariableType::Boolean:
					{
						bool bBooleanValue;
						Variable.TryGet(Name, bBooleanValue);
						Value = bBooleanValue ? TEXT("true") : TEXT("false");
						break;
					}
				case EShidenVariableType::String:
				case EShidenVariableType::AssetPath:
					{
						Variable.TryGet(Name, Value);
						break;
					}
				case EShidenVariableType::Integer:
					{
						int32 IntegerValue;
						Variable.TryGet(Name, IntegerValue);
						Value = FString::FromInt(IntegerValue);
						break;
					}
				case EShidenVariableType::Float:
					{
						float FloatValue;
						Variable.TryGet(Name, FloatValue);
						Value = FString::SanitizeFloat(FloatValue);
						break;
					}
				case EShidenVariableType::Vector2:
					{
						FVector2d Vector2Value;
						Variable.TryGet(Name, Vector2Value);
						Value = Vector2Value.ToString();
						break;
					}
				case EShidenVariableType::Vector3:
					{
						FVector Vector3Value;
						Variable.TryGet(Name, Vector3Value);
						Value = Vector3Value.ToString();
						break;
					}
				}
				VariableDescriptors.Add(FShidenVariableDescriptor(ScopeKey, Name, Definition.Type, Definition.AssetPathType, Value,
				                                                  Definition.DefaultValue, Definition.bIsReadOnly));
			}
		}
	}
}

SHIDENCORE_API bool FShidenLocalVariable::IsValidScope(const FString& ScopeKey) const
{
	if (Variables.Contains(ScopeKey))
	{
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Scope key is invalid. Please initialize LocalVariable with ScopeKey: %s"), *ScopeKey);
	return false;
}
