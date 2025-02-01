// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Variable/ShidenVariable.h"

SHIDENCORE_API bool FShidenVariable::TryGetDefinition(const FString& Name, FShidenVariableDefinition& Definition)
{
	if (const FShidenVariableDefinition* Temp = VariableDefinitions.FindByKey(Name))
	{
		Definition = *Temp;
		return true;
	}
	if (Name == TEXT("EMPTY"))
	{
		Definition = FShidenVariableDefinition::Empty;
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::CanUpdate(const FString& Name, const EShidenVariableType& Type, const bool bForceUpdateReadOnly)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable %s is not defined."), *Name);
		return false;
	}
	if (!bForceUpdateReadOnly && Definition.IsReadOnly)
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

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const bool Value, const bool bForceUpdateReadOnly /*= false*/)
{
	if (CanUpdate(Name, EShidenVariableType::Boolean, bForceUpdateReadOnly))
	{
		BooleanVariables.Add(Name, Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const int32 Value, const bool bForceUpdateReadOnly /*= false*/)
{
	if (CanUpdate(Name, EShidenVariableType::Integer, bForceUpdateReadOnly))
	{
		IntegerVariables.Add(Name, Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const float Value, const bool bForceUpdateReadOnly /*= false*/)
{
	if (CanUpdate(Name, EShidenVariableType::Float, bForceUpdateReadOnly))
	{
		FloatVariables.Add(Name, Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const FString& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	if (CanUpdate(Name, EShidenVariableType::String, bForceUpdateReadOnly))
	{
		StringVariables.Add(Name, Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const FVector& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	if (CanUpdate(Name, EShidenVariableType::Vector3, bForceUpdateReadOnly))
	{
		Vector3Variables.Add(Name, Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryUpdate(const FString& Name, const FVector2D& Value, const bool bForceUpdateReadOnly /*= false*/)
{
	if (CanUpdate(Name, EShidenVariableType::Vector2, bForceUpdateReadOnly))
	{
		Vector2Variables.Add(Name, Value);
		return true;
	}
	return false;
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

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, bool& Value)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	if (CanGet(&Definition, Name, EShidenVariableType::Boolean))
	{
		Value = BooleanVariables.Contains(Name)
			        ? BooleanVariables[Name]
			        : Definition.DefaultValue == TEXT("true");
		return true;
	}
	Value = false;
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, int32& Value)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	if (CanGet(&Definition, Name, EShidenVariableType::Integer))
	{
		Value = IntegerVariables.Contains(Name)
			        ? IntegerVariables[Name]
			        : FCString::Atoi(*Definition.DefaultValue);
		return true;
	}
	Value = 0;
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, float& Value)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	if (CanGet(&Definition, Name, EShidenVariableType::Float))
	{
		Value = FloatVariables.Contains(Name)
			        ? FloatVariables[Name]
			        : FCString::Atof(*Definition.DefaultValue);
		return true;
	}
	Value = 0;
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, FString& Value)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	if (CanGet(&Definition, Name, EShidenVariableType::String))
	{
		Value = StringVariables.Contains(Name)
			        ? StringVariables[Name]
			        : *Definition.DefaultValue;
		return true;
	}
	Value = FString();
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, FVector& Value)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	if (CanGet(&Definition, Name, EShidenVariableType::Vector3))
	{
		if (Vector3Variables.Contains(Name))
		{
			Value = Vector3Variables[Name];
		}
		else
		{
			Value.InitFromString(Definition.DefaultValue);
		}
		return true;
	}
	Value = FVector();
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryGet(const FString& Name, FVector2d& Value)
{
	FShidenVariableDefinition Definition;
	if (!TryGetDefinition(Name, Definition))
	{
		return false;
	}
	if (CanGet(&Definition, Name, EShidenVariableType::Vector2))
	{
		if (Vector2Variables.Contains(Name))
		{
			Value = Vector2Variables[Name];
		}
		else
		{
			Value.InitFromString(Definition.DefaultValue);
		}
		return true;
	}
	Value = FVector2d();
	return false;
}

SHIDENCORE_API bool FShidenVariable::TryGetAsString(const FString& Name, EShidenVariableType& Type, FString& Value)
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
			if (FVector2d Vector2Value; TryGet(Name, Vector2Value))
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

SHIDENCORE_API void FShidenVariable::ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors)
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
					FVector2d Vector2Value;
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
			VariableDescriptors.Add(FShidenVariableDescriptor(Name, Definition.Type, Value, Definition.DefaultValue, Definition.IsReadOnly));
		}
	}
}

SHIDENCORE_API void FShidenVariable::UpdateVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	VariableDefinitions = Definitions;
}
