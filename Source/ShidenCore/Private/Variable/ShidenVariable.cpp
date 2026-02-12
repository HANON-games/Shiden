// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Variable/ShidenVariable.h"
#include "System/ShidenStructuredLog.h"

namespace
{
	bool CanGet(const FShidenVariableDefinition* Definition, const FString& Name, const EShidenVariableType& Type)
	{
		if (!Definition)
		{
			SHIDEN_WARNING("Variable {name} is not defined.", *Name);
			return false;
		}
		const EShidenVariableType& TypeA = Definition->Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Definition->Type;
		const EShidenVariableType& TypeB = Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Type;
		if (TypeA != TypeB)
		{
			SHIDEN_WARNING("Variable {name} is not {type}.", *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
			return false;
		}
		return true;
	}

	FORCEINLINE bool TryGetDefaultValue(const FString& DefaultValue, bool& Value)
	{
		Value = DefaultValue == TEXT("true");
		return true;
	}

	FORCEINLINE bool TryGetDefaultValue(const FString& DefaultValue, int32& Value)
	{
		Value = FCString::Atoi(*DefaultValue);
		return true;
	}

	FORCEINLINE bool TryGetDefaultValue(const FString& DefaultValue, float& Value)
	{
		Value = FCString::Atof(*DefaultValue);
		return true;
	}

	FORCEINLINE bool TryGetDefaultValue(const FString& DefaultValue, FString& Value)
	{
		Value = DefaultValue;
		return true;
	}

	FORCEINLINE bool TryGetDefaultValue(const FString& DefaultValue, FVector& Value)
	{
		return Value.InitFromString(DefaultValue);
	}

	FORCEINLINE bool TryGetDefaultValue(const FString& DefaultValue, FVector2D& Value)
	{
		return Value.InitFromString(DefaultValue);
	}
}

bool FShidenVariable::TryGetDefinition(const FString& Name, FShidenVariableDefinition& Definition) const
{
	if (const FShidenVariableDefinition* FoundDefinition = VariableDefinitions.FindByKey(Name))
	{
		Definition = *FoundDefinition;
		return true;
	}
	if (Name == TEXT("EMPTY"))
	{
		Definition = FShidenVariableDefinition::Empty;
		return true;
	}
	return false;
}

bool FShidenVariable::CanUpdate(const FString& Name, const EShidenVariableType& Type, const bool bForceUpdateReadOnly) const
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		SHIDEN_WARNING("Variable {name} is not defined.", *Name);
		return false;
	}
	if (!bForceUpdateReadOnly && Definition.bIsReadOnly)
	{
		SHIDEN_WARNING("Variable {name} is read only.", *Name);
		return false;
	}
	const EShidenVariableType& TypeA = Definition.Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Definition.Type;
	const EShidenVariableType& TypeB = Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Type;
	if (TypeA != TypeB)
	{
		SHIDEN_WARNING("Variable {name} is not {type}.", *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
		return false;
	}
	return true;
}

template <typename T>
bool FShidenVariable::TryUpdateImpl(const FString& Name, const T& Value, const EShidenVariableType Type, TMap<FString, T>& VariableMap, const bool bForceUpdateReadOnly)
{
	if (CanUpdate(Name, Type, bForceUpdateReadOnly))
	{
		VariableMap.Add(Name, Value);
		return true;
	}
	return false;
}

bool FShidenVariable::TryUpdate(const FString& Name, const bool Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Boolean, BooleanVariables, bForceUpdateReadOnly);
}

bool FShidenVariable::TryUpdate(const FString& Name, const int32 Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Integer, IntegerVariables, bForceUpdateReadOnly);
}

bool FShidenVariable::TryUpdate(const FString& Name, const float Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Float, FloatVariables, bForceUpdateReadOnly);
}

bool FShidenVariable::TryUpdate(const FString& Name, const FString& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::String, StringVariables, bForceUpdateReadOnly);
}

bool FShidenVariable::TryUpdate(const FString& Name, const FVector& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Vector3, Vector3Variables, bForceUpdateReadOnly);
}

bool FShidenVariable::TryUpdate(const FString& Name, const FVector2D& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Vector2, Vector2Variables, bForceUpdateReadOnly);
}

bool FShidenVariable::Contains(const FString& Name) const
{
	return VariableDefinitions.FindByKey(Name) != nullptr;
}


template <typename T>
bool FShidenVariable::TryGetImpl(const FString& Name, T& Value, const EShidenVariableType Type, const TMap<FString, T>& VariableMap) const
{
	FShidenVariableDefinition Definition;

	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}

	if (!CanGet(&Definition, Name, Type))
	{
		return false;
	}

	if (const T* FoundValue = VariableMap.Find(Name))
	{
		Value = *FoundValue;
		return true;
	}

	return TryGetDefaultValue(Definition.DefaultValue, Value);
}

