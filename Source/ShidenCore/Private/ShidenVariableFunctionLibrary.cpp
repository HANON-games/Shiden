// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenVariableFunctionLibrary.h"

SHIDENCORE_API bool IsValidUserVariableKey(const FString Key)
{
	const bool Result = Key != TEXT("EMPTY");
	if (!Result)
	{
		UE_LOG(LogTemp, Error, TEXT("Adding variable was failed. \"EMPTY\" is reserved key."));
	}
	return Result;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserBoolean(const FString Key, const bool bValue)
{
	if (!IsValidUserVariableKey(Key))
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Boolean)
	{
		RemoveUserVariable(Key);
	}
	ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Boolean);
	ShidenSubsystem->UserVariable.BooleanVariables.Add(Key, bValue);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserInteger(const FString Key, const int32 Value)
{
	if (!IsValidUserVariableKey(Key))
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Integer)
	{
		RemoveUserVariable(Key);
	}
	ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Integer);
	ShidenSubsystem->UserVariable.IntegerVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserFloat(const FString Key, const float Value)
{
	if (!IsValidUserVariableKey(Key))
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Float)
	{
		RemoveUserVariable(Key);
	}
	ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Float);
	ShidenSubsystem->UserVariable.FloatVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserString(const FString Key, const FString Value)
{
	if (!IsValidUserVariableKey(Key))
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::String)
	{
		RemoveUserVariable(Key);
	}

	ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::String);
	ShidenSubsystem->UserVariable.StringVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserVector3(const FString Key, const FVector Value)
{
	if (!IsValidUserVariableKey(Key))
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Vector3)
	{
		RemoveUserVariable(Key);
	}
	ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Vector3);
	ShidenSubsystem->UserVariable.Vector3Variables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserVector2(const FString Key, const FVector2D Value)
{
	if (!IsValidUserVariableKey(Key))
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Vector2)
	{
		RemoveUserVariable(Key);
	}
	ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Vector2);
	ShidenSubsystem->UserVariable.Vector2Variables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ContainsUserVariable(const FString Key, bool& bResult)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bResult = ShidenSubsystem->UserVariable.InternalKeys.Contains(Key);
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::IsEmptyUserVariable()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	return ShidenSubsystem->UserVariable.InternalKeys.IsEmpty();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UserVariableKeys(TArray<FString>& Keys)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->UserVariable.InternalKeys.GetKeys(Keys);
}

SHIDENCORE_API int32 UShidenVariableFunctionLibrary::UserVariableLength()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->UserVariable.InternalKeys.Num();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveUserVariable(const FString Key)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	const EShidenVariableType* Type = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);

	if (Type)
	{
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			ShidenSubsystem->UserVariable.BooleanVariables.Remove(Key);
			break;
		case EShidenVariableType::Integer:
			ShidenSubsystem->UserVariable.IntegerVariables.Remove(Key);
			break;
		case EShidenVariableType::Float:
			ShidenSubsystem->UserVariable.FloatVariables.Remove(Key);
			break;
		case EShidenVariableType::String:
			ShidenSubsystem->UserVariable.StringVariables.Remove(Key);
			break;
		case EShidenVariableType::Vector2:
			ShidenSubsystem->UserVariable.Vector2Variables.Remove(Key);
			break;
		case EShidenVariableType::Vector3:
			ShidenSubsystem->UserVariable.Vector3Variables.Remove(Key);
			break;
		}

		ShidenSubsystem->UserVariable.InternalKeys.Remove(Key);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindUserVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	if (Key.Compare(TEXT("EMPTY")) == 0)
	{
		StringValue = TEXT("");
		VariableType = EShidenVariableType::String;
		bReturnValue = true;
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* Type = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (Type)
	{
		VariableType = *Type;
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			bBooleanValue = ShidenSubsystem->UserVariable.BooleanVariables.FindRef(Key);
			break;
		case EShidenVariableType::Integer:
			IntegerValue = ShidenSubsystem->UserVariable.IntegerVariables.FindRef(Key);
			break;
		case EShidenVariableType::Float:
			FloatValue = ShidenSubsystem->UserVariable.FloatVariables.FindRef(Key);
			break;
		case EShidenVariableType::String:
			StringValue = ShidenSubsystem->UserVariable.StringVariables.FindRef(Key);
			break;
		case EShidenVariableType::Vector2:
			Vector2Value = ShidenSubsystem->UserVariable.Vector2Variables.FindRef(Key);
			break;
		case EShidenVariableType::Vector3:
			Vector3Value = ShidenSubsystem->UserVariable.Vector3Variables.FindRef(Key);
			break;
		}
		bReturnValue = true;
	}
	else
	{
		bReturnValue = false;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ClearUserVariables()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->UserVariable.BooleanVariables.Empty();
	ShidenSubsystem->UserVariable.IntegerVariables.Empty();
	ShidenSubsystem->UserVariable.FloatVariables.Empty();
	ShidenSubsystem->UserVariable.StringVariables.Empty();
	ShidenSubsystem->UserVariable.Vector2Variables.Empty();
	ShidenSubsystem->UserVariable.Vector3Variables.Empty();
	ShidenSubsystem->UserVariable.InternalKeys.Empty();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindUserVariableAsString(const FString Key, FString& Result, bool& bReturnValue)
{
	if (Key.Compare(TEXT("EMPTY")) == 0)
	{
		Result = TEXT("");
		bReturnValue = true;
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* Type = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
	if (Type)
	{
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			Result = ShidenSubsystem->UserVariable.BooleanVariables.FindRef(Key) ? TEXT("true") : TEXT("false");
			break;
		case EShidenVariableType::Integer:
			Result = FString::FromInt(ShidenSubsystem->UserVariable.IntegerVariables.FindRef(Key));
			break;
		case EShidenVariableType::Float:
			Result = FString::SanitizeFloat(ShidenSubsystem->UserVariable.FloatVariables.FindRef(Key));
			break;
		case EShidenVariableType::String:
			Result = ShidenSubsystem->UserVariable.StringVariables.FindRef(Key);
			break;
		case EShidenVariableType::Vector2:
			Result = ShidenSubsystem->UserVariable.Vector2Variables.FindRef(Key).ToString();
			break;
		case EShidenVariableType::Vector3:
			Result = ShidenSubsystem->UserVariable.Vector3Variables.FindRef(Key).ToString();
			break;
		}
		bReturnValue = true;
	}
	else
	{
		bReturnValue = false;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemBoolean(const FString Key, const bool bValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Boolean)
	{
		RemoveSystemVariable(Key);
	}
	ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Boolean);
	ShidenSubsystem->SystemVariable.BooleanVariables.Add(Key, bValue);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemInteger(const FString Key, const int32 Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Integer)
	{
		RemoveSystemVariable(Key);
	}
	ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Integer);
	ShidenSubsystem->SystemVariable.IntegerVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemFloat(const FString Key, const float Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Float)
	{
		RemoveSystemVariable(Key);
	}
	ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Float);
	ShidenSubsystem->SystemVariable.FloatVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemString(const FString Key, const FString Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::String)
	{
		RemoveSystemVariable(Key);
	}
	ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::String);
	ShidenSubsystem->SystemVariable.StringVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemVector2(const FString Key, const FVector2D Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Vector2)
	{
		RemoveSystemVariable(Key);
	}
	ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Vector2);
	ShidenSubsystem->SystemVariable.Vector2Variables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemVector3(const FString Key, const FVector Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Vector3)
	{
		RemoveSystemVariable(Key);
	}
	ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Vector3);
	ShidenSubsystem->SystemVariable.Vector3Variables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ContainsSystemVariable(const FString Key, bool& bResult)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bResult = ShidenSubsystem->SystemVariable.InternalKeys.Contains(Key);
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::IsEmptySystemVariable()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->SystemVariable.InternalKeys.IsEmpty();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SystemVariableKeys(TArray<FString>& Keys)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	ShidenSubsystem->SystemVariable.InternalKeys.GetKeys(Keys);
}

