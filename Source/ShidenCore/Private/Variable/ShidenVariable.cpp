// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Variable/ShidenVariable.h"

SHIDENCORE_API bool FShidenVariable::TryGetDefinition(const FString& Name, FShidenVariableDefinition& Definition) const
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

SHIDENCORE_API bool FShidenVariable::CanUpdate(const FString& Name, const EShidenVariableType& Type, const bool bForceUpdateReadOnly) const
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable %s is not defined."), *Name);
		return false;
	}
	if (!bForceUpdateReadOnly && Definition.bIsReadOnly)
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable %s is read only."), *Name);
		return false;
	}
	const EShidenVariableType& TypeA = Definition.Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Definition.Type;
	const EShidenVariableType& TypeB = Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Type;
	if (TypeA != TypeB)
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable %s is not %s."), *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
		return false;
	}
	return true;
}

template<typename T>
bool FShidenVariable::TryUpdateImpl(const FString& Name, const T& Value, const EShidenVariableType Type, TMap<FString, T>& VariableMap, const bool bForceUpdateReadOnly)
{
	if (CanUpdate(Name, Type, bForceUpdateReadOnly))
	{
		VariableMap.Add(Name, Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const bool Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Boolean, BooleanVariables, bForceUpdateReadOnly);
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const int32 Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Integer, IntegerVariables, bForceUpdateReadOnly);
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const float Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Float, FloatVariables, bForceUpdateReadOnly);
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const FString& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::String, StringVariables, bForceUpdateReadOnly);
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const FVector& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Vector3, Vector3Variables, bForceUpdateReadOnly);
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const FVector2D& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	return TryUpdateImpl(Name, Value, EShidenVariableType::Vector2, Vector2Variables, bForceUpdateReadOnly);
}

SHIDENCORE_API bool FShidenVariable::Contains(const FString& Name) const
{
	return VariableDefinitions.FindByKey(Name) != nullptr;
}

SHIDENCORE_API bool FShidenVariable::CanGet(const FShidenVariableDefinition* Definition, const FString& Name, const EShidenVariableType& Type)
{
	if (!Definition)
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable %s is not defined."), *Name);
		return false;
	}
	const EShidenVariableType& TypeA = Definition->Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Definition->Type;
	const EShidenVariableType& TypeB = Type == EShidenVariableType::AssetPath ? EShidenVariableType::String : Type;
	if (TypeA != TypeB)
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable %s is not %s."), *Name, *StaticEnum<EShidenVariableType>()->GetValueAsString(Type));
		return false;
	}
	return true;
}

template<typename T>
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

	return GetDefaultValue(Definition.DefaultValue, Value);
}

FORCEINLINE bool FShidenVariable::GetDefaultValue(const FString& DefaultValue, bool& Value)
{
	Value = DefaultValue == TEXT("true");
	return true;
}

FORCEINLINE bool FShidenVariable::GetDefaultValue(const FString& DefaultValue, int32& Value)
{
	Value = FCString::Atoi(*DefaultValue);
	return true;
}

FORCEINLINE bool FShidenVariable::GetDefaultValue(const FString& DefaultValue, float& Value)
{
	Value = FCString::Atof(*DefaultValue);
	return true;
}

FORCEINLINE bool FShidenVariable::GetDefaultValue(const FString& DefaultValue, FString& Value)
{
	Value = DefaultValue;
	return true;
}

FORCEINLINE bool FShidenVariable::GetDefaultValue(const FString& DefaultValue, FVector& Value)
{
	return Value.InitFromString(DefaultValue);
}

FORCEINLINE bool FShidenVariable::GetDefaultValue(const FString& DefaultValue, FVector2D& Value)
{
	return Value.InitFromString(DefaultValue);
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, bool& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Boolean, BooleanVariables);
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, int32& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Integer, IntegerVariables);
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, float& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Float, FloatVariables);
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, FString& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::String, StringVariables);
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, FVector& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Vector3, Vector3Variables);
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, FVector2D& Value) const
{
	return TryGetImpl(Name, Value, EShidenVariableType::Vector2, Vector2Variables);
}

