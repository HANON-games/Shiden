// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Variable/ShidenLocalVariable.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenStructuredLog.h"

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
	if (!Scenario)
	{
		SHIDEN_WARNING("InitLocalVariable failed: Scenario is null for ScopeKey: {key}", *ScopeKey);
		return;
	}

	TArray<FShidenVariableDefinition> Definitions = static_cast<TArray<FShidenVariableDefinition>>(Scenario->MacroParameterDefinitions);
	Definitions.Append(Scenario->LocalVariableDefinitions);
	Variables.Add(ScopeKey, FShidenVariable(Definitions));
	ScenarioIds.Add(ScopeKey, Scenario->ScenarioId);
}

SHIDENCORE_API void FShidenLocalVariable::UpdateVariableDefinitions()
{
	for (TPair<FString, FShidenVariable>& Pair : Variables)
	{
		const FString& ScopeKey = Pair.Key;
		UShidenScenario* Scenario = nullptr;
		if (ScenarioIds.Contains(ScopeKey) && UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioIds[ScopeKey], Scenario))
		{
			TArray<FShidenVariableDefinition> Definitions = static_cast<TArray<FShidenVariableDefinition>>(Scenario->MacroParameterDefinitions);
			Definitions.Append(Scenario->LocalVariableDefinitions);
			Pair.Value.UpdateVariableDefinitions(Definitions);
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

SHIDENCORE_API bool FShidenLocalVariable::TryGet(const FString& ScopeKey, const FString& Name, FVector2D& Value)
{
	if (IsValidScope(ScopeKey))
	{
		return Variables[ScopeKey].TryGet(Name, Value);
	}
	Value = FVector2D();
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
	for (const TPair<FString, FShidenVariable>& Pair : Variables)
	{
		Count += Pair.Value.Num();
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
	TArray<FString> KeysToRemove;
	const FString SearchKey = ProcessName + TEXT("$");
	for (const TPair<FString, FShidenVariable>& Pair : Variables)
	{
		if (Pair.Key.StartsWith(SearchKey, ESearchCase::CaseSensitive))
		{
			KeysToRemove.Add(Pair.Key);
		}
	}
	for (const FString& Key : KeysToRemove)
	{
		Variables.Remove(Key);
	}
}

SHIDENCORE_API void FShidenLocalVariable::ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
{
	VariableDescriptors.Empty();
	for (const TPair<FString, FShidenVariable>& Pair : Variables)
	{
		const FShidenVariable& Variable = Pair.Value;
		const FString& ScopeKey = Pair.Key;
		TArray<FString> Names;
		Variable.GetNames(Names);
		for (const FString& Name : Names)
		{
			if (FShidenVariableDefinition Definition; Variable.TryGetDefinition(Name, Definition))
			{
				FString Value;
				if (Variable.ConvertVariableValueToString(Definition, Name, Value))
				{
					VariableDescriptors.Add(FShidenVariableDescriptor(ScopeKey, Name, Definition.Type, Definition.AssetPathType, Value,
					                                                  Definition.DefaultValue, Definition.bIsReadOnly));
				}
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
	SHIDEN_WARNING("Scope key is invalid. Please initialize LocalVariable with ScopeKey: {key}", *ScopeKey);
	return false;
}