SHIDENCORE_API int32 UShidenVariableFunctionLibrary::SystemVariableLength()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->SystemVariable.InternalKeys.Num();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveSystemVariable(const FString Key)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	const EShidenVariableType* Type = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);

	if (Type)
	{
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			ShidenSubsystem->SystemVariable.BooleanVariables.Remove(Key);
			break;
		case EShidenVariableType::Integer:
			ShidenSubsystem->SystemVariable.IntegerVariables.Remove(Key);
			break;
		case EShidenVariableType::Float:
			ShidenSubsystem->SystemVariable.FloatVariables.Remove(Key);
			break;
		case EShidenVariableType::String:
			ShidenSubsystem->SystemVariable.StringVariables.Remove(Key);
			break;
		case EShidenVariableType::Vector2:
			ShidenSubsystem->SystemVariable.Vector2Variables.Remove(Key);
			break;
		case EShidenVariableType::Vector3:
			ShidenSubsystem->SystemVariable.Vector3Variables.Remove(Key);
			break;
		}

		ShidenSubsystem->SystemVariable.InternalKeys.Remove(Key);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindSystemVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);


	const EShidenVariableType* Type = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (Type)
	{
		VariableType = *Type;
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			bBooleanValue = ShidenSubsystem->SystemVariable.BooleanVariables.FindRef(Key);
			break;
		case EShidenVariableType::Integer:
			IntegerValue = ShidenSubsystem->SystemVariable.IntegerVariables.FindRef(Key);
			break;
		case EShidenVariableType::Float:
			FloatValue = ShidenSubsystem->SystemVariable.FloatVariables.FindRef(Key);
			break;
		case EShidenVariableType::String:
			StringValue = ShidenSubsystem->SystemVariable.StringVariables.FindRef(Key);
			break;
		case EShidenVariableType::Vector2:
			Vector2Value = ShidenSubsystem->SystemVariable.Vector2Variables.FindRef(Key);
			break;
		case EShidenVariableType::Vector3:
			Vector3Value = ShidenSubsystem->SystemVariable.Vector3Variables.FindRef(Key);
			break;
		}
		bReturnValue = true;
	}
	else
	{
		bReturnValue = false;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ClearSystemVariables()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable.BooleanVariables.Empty();
	ShidenSubsystem->SystemVariable.IntegerVariables.Empty();
	ShidenSubsystem->SystemVariable.FloatVariables.Empty();
	ShidenSubsystem->SystemVariable.StringVariables.Empty();
	ShidenSubsystem->SystemVariable.Vector2Variables.Empty();
	ShidenSubsystem->SystemVariable.Vector3Variables.Empty();
	ShidenSubsystem->SystemVariable.InternalKeys.Empty();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindSystemVariableAsString(const FString Key, FString& Result, bool& bReturnValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const EShidenVariableType* Type = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
	if (Type)
	{
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			Result = ShidenSubsystem->SystemVariable.BooleanVariables.FindRef(Key) ? TEXT("true") : TEXT("false");
			break;
		case EShidenVariableType::Integer:
			Result = FString::FromInt(ShidenSubsystem->SystemVariable.IntegerVariables.FindRef(Key));
			break;
		case EShidenVariableType::Float:
			Result = FString::SanitizeFloat(ShidenSubsystem->SystemVariable.FloatVariables.FindRef(Key));
			break;
		case EShidenVariableType::String:
			Result = ShidenSubsystem->SystemVariable.StringVariables.FindRef(Key);
			break;
		case EShidenVariableType::Vector2:
			Result = ShidenSubsystem->SystemVariable.Vector2Variables.FindRef(Key).ToString();
			break;
		case EShidenVariableType::Vector3:
			Result = ShidenSubsystem->SystemVariable.Vector3Variables.FindRef(Key).ToString();
			break;
		}
		bReturnValue = true;
	}
	else
	{
		bReturnValue = false;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveLocalVariable(const FString ProcessName, const FString Key)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = ShidenSubsystem->LocalVariables.Find(LocalKey);

	if (LocalVariables)
	{
		const EShidenVariableType* Type = LocalVariables->InternalKeys.Find(Key);

		if (Type)
		{
			switch (*Type)
			{
			case EShidenVariableType::Boolean:
				LocalVariables->BooleanVariables.Remove(Key);
				break;
			case EShidenVariableType::Integer:
				LocalVariables->IntegerVariables.Remove(Key);
				break;
			case EShidenVariableType::Float:
				LocalVariables->FloatVariables.Remove(Key);
				break;
			case EShidenVariableType::String:
				LocalVariables->StringVariables.Remove(Key);
				break;
			case EShidenVariableType::Vector2:
				LocalVariables->Vector2Variables.Remove(Key);
				break;
			case EShidenVariableType::Vector3:
				LocalVariables->Vector3Variables.Remove(Key);
				break;
			}

			LocalVariables->InternalKeys.Remove(Key);
		}
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddLocalBoolean(const FString ProcessName, const FString Key, const bool bValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = &ShidenSubsystem->LocalVariables.FindOrAdd(LocalKey);
	const EShidenVariableType* CurrentType = LocalVariables->InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Boolean)
	{
		RemoveLocalVariable(ProcessName, Key);
	}
	LocalVariables->InternalKeys.Add(Key, EShidenVariableType::Boolean);
	LocalVariables->BooleanVariables.Add(Key, bValue);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddLocalInteger(const FString ProcessName, const FString Key, const int32 Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = &ShidenSubsystem->LocalVariables.FindOrAdd(LocalKey);
	const EShidenVariableType* CurrentType = LocalVariables->InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Integer)
	{
		RemoveLocalVariable(ProcessName, Key);
	}
	LocalVariables->InternalKeys.Add(Key, EShidenVariableType::Integer);
	LocalVariables->IntegerVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddLocalFloat(const FString ProcessName, const FString Key, const float Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = &ShidenSubsystem->LocalVariables.FindOrAdd(LocalKey);
	const EShidenVariableType* CurrentType = LocalVariables->InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Float)
	{
		RemoveLocalVariable(ProcessName, Key);
	}
	LocalVariables->InternalKeys.Add(Key, EShidenVariableType::Float);
	LocalVariables->FloatVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddLocalString(const FString ProcessName, const FString Key, const FString Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = &ShidenSubsystem->LocalVariables.FindOrAdd(LocalKey);
	const EShidenVariableType* CurrentType = LocalVariables->InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::String)
	{
		RemoveLocalVariable(ProcessName, Key);
	}
	LocalVariables->InternalKeys.Add(Key, EShidenVariableType::String);
	LocalVariables->StringVariables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddLocalVector2(const FString ProcessName, const FString Key, const FVector2D Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = &ShidenSubsystem->LocalVariables.FindOrAdd(LocalKey);
	const EShidenVariableType* CurrentType = LocalVariables->InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Vector2)
	{
		RemoveLocalVariable(ProcessName, Key);
	}
	LocalVariables->InternalKeys.Add(Key, EShidenVariableType::Vector2);
	LocalVariables->Vector2Variables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddLocalVector3(const FString ProcessName, const FString Key, const FVector Value)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = &ShidenSubsystem->LocalVariables.FindOrAdd(LocalKey);
	const EShidenVariableType* CurrentType = LocalVariables->InternalKeys.Find(Key);
	if (CurrentType && *CurrentType != EShidenVariableType::Vector3)
	{
		RemoveLocalVariable(ProcessName, Key);
	}
	LocalVariables->InternalKeys.Add(Key, EShidenVariableType::Vector3);
	LocalVariables->Vector3Variables.Add(Key, Value);
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveCurrentLocalVariables(const FString ProcessName)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}

	ShidenSubsystem->LocalVariables.Remove(LocalKey);

	return;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveProcessLocalVariables(const FString ProcessName)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FString SearchKey = ProcessName + TEXT("$");

	for (const auto& Elem : ShidenSubsystem->LocalVariables)
	{
		const FString Key = Elem.Key;
		if (Key.StartsWith(ProcessName, ESearchCase::CaseSensitive))
		{
			ShidenSubsystem->LocalVariables.Remove(Key);
		}
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ClearAllLocalVariables()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	ShidenSubsystem->LocalVariables.Empty();
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindLocalVariable(const FString ProcessName, const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		bReturnValue = false;
		return;
	}
	const FShidenVariable* LocalVariables = ShidenSubsystem->LocalVariables.Find(LocalKey);
	if (!LocalVariables)
	{
		bReturnValue = false;
		return;
	}
	const EShidenVariableType* Type = LocalVariables->InternalKeys.Find(Key);
	if (Type)
	{
		VariableType = *Type;
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			bBooleanValue = LocalVariables->BooleanVariables.FindRef(Key);
			break;
		case EShidenVariableType::String:
			StringValue = LocalVariables->StringVariables.FindRef(Key);
			break;
		case EShidenVariableType::Integer:
			IntegerValue = LocalVariables->IntegerVariables.FindRef(Key);
			break;
		case EShidenVariableType::Float:
			FloatValue = LocalVariables->FloatVariables.FindRef(Key);
			break;
		case EShidenVariableType::Vector2:
			Vector2Value = LocalVariables->Vector2Variables.FindRef(Key);
			break;
		case EShidenVariableType::Vector3:
			Vector3Value = LocalVariables->Vector3Variables.FindRef(Key);
			break;
		}
		bReturnValue = true;
	}
	else
	{
		bReturnValue = false;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindLocalVariableAsString(const FString ProcessName, const FString Key, FString& Result, bool& bReturnValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		bReturnValue = false;
		return;
	}
	const FShidenVariable* LocalVariables = ShidenSubsystem->LocalVariables.Find(LocalKey);
	if (!LocalVariables)
	{
		bReturnValue = false;
		return;
	}
	const EShidenVariableType* Type = LocalVariables->InternalKeys.Find(Key);
	if (Type)
	{
		switch (*Type)
		{
		case EShidenVariableType::Boolean:
			Result = LocalVariables->BooleanVariables.FindRef(Key) ? TEXT("true") : TEXT("false");
			break;
		case EShidenVariableType::String:
			Result = LocalVariables->StringVariables.FindRef(Key);
			break;
		case EShidenVariableType::Integer:
			Result = FString::FromInt(LocalVariables->IntegerVariables.FindRef(Key));
			break;
		case EShidenVariableType::Float:
			Result = FString::SanitizeFloat(LocalVariables->FloatVariables.FindRef(Key));
			break;
		case EShidenVariableType::Vector2:
			Result = LocalVariables->Vector2Variables.FindRef(Key).ToString();
			break;
		case EShidenVariableType::Vector3:
			Result = LocalVariables->Vector3Variables.FindRef(Key).ToString();
			break;
		}
		bReturnValue = true;
	}
	else
	{
		bReturnValue = false;
	}
}

#define PROPERTY_NAME_OF(Property) GetLowerPropertyName(FString(TEXT(#Property)))

FString GetLowerPropertyName(FString FullPropertyName)
{
	FString Result, Dummy;
	FullPropertyName.Split(TEXT("::"), &Dummy, &Result);
	return Result;
}

FORCEINLINE bool PredefinedVariableKeyEquals(const FString& Key, const FString& PropertyName)
{
	return Key.Compare(PropertyName, ESearchCase::IgnoreCase) == 0;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindPredefinedSystemVariable(const FString Key, EShidenVariableType& Type, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bReturnValue = true;

	FString SearchKey = Key.Replace(TEXT(" "), TEXT(""));

	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bAutoRemoveWidgetOnEnd).Mid(1)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bAutoRemoveWidgetOnEnd;
		Type = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::BgmVolumeRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate;
		Type = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::LanguageIndex)))
	{
		IntegerValue = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
		Type = EShidenVariableType::Integer;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::MappingContext)))
	{
		StringValue = ShidenSubsystem->PredefinedSystemVariable.MappingContext.GetPath();
		Type = EShidenVariableType::String;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::MarkedReadLines)))
	{
		IntegerValue = ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines.Num();
		Type = EShidenVariableType::Integer;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SecondsToWaitForEachLetter)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.SecondsToWaitForEachLetter;
		Type = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SeVolumeRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate;
		Type = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bShowMouseCursorWhenStarting).Mid(1)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bShowMouseCursorWhenStarting;
		Type = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SkipSpeedRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate;
		Type = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bSkipUnread).Mid(1)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bSkipUnread;
		Type = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bStopVoiceWhenNextDialogueStart).Mid(1)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bStopVoiceWhenNextDialogueStart;
		Type = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::TargetFrameRate)))
	{
		IntegerValue = ShidenSubsystem->PredefinedSystemVariable.TargetFrameRate;
		Type = EShidenVariableType::Integer;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bUseAddToPlayerScreenInsteadOfAddToViewport).Mid(1)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bUseAddToPlayerScreenInsteadOfAddToViewport;
		Type = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::VoiceVolumeRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate;
		Type = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::WaitTimeOnAutoMode)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.WaitTimeOnAutoMode;
		Type = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::ClickWaitingGlyph)))
	{
		StringValue = ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph;
		Type = EShidenVariableType::String;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bAutoSaveOnMobileAppWillDeactivate).Mid(1)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bAutoSaveOnMobileAppWillDeactivate;
		Type = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::PlatformName)))
	{
		StringValue = ShidenSubsystem->PredefinedSystemVariable.PlatformName;
		Type = EShidenVariableType::String;
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Predefined variable key \"%s\" is invalid."), *Key);
	bReturnValue = false;
	return;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindPredefinedSystemVariableAsString(const FString Key, FString& Result, bool& bReturnValue)
{
	EShidenVariableType Type;
	bool bBooleanValue;
	FString StringValue;
	int32 IntegerValue;
	float FloatValue;
	FVector2D Vector2Value;
	FVector Vector3Value;
	bool bTempReturnValue;
	UShidenVariableFunctionLibrary::FindPredefinedSystemVariable(Key, Type, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bTempReturnValue);
	if (!bTempReturnValue)
	{
		bReturnValue = false;
		return;
	}
	switch (Type)
	{
		case EShidenVariableType::Boolean:
			Result = bBooleanValue ? TEXT("true") : TEXT("false");
			break;
		case EShidenVariableType::String:
			Result = StringValue;
			break;
		case EShidenVariableType::Integer:
			Result = FString::FromInt(IntegerValue);
			break;
		case EShidenVariableType::Float:
			Result = FString::SanitizeFloat(FloatValue);
			break;
		case EShidenVariableType::Vector2:
			Result = Vector2Value.ToString();
			break;
		case EShidenVariableType::Vector3:
			Result = Vector3Value.ToString();
			break;
	}
	bReturnValue = true;
	return;
}