SHIDENCORE_API bool FShidenVariable::TryGetAsString(const FString& Name, EShidenVariableType& Type, FString& Value) const
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	Type = Definition.Type;
	switch (Type)
	{
	case EShidenVariableType::Boolean:
		{
			if (bool bBooleanValue; TryGet(Name, bBooleanValue))
			{
				Value = bBooleanValue ? TEXT("true") : TEXT("false");
				return true;
			}
			return false;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			return TryGet(Name, Value);
		}
	case EShidenVariableType::Integer:
		{
			if (int32 IntValue; TryGet(Name, IntValue))
			{
				Value = FString::FromInt(IntValue);
				return true;
			}
			return false;
		}
	case EShidenVariableType::Float:
		{
			if (float FloatValue; TryGet(Name, FloatValue))
			{
				Value = FString::SanitizeFloat(FloatValue);
				return true;
			}
			return false;
		}
	case EShidenVariableType::Vector2:
		{
			if (FVector2D Vector2Value; TryGet(Name, Vector2Value))
			{
				Value = Vector2Value.ToString();
				return true;
			}
			return false;
		}
	case EShidenVariableType::Vector3:
		{
			if (FVector Vector3Value; TryGet(Name, Vector3Value))
			{
				Value = Vector3Value.ToString();
				return true;
			}
			return false;
		}
	}
	return false;
}

SHIDENCORE_API void FShidenVariable::ResetAll()
{
	BooleanVariables.Empty();
	IntegerVariables.Empty();
	FloatVariables.Empty();
	StringVariables.Empty();
	Vector3Variables.Empty();
	Vector2Variables.Empty();
}

SHIDENCORE_API bool FShidenVariable::TryReset(const FString& Name)
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
		}
		return true;
	}
	return false;
}

SHIDENCORE_API void FShidenVariable::GetNames(TArray<FString>& OutNames) const
{
	OutNames.Empty();
	for (const FShidenVariableDefinition& Definition : VariableDefinitions)
	{
		OutNames.Add(Definition.Name);
	}
}

SHIDENCORE_API int32 FShidenVariable::Num() const
{
	return VariableDefinitions.Num();
}

SHIDENCORE_API void FShidenVariable::ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors) const
{
	VariableDescriptors.Empty();
	TArray<FString> Names;
	GetNames(Names);
	for (const FString& Name : Names)
	{
		if (FShidenVariableDefinition Definition; TryGetDefinition(Name, Definition))
		{
			FString Value;
			switch (Definition.Type)
			{
			case EShidenVariableType::Boolean:
				{
					bool bBooleanValue;
					TryGet(Name, bBooleanValue);
					Value = bBooleanValue ? TEXT("true") : TEXT("false");
					break;
				}
			case EShidenVariableType::String:
			case EShidenVariableType::AssetPath:
				{
					TryGet(Name, Value);
					break;
				}
			case EShidenVariableType::Integer:
				{
					int32 IntegerValue;
					TryGet(Name, IntegerValue);
					Value = FString::FromInt(IntegerValue);
					break;
				}
			case EShidenVariableType::Float:
				{
					float FloatValue;
					TryGet(Name, FloatValue);
					Value = FString::SanitizeFloat(FloatValue);
					break;
				}
			case EShidenVariableType::Vector2:
				{
					FVector2D Vector2Value;
					TryGet(Name, Vector2Value);
					Value = Vector2Value.ToString();
					break;
				}
			case EShidenVariableType::Vector3:
				{
					FVector Vector3Value;
					TryGet(Name, Vector3Value);
					Value = Vector3Value.ToString();
					break;
				}
			}
			VariableDescriptors.Add(FShidenVariableDescriptor(Name, Definition.Type, Definition.AssetPathType, Value, Definition.DefaultValue,
			                                                  Definition.bIsReadOnly));
		}
	}
}

SHIDENCORE_API void FShidenVariable::UpdateVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	VariableDefinitions = Definitions;
}