bool FShidenVariable::TryGet(const FString& Name, bool& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Boolean, BooleanVariables);
}

bool FShidenVariable::TryGet(const FString& Name, int32& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Integer, IntegerVariables);
}

bool FShidenVariable::TryGet(const FString& Name, float& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Float, FloatVariables);
}

bool FShidenVariable::TryGet(const FString& Name, FString& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::String, StringVariables);
}

bool FShidenVariable::TryGet(const FString& Name, FVector& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Vector3, Vector3Variables);
}

bool FShidenVariable::TryGet(const FString& Name, FVector2D& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Vector2, Vector2Variables);
}

bool FShidenVariable::TryGetAsString(const FString& Name, EShidenVariableType& Type, FString& Value) const
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	Type = Definition.Type;
	return TryConvertVariableValueToString(Definition, Name, Value);
}

bool FShidenVariable::TryConvertVariableValueToString(const FShidenVariableDefinition& Definition,
                                                                     const FString& Name, FString& OutValue) const
{
	switch (Definition.Type)
	{
	case EShidenVariableType::Boolean:
		{
			bool bBooleanValue = false;
			if (!TryGet(Name, bBooleanValue))
			{
				return false;
			}
			OutValue = bBooleanValue ? TEXT("true") : TEXT("false");
			return true;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			return TryGet(Name, OutValue);
		}
	case EShidenVariableType::Integer:
		{
			int32 IntegerValue = 0;
			if (!TryGet(Name, IntegerValue))
			{
				return false;
			}
			OutValue = FString::FromInt(IntegerValue);
			return true;
		}
	case EShidenVariableType::Float:
		{
			float FloatValue = 0.0f;
			if (!TryGet(Name, FloatValue))
			{
				return false;
			}
			OutValue = FString::SanitizeFloat(FloatValue);
			return true;
		}
	case EShidenVariableType::Vector2:
		{
			FVector2D Vector2Value = FVector2D::ZeroVector;
			if (!TryGet(Name, Vector2Value))
			{
				return false;
			}
			OutValue = Vector2Value.ToString();
			return true;
		}
	case EShidenVariableType::Vector3:
		{
			FVector Vector3Value = FVector::ZeroVector;
			if (!TryGet(Name, Vector3Value))
			{
				return false;
			}
			OutValue = Vector3Value.ToString();
			return true;
		}
	default:
		SHIDEN_ERROR("Unknown variable type {type} for variable: {name}", static_cast<int32>(Definition.Type), *Name);
		return false;
	}
}

void FShidenVariable::ResetAll()
{
	BooleanVariables.Empty();
	IntegerVariables.Empty();
	FloatVariables.Empty();
	StringVariables.Empty();
	Vector3Variables.Empty();
	Vector2Variables.Empty();
}

bool FShidenVariable::TryReset(const FString& Name)
{
	if (FShidenVariableDefinition Definition; TryGetDefinition(Name, Definition))
	{
		switch (Definition.Type)
		{
		case EShidenVariableType::Boolean:
			BooleanVariables.Remove(Name);
			break;
		case EShidenVariableType::String:
		case EShidenVariableType::AssetPath:
			StringVariables.Remove(Name);
			break;
		case EShidenVariableType::Integer:
			IntegerVariables.Remove(Name);
			break;
		case EShidenVariableType::Float:
			FloatVariables.Remove(Name);
			break;
		case EShidenVariableType::Vector2:
			Vector2Variables.Remove(Name);
			break;
		case EShidenVariableType::Vector3:
			Vector3Variables.Remove(Name);
			break;
		default:
			SHIDEN_ERROR("Unknown variable type {type} for variable: {name}", static_cast<int32>(Definition.Type), *Name);
			return false;
		}
		return true;
	}
	return false;
}

void FShidenVariable::GetNames(TArray<FString>& OutNames) const
{
	OutNames.Empty();
	for (const FShidenVariableDefinition& Definition : VariableDefinitions)
	{
		OutNames.Add(Definition.Name);
	}
}

int32 FShidenVariable::Num() const
{
	return VariableDefinitions.Num();
}

void FShidenVariable::ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors) const
{
	VariableDescriptors.Empty();
	TArray<FString> Names;
	GetNames(Names);
	for (const FString& Name : Names)
	{
		if (FShidenVariableDefinition Definition; TryGetDefinition(Name, Definition))
		{
			FString Value;
			if (TryConvertVariableValueToString(Definition, Name, Value))
			{
				VariableDescriptors.Add(FShidenVariableDescriptor(Name, Definition.Type, Definition.AssetPathType, Value, Definition.DefaultValue,
				                                                  Definition.bIsReadOnly));
			}
		}
	}
}

void FShidenVariable::UpdateVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	VariableDefinitions = Definitions;
}