SHIDENCORE_API TArray<FString> UShidenVariableFunctionLibrary::GetVariableNamesFromText(const FString text)
{
	if (!text.Contains(TEXT("{")) || !text.Contains(TEXT("}")))
	{
		return TArray<FString>();
	}

	TArray<FString> Result;

	const FRegexPattern GetVariablePattern = FRegexPattern(FString(TEXT("(?<!\\\\)\\{\\s*([^ \\}]+?)\\s*(?<!\\\\)\\}")));
	FRegexMatcher Matcher(GetVariablePattern, text);

	while (Matcher.FindNext())
	{
		Result.AddUnique(Matcher.GetCaptureGroup(1));
	}

	return Result;
}

FString ReplaceTextWithVariable(const FString& ProcessName, const FString& Text)
{
	if (!Text.Contains(TEXT("{")) || !Text.Contains(TEXT("}")))
	{
		return Text;
	}

	FString ResultText = Text;

	const FRegexPattern ReplaceTextPattern = FRegexPattern(FString(TEXT("((?<!\\\\)\\{ *[^ \\}]+ *(?<!\\\\)\\})")));
	FRegexMatcher Matcher(ReplaceTextPattern, Text);

	while (Matcher.FindNext())
	{
		FString Str = Matcher.GetCaptureGroup(1);
		FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();

		FString VariableKind, ReplacementText, VariableKey;
		bool bReturnValue;
		VariableName.Split(TEXT("::"), &VariableKind, &VariableKey, ESearchCase::CaseSensitive);

		if (VariableKey.IsEmpty())
		{
			// UserVariable
			UShidenVariableFunctionLibrary::FindUserVariableAsString(VariableName.Replace(TEXT("\\:"), TEXT(":")), ReplacementText, bReturnValue);
			if (!bReturnValue)
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("System"))
		{
			UShidenVariableFunctionLibrary::FindSystemVariableAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), ReplacementText, bReturnValue);
			if (!bReturnValue)
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("Local"))
		{
			UShidenVariableFunctionLibrary::FindLocalVariableAsString(ProcessName, VariableKey.Replace(TEXT("\\:"), TEXT(":")), ReplacementText, bReturnValue);
			if (!bReturnValue)
			{
				ReplacementText = TEXT("Error");
			}
		}
		else if (VariableKind == TEXT("Predefined"))
		{
			UShidenVariableFunctionLibrary::FindPredefinedSystemVariableAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), ReplacementText, bReturnValue);
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

SHIDENCORE_API FShidenCommand UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(const FString ProcessName, const FShidenCommand Command)
{
	FShidenCommand Result = Command;
	Result.Args = TMap<FString, FString>();
	TArray<FString> Keys = TArray<FString>();
	Command.Args.GetKeys(Keys);
	for (const FString& Key : Keys)
	{
		FString Value = Command.Args.FindRef(Key);
		FString NewValue = ReplaceTextWithVariable(ProcessName, Value);
		Result.Args.Add(Key, NewValue);
	}
	return Result;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ConvertToVariableKind(const FString VariableKind, EShidenVariableKind& Result, bool& bSuccess)
{
	FString CleanedVariableKind = VariableKind.Replace(TEXT(" "), TEXT(""));
	bSuccess = false;

	if (CleanedVariableKind.Equals(TEXT("UserVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::UserVariable;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableKind.Equals(TEXT("LocalVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::LocalVariable;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableKind.Equals(TEXT("SystemVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::SystemVariable;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableKind.Equals(TEXT("PredefinedSystemVariable"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableKind::PredefinedSystemVariable;
		bSuccess = true;
		return;
	}
	else
	{
		Result = EShidenVariableKind::UserVariable;
		return;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ConvertToVariableType(const FString VariableType, EShidenVariableType& Result, bool& bSuccess)
{
	FString CleanedVariableType = VariableType.Replace(TEXT(" "), TEXT(""));
	bSuccess = false;

	if (CleanedVariableType.Equals(TEXT("Boolean"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Boolean;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableType.Equals(TEXT("String"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::String;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableType.Equals(TEXT("Integer"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Integer;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableType.Equals(TEXT("Float"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Float;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableType.Equals(TEXT("Vector2"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Vector2;
		bSuccess = true;
		return;
	}
	else if (CleanedVariableType.Equals(TEXT("Vector3"), ESearchCase::IgnoreCase))
	{
		Result = EShidenVariableType::Vector3;
		bSuccess = true;
		return;
	}
	else
	{
		Result = EShidenVariableType::Boolean;
		return;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::InitPredefindSystemVariables()
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->PredefinedSystemVariable = ShidenProjectConfig->PredefinedSystemVariable;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSkipSpeedRate(float& SkipSpeedRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SkipSpeedRate = ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetVoiceVolumeRate(float& VoiceVolumeRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	VoiceVolumeRate = ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSeVolumeRate(float& SeVolumeRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SeVolumeRate = ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetBgmVolumeRate(float& BgmVolumeRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	BgmVolumeRate = ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetShowMouseCursorWhenStarting(bool& bShowMouseCursorWhenStarting)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bShowMouseCursorWhenStarting = ShidenSubsystem->PredefinedSystemVariable.bShowMouseCursorWhenStarting;
}


SHIDENCORE_API void UShidenVariableFunctionLibrary::GetLanguageIndex(int& LanguageIndex)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	LanguageIndex = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSecondsToWaitForEachLetter(float& SecondsToWaitForEachLetter)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SecondsToWaitForEachLetter = ShidenSubsystem->PredefinedSystemVariable.SecondsToWaitForEachLetter;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetWaitTimeOnAutoMode(float& WaitTimeOnAutoMode)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	WaitTimeOnAutoMode = ShidenSubsystem->PredefinedSystemVariable.WaitTimeOnAutoMode;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetStopVoiceWhenNextDialogueStart(bool& bStopVoiceWhenNextDialogueStart)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bStopVoiceWhenNextDialogueStart = ShidenSubsystem->PredefinedSystemVariable.bStopVoiceWhenNextDialogueStart;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetMappingContext(UInputMappingContext*& MappingContext)
{

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	MappingContext = ShidenSubsystem->PredefinedSystemVariable.MappingContext;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetUseAddToPlayerScreenInsteadOfAddToViewport(bool& bUseAddToPlayerScreenInsteadOfAddToViewport)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bUseAddToPlayerScreenInsteadOfAddToViewport = ShidenSubsystem->PredefinedSystemVariable.bUseAddToPlayerScreenInsteadOfAddToViewport;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetAutoRemoveWidgetOnEnd(bool& bAutoRemoveWidgetOnEnd)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bAutoRemoveWidgetOnEnd = ShidenSubsystem->PredefinedSystemVariable.bAutoRemoveWidgetOnEnd;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSkipUnread(bool& bSkipUnread)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bSkipUnread = ShidenSubsystem->PredefinedSystemVariable.bSkipUnread;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetTargetFrameRate(int& TargetFrameRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	TargetFrameRate = ShidenSubsystem->PredefinedSystemVariable.TargetFrameRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetMarkedReadLines(TMap<FGuid, FShidenReadLines>& MarkedReadLines)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	MarkedReadLines = ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetClickWaitingGlyph(FString& ClickWaitingGlyph)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ClickWaitingGlyph = ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetAutoSaveOnMobileAppWillDeactivate(bool& bAutoSaveOnMobileAppWillDeactivate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	bAutoSaveOnMobileAppWillDeactivate = ShidenSubsystem->PredefinedSystemVariable.bAutoSaveOnMobileAppWillDeactivate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetPlatformName(FString& PlatformName)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	PlatformName = ShidenSubsystem->PredefinedSystemVariable.PlatformName;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSkipSpeedRate(const float SkipSpeedRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate = SkipSpeedRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetVoiceVolumeRate(const float VoiceVolumeRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate = VoiceVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSeVolumeRate(const float SeVolumeRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate = SeVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetBgmVolumeRate(const float BgmVolumeRate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate = BgmVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetShowMouseCursorWhenStarting(const bool bShowMouseCursorWhenStarting)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.bShowMouseCursorWhenStarting = bShowMouseCursorWhenStarting;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetLanguageIndex(const int32 LanguageIndex)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.LanguageIndex = LanguageIndex;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSecondsToWaitForEachLetter(const float SecondsToWaitForEachLetter)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.SecondsToWaitForEachLetter = SecondsToWaitForEachLetter;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetWaitTimeOnAutoMode(const float WaitTimeOnAutoMode)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.WaitTimeOnAutoMode = WaitTimeOnAutoMode;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetStopVoiceWhenNextDialogueStart(const bool bStopVoiceWhenNextDialogueStart)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.bStopVoiceWhenNextDialogueStart = bStopVoiceWhenNextDialogueStart;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetMappingContext(UInputMappingContext* MappingContext)
{
	if (!MappingContext)
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.MappingContext = MappingContext;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetUseAddToPlayerScreenInsteadOfAddToViewport(const bool bUseAddToPlayerScreenInsteadOfAddToViewport)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.bUseAddToPlayerScreenInsteadOfAddToViewport = bUseAddToPlayerScreenInsteadOfAddToViewport;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetAutoRemoveWidgetOnEnd(const bool bAutoRemoveWidgetOnEnd)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.bAutoRemoveWidgetOnEnd = bAutoRemoveWidgetOnEnd;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSkipUnread(const bool bSkipUnread)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.bSkipUnread = bSkipUnread;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetTargetFrameRate(const int32 TargetFrameRate)
{
	if (TargetFrameRate <= 0)
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.TargetFrameRate = TargetFrameRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetMarkedReadLines(const TMap<FGuid, FShidenReadLines>& MarkedReadLines)
{
	if (MarkedReadLines.Num() == 0)
	{
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines = MarkedReadLines;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetClickWaitingGlyph(const FString ClickWaitingGlyph)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph = ClickWaitingGlyph;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetAutoSaveOnMobileAppWillDeactivate(const bool bAutoSaveOnMobileAppWillDeactivate)
{
	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->PredefinedSystemVariable.bAutoSaveOnMobileAppWillDeactivate = bAutoSaveOnMobileAppWillDeactivate;
}

SHIDENCORE_API void AddPredefinedVariable(const EShidenVariableType& Type, const FString Key, const bool bBooleanValue, const FString StringValue, const int32 IntegerValue, const float FloatValue, bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	if (Key.IsEmpty())
	{
		ErrorMessage = TEXT("Invalid Key Name");
		return;
	}

	FString SearchKey = Type == EShidenVariableType::Boolean
		? TEXT("b") + Key.Replace(TEXT(" "), TEXT(""))
		: Key.Replace(TEXT(" "), TEXT(""));

	if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SkipSpeedRate)))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetSkipSpeedRate(FloatValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::VoiceVolumeRate)))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetVoiceVolumeRate(FloatValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SeVolumeRate)))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetSeVolumeRate(FloatValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::BgmVolumeRate)))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetBgmVolumeRate(FloatValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bShowMouseCursorWhenStarting)))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetShowMouseCursorWhenStarting(bBooleanValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::LanguageIndex)))
	{
		if (Type != EShidenVariableType::Integer)
		{
			ErrorMessage = TEXT("Type must be integer");
			return;
		}
		UShidenVariableFunctionLibrary::SetLanguageIndex(IntegerValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SecondsToWaitForEachLetter)))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetSecondsToWaitForEachLetter(FloatValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::WaitTimeOnAutoMode)))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetWaitTimeOnAutoMode(FloatValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bStopVoiceWhenNextDialogueStart)))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetStopVoiceWhenNextDialogueStart(bBooleanValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::MappingContext)))
	{
		ErrorMessage = TEXT("MappingContext not supported");
		return;
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bUseAddToPlayerScreenInsteadOfAddToViewport)))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetUseAddToPlayerScreenInsteadOfAddToViewport(bBooleanValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bAutoRemoveWidgetOnEnd)))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetAutoRemoveWidgetOnEnd(bBooleanValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bSkipUnread)))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetSkipUnread(bBooleanValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::TargetFrameRate)))
	{
		if (Type != EShidenVariableType::Integer)
		{
			ErrorMessage = TEXT("Type must be integer");
			return;
		}
		UShidenVariableFunctionLibrary::SetTargetFrameRate(IntegerValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::ClickWaitingGlyph)))
	{
		if (Type != EShidenVariableType::String)
		{
			ErrorMessage = TEXT("Type must be string");
			return;
		}
		UShidenVariableFunctionLibrary::SetClickWaitingGlyph(StringValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::bAutoSaveOnMobileAppWillDeactivate)))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetAutoSaveOnMobileAppWillDeactivate(bBooleanValue);
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::PlatformName)))
	{
		ErrorMessage = TEXT("PlatformName is not supported");
		return;
	}
	else if (PredefinedVariableKeyEquals(SearchKey, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::MarkedReadLines)))
	{
		ErrorMessage = TEXT("MarkedReadLines is not supported");
		return;
	}
	else
	{
		ErrorMessage = TEXT("Invalid Key Name");
		return;
	}

	bSuccess = true;
	ErrorMessage = TEXT("");
	return;
}

SHIDENCORE_API void AddLocalVariable(const FString ProcessName, const EShidenVariableType& Type, const FString Key, const bool& bBooleanValue, const FString StringValue, const int32 IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
	{
		UShidenVariableFunctionLibrary::AddLocalBoolean(ProcessName, Key, bBooleanValue);
		break;
	}
	case EShidenVariableType::String:
	{
		UShidenVariableFunctionLibrary::AddLocalString(ProcessName, Key, StringValue);
		break;
	}
	case EShidenVariableType::Integer:
	{
		UShidenVariableFunctionLibrary::AddLocalInteger(ProcessName, Key, IntegerValue);
		break;
	}
	case EShidenVariableType::Float:
	{
		UShidenVariableFunctionLibrary::AddLocalFloat(ProcessName, Key, FloatValue);
		break;
	}
	case EShidenVariableType::Vector2:
	{
		UShidenVariableFunctionLibrary::AddLocalVector2(ProcessName, Key, Vector2Value);
		break;
	}
	case EShidenVariableType::Vector3:
	{
		UShidenVariableFunctionLibrary::AddLocalVector3(ProcessName, Key, Vector3Value);
		break;
	}
	}
}

SHIDENCORE_API void AddSystemVariable(const EShidenVariableType& Type, const FString Key, const bool& bBooleanValue, const FString StringValue, const int32 IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
	{
		UShidenVariableFunctionLibrary::AddSystemBoolean(Key, bBooleanValue);
		break;
	}
	case EShidenVariableType::String:
	{
		UShidenVariableFunctionLibrary::AddSystemString(Key, StringValue);
		break;
	}
	case EShidenVariableType::Integer:
	{
		UShidenVariableFunctionLibrary::AddSystemInteger(Key, IntegerValue);
		break;
	}
	case EShidenVariableType::Float:
	{
		UShidenVariableFunctionLibrary::AddSystemFloat(Key, FloatValue);
		break;
	}
	case EShidenVariableType::Vector2:
	{
		UShidenVariableFunctionLibrary::AddSystemVector2(Key, Vector2Value);
		break;
	}
	case EShidenVariableType::Vector3:
	{
		UShidenVariableFunctionLibrary::AddSystemVector3(Key, Vector3Value);
		break;
	}
	}
}

SHIDENCORE_API void AddUserVariable(const EShidenVariableType& Type, const FString Key, const bool& bBooleanValue, const FString StringValue, const int32 IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value)
{
	switch (Type)
	{
	case EShidenVariableType::Boolean:
	{
		UShidenVariableFunctionLibrary::AddUserBoolean(Key, bBooleanValue);
		break;
	}
	case EShidenVariableType::String:
	{
		UShidenVariableFunctionLibrary::AddUserString(Key, StringValue);
		break;
	}
	case EShidenVariableType::Integer:
	{
		UShidenVariableFunctionLibrary::AddUserInteger(Key, IntegerValue);
		break;
	}
	case EShidenVariableType::Float:
	{
		UShidenVariableFunctionLibrary::AddUserFloat(Key, FloatValue);
		break;
	}
	case EShidenVariableType::Vector2:
	{
		UShidenVariableFunctionLibrary::AddUserVector2(Key, Vector2Value);
		break;
	}
	case EShidenVariableType::Vector3:
	{
		UShidenVariableFunctionLibrary::AddUserVector3(Key, Vector3Value);
		break;
	}
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddVariable(const FString ProcessName, const EShidenVariableKind Kind, const EShidenVariableType Type, const FString Key, const bool bBooleanValue, const FString StringValue, const int32 IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value, bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	switch (Kind)
	{
		case EShidenVariableKind::UserVariable:
			AddUserVariable(Type, Key, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value);
			bSuccess = true;
			break;
		case EShidenVariableKind::LocalVariable:
			AddLocalVariable(ProcessName, Type, Key, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value);
			bSuccess = true;
			break;
		case EShidenVariableKind::SystemVariable:
			AddSystemVariable(Type, Key, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value);
			bSuccess = true;
			break;
		case EShidenVariableKind::PredefinedSystemVariable:
			AddPredefinedVariable(Type, Key, bBooleanValue, StringValue, IntegerValue, FloatValue, bSuccess, ErrorMessage);
			break;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindVariable(const FString ProcessName, const EShidenVariableKind Kind, const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	switch (Kind)
	{
		case EShidenVariableKind::UserVariable:
			UShidenVariableFunctionLibrary::FindUserVariable(Key, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bReturnValue);
			break;
		case EShidenVariableKind::LocalVariable:
			UShidenVariableFunctionLibrary::FindLocalVariable(ProcessName, Key, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bReturnValue);
			break;
		case EShidenVariableKind::SystemVariable:
			UShidenVariableFunctionLibrary::FindSystemVariable(Key, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bReturnValue);
			break;
		case EShidenVariableKind::PredefinedSystemVariable:
			UShidenVariableFunctionLibrary::FindPredefinedSystemVariable(Key, VariableType, bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bReturnValue);
			break;
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::EvaluateCondition(const EShidenVariableType Type, const FString Operator, const bool bABooleanValue, const FString AStringValue, const int32 AIntegerValue, const float AFloatValue, const FVector2D AVector2Value, const FVector AVector3Value,
	const bool bBBooleanValue, const FString BStringValue, const int32 BIntegerValue, const float BFloatValue, const FVector2D BVector2Value, const FVector BVector3Value, bool& bResult, bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	switch (Type)
	{
		case EShidenVariableType::Boolean:
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = bABooleanValue == bBBooleanValue;
				return;
			}
			else if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = bABooleanValue != bBBooleanValue;
				return;
			}
			else
			{
				ErrorMessage = TEXT("Invalid Operator");
				return;
			}
		case EShidenVariableType::String:
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AStringValue.Equals(BStringValue, ESearchCase::IgnoreCase);
				return;
			}
			else if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = !AStringValue.Equals(BStringValue, ESearchCase::IgnoreCase);
				return;
			}
			else
			{
				ErrorMessage = TEXT("Invalid Operator");
				return;
			}
		case EShidenVariableType::Integer:
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AIntegerValue == BIntegerValue;
				return;
			}
			else if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AIntegerValue != BIntegerValue;
				return;
			}
			else if (Operator == TEXT(">"))
			{
				bSuccess = true;
				bResult = AIntegerValue > BIntegerValue;
				return;
			}
			else if (Operator == TEXT("<"))
			{
				bSuccess = true;
				bResult = AIntegerValue < BIntegerValue;
				return;
			}
			else if (Operator == TEXT(">="))
			{
				bSuccess = true;
				bResult = AIntegerValue >= BIntegerValue;
				return;
			}
			else if (Operator == TEXT("<="))
			{
				bSuccess = true;
				bResult = AIntegerValue <= BIntegerValue;
				return;
			}
			else
			{
				ErrorMessage = TEXT("Invalid Operator");
				return;
			}
		case EShidenVariableType::Float:
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AFloatValue == BFloatValue;
				return;
			}
			else if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AFloatValue != BFloatValue;
				return;
			}
			else if (Operator == TEXT(">"))
			{
				bSuccess = true;
				bResult = AFloatValue > BFloatValue;
				return;
			}
			else if (Operator == TEXT("<"))
			{
				bSuccess = true;
				bResult = AFloatValue < BFloatValue;
				return;
			}
			else if (Operator == TEXT(">="))
			{
				bSuccess = true;
				bResult = AFloatValue >= BFloatValue;
				return;
			}
			else if (Operator == TEXT("<="))
			{
				bSuccess = true;
				bResult = AFloatValue <= BFloatValue;
				return;
			}
			else
			{
				ErrorMessage = TEXT("Invalid Operator");
				return;
			}
		case EShidenVariableType::Vector2:
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AVector2Value == BVector2Value;
				return;
			}
			else if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AVector2Value != BVector2Value;
				return;
			}
			else
			{
				ErrorMessage = TEXT("Invalid Operator");
				return;
			}
		case EShidenVariableType::Vector3:
			if (Operator == TEXT("=="))
			{
				bSuccess = true;
				bResult = AVector3Value == BVector3Value;
				return;
			}
			else if (Operator == TEXT("!="))
			{
				bSuccess = true;
				bResult = AVector3Value != BVector3Value;
				return;
			}
			else
			{
				ErrorMessage = TEXT("Invalid Operator");
				return;
			}
	}
}
