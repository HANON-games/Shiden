// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenVariableFunctionLibrary.h"
#include "Internationalization/Regex.h"
#include "Misc/FileHelper.h"
#include "Misc/OutputDeviceNull.h"
#include "Misc/Paths.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Serialization/Csv/CsvParser.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "DelayAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ShidenSubsystem.h"
#include "ShidenVariable.h"

SHIDENCORE_API bool IsValidUserVariableKey(const FString Key)
{
	const bool bResult = Key != TEXT("EMPTY");
	if (!bResult)
	{
		UE_LOG(LogTemp, Error, TEXT("Adding variable was failed. \"EMPTY\" is reserved key."));
	}
	return bResult;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserBoolean(const FString Key, const bool bValue)
{
	if (IsValidUserVariableKey(Key) == false)
	{
		return;
	}
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Boolean)
		{
			RemoveUserVariable(Key);
		}
		ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Boolean);
		ShidenSubsystem->UserVariable.BooleanVariables.Add(Key, bValue);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserInteger(const FString Key, const int32 Value)
{
	if (IsValidUserVariableKey(Key) == false)
	{
		return;
	}
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Integer)
		{
			RemoveUserVariable(Key);
		}
		ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Integer);
		ShidenSubsystem->UserVariable.IntegerVariables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserFloat(const FString Key, const float Value)
{
	if (IsValidUserVariableKey(Key) == false)
	{
		return;
	}
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Float)
		{
			RemoveUserVariable(Key);
		}
		ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Float);
		ShidenSubsystem->UserVariable.FloatVariables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserString(const FString Key, const FString Value)
{
	if (IsValidUserVariableKey(Key) == false)
	{
		return;
	}
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::String)
		{
			RemoveUserVariable(Key);
		}

		ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::String);
		ShidenSubsystem->UserVariable.StringVariables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserVector3(const FString Key, const FVector Value)
{
	if (IsValidUserVariableKey(Key) == false)
	{
		return;
	}
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Vector3)
		{
			RemoveUserVariable(Key);
		}
		ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Vector3);
		ShidenSubsystem->UserVariable.Vector3Variables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddUserVector2(const FString Key, const FVector2D Value)
{
	if (IsValidUserVariableKey(Key) == false)
	{
		return;
	}
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Vector2)
		{
			RemoveUserVariable(Key);
		}
		ShidenSubsystem->UserVariable.InternalKeys.Add(Key, EShidenVariableType::Vector2);
		ShidenSubsystem->UserVariable.Vector2Variables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ContainsUserVariable(const FString Key, bool& bResult)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		bResult = ShidenSubsystem->UserVariable.InternalKeys.Contains(Key);
	}
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::IsEmptyUserVariable()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		return ShidenSubsystem->UserVariable.InternalKeys.IsEmpty();
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::IsNotEmptyUserVariable()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		return !ShidenSubsystem->UserVariable.InternalKeys.IsEmpty();
	}
	return false;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::UserVariableKeys(TArray<FString>& Keys)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		ShidenSubsystem->UserVariable.InternalKeys.GetKeys(Keys);
	}
}

SHIDENCORE_API int32 UShidenVariableFunctionLibrary::UserVariableLength()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		return ShidenSubsystem->UserVariable.InternalKeys.Num();
	}
	return 0;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveUserVariable(const FString Key)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* type = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);

		if (type)
		{
			switch (*type)
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
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindUserVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	if (Key.Compare(TEXT("EMPTY")) == 0)
	{
		StringValue = TEXT("");
		VariableType = EShidenVariableType::String;
		bReturnValue = true;
		return;
	}

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* type = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (type)
		{
			VariableType = *type;
			switch (*type)
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
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ClearUserVariables()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		ShidenSubsystem->UserVariable.BooleanVariables.Empty();
		ShidenSubsystem->UserVariable.IntegerVariables.Empty();
		ShidenSubsystem->UserVariable.FloatVariables.Empty();
		ShidenSubsystem->UserVariable.StringVariables.Empty();
		ShidenSubsystem->UserVariable.Vector2Variables.Empty();
		ShidenSubsystem->UserVariable.Vector3Variables.Empty();
		ShidenSubsystem->UserVariable.InternalKeys.Empty();
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindUserVariableAsString(const FString Key, FString& Result, bool& bReturnValue)
{
	if (Key.Compare(TEXT("EMPTY")) == 0)
	{
		Result = TEXT("");
		bReturnValue = true;
		return;
	}

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* type = ShidenSubsystem->UserVariable.InternalKeys.Find(Key);
		if (type)
		{
			switch (*type)
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
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemBoolean(const FString Key, const bool bValue)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Boolean)
		{
			RemoveSystemVariable(Key);
		}
		ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Boolean);
		ShidenSubsystem->SystemVariable.BooleanVariables.Add(Key, bValue);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemInteger(const FString Key, const int32 Value)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Integer)
		{
			RemoveSystemVariable(Key);
		}
		ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Integer);
		ShidenSubsystem->SystemVariable.IntegerVariables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemFloat(const FString Key, const float Value)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Float)
		{
			RemoveSystemVariable(Key);
		}
		ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Float);
		ShidenSubsystem->SystemVariable.FloatVariables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemString(const FString Key, const FString value)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::String)
		{
			RemoveSystemVariable(Key);
		}
		ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::String);
		ShidenSubsystem->SystemVariable.StringVariables.Add(Key, value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemVector2(const FString Key, const FVector2D Value)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Vector2)
		{
			RemoveSystemVariable(Key);
		}
		ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Vector2);
		ShidenSubsystem->SystemVariable.Vector2Variables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddSystemVector3(const FString Key, const FVector Value)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* CurrentType = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (CurrentType && *CurrentType != EShidenVariableType::Vector3)
		{
			RemoveSystemVariable(Key);
		}
		ShidenSubsystem->SystemVariable.InternalKeys.Add(Key, EShidenVariableType::Vector3);
		ShidenSubsystem->SystemVariable.Vector3Variables.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ContainsSystemVariable(const FString Key, bool& bResult)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		bResult = ShidenSubsystem->SystemVariable.InternalKeys.Contains(Key);
	}
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::IsEmptySystemVariable()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		return ShidenSubsystem->SystemVariable.InternalKeys.IsEmpty();
	}
	return true;
}

SHIDENCORE_API bool UShidenVariableFunctionLibrary::IsNotEmptySystemVariable()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		return !ShidenSubsystem->SystemVariable.InternalKeys.IsEmpty();
	}
	return false;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SystemVariableKeys(TArray<FString>& Keys)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		ShidenSubsystem->SystemVariable.InternalKeys.GetKeys(Keys);
	}
}

SHIDENCORE_API int32 UShidenVariableFunctionLibrary::SystemVariableLength()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		return ShidenSubsystem->SystemVariable.InternalKeys.Num();
	}
	return 0;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveSystemVariable(const FString Key)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* type = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);

		if (type)
		{
			switch (*type)
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
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindSystemVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* type = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (type)
		{
			VariableType = *type;
			switch (*type)
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
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::ClearSystemVariables()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		ShidenSubsystem->SystemVariable.BooleanVariables.Empty();
		ShidenSubsystem->SystemVariable.IntegerVariables.Empty();
		ShidenSubsystem->SystemVariable.FloatVariables.Empty();
		ShidenSubsystem->SystemVariable.StringVariables.Empty();
		ShidenSubsystem->SystemVariable.Vector2Variables.Empty();
		ShidenSubsystem->SystemVariable.Vector3Variables.Empty();
		ShidenSubsystem->SystemVariable.InternalKeys.Empty();
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindSystemVariableAsString(const FString Key, FString& Result, bool& bReturnValue)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		const EShidenVariableType* type = ShidenSubsystem->SystemVariable.InternalKeys.Find(Key);
		if (type)
		{
			switch (*type)
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
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::MakeLocalVariableKey(const FString ProcessName, FString& LocalKey, bool& bSuccess)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		LocalKey = FString();
		bSuccess = false;
		return;
	}
	LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	return;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::RemoveLocalVariable(const FString ProcessName, const FString Key)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
	bool bSuccess = false;
	const FString LocalKey = UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
	if (!bSuccess)
	{
		return;
	}
	FShidenVariable* LocalVariables = ShidenSubsystem->LocalVariables.Find(LocalKey);

	if (LocalVariables)
	{
		const EShidenVariableType* type = LocalVariables->InternalKeys.Find(Key);

		if (type)
		{
			switch (*type)
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (ShidenSubsystem)
	{
		ShidenSubsystem->LocalVariables.Empty();
	}
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindLocalVariable(const FString ProcessName, const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		bReturnValue = false;
		return;
	}
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
	const EShidenVariableType* type = LocalVariables->InternalKeys.Find(Key);
	VariableType = *type;
	if (type)
	{
		switch (*type)
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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		bReturnValue = false;
		return;
	}
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
	const EShidenVariableType* type = LocalVariables->InternalKeys.Find(Key);
	if (type)
	{
		switch (*type)
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

FORCEINLINE bool PredefinedVariableKeyEquals(const FString Key, const FString PropertyName)
{
	return Key.Compare(PropertyName, ESearchCase::IgnoreCase) == 0;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::FindPredefinedSystemVariable(const FString Key, EShidenVariableType& VariableType, bool& bBooleanValue, FString& StringValue, int32& IntegerValue, float& FloatValue, FVector2D& Vector2Value, FVector& Vector3Value, bool& bReturnValue)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		bReturnValue = false;
		return;
	}

	bReturnValue = true;

	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::AutoRemoveWidgetOnEnd)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bAutoRemoveWidgetOnEnd;
		VariableType = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::BgmVolumeRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate;
		VariableType = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::LanguageIndex)))
	{
		IntegerValue = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
		VariableType = EShidenVariableType::Integer;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::MappingContext)))
	{
		StringValue = ShidenSubsystem->PredefinedSystemVariable.MappingContext.GetPath();
		VariableType = EShidenVariableType::String;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::MarkedReadLines)))
	{
		IntegerValue = ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines.Num();
		VariableType = EShidenVariableType::Integer;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SecondsToWaitForEachLetter)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.SecondsToWaitForEachLetter;
		VariableType = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SeVolumeRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate;
		VariableType = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::ShowMouseCursorWhenStarting)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bShowMouseCursorWhenStarting;
		VariableType = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SkipSpeedRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate;
		VariableType = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::SkipUnread)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bSkipUnread;
		VariableType = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::StopVoiceWhenNextDialogueStart)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bStopVoiceWhenNextDialogueStart;
		VariableType = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::TargetFrameRate)))
	{
		IntegerValue = ShidenSubsystem->PredefinedSystemVariable.TargetFrameRate;
		VariableType = EShidenVariableType::Integer;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::UseAddToPlayerScreenInsteadOfAddToViewport)))
	{
		bBooleanValue = ShidenSubsystem->PredefinedSystemVariable.bUseAddToPlayerScreenInsteadOfAddToViewport;
		VariableType = EShidenVariableType::Boolean;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::VoiceVolumeRate)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate;
		VariableType = EShidenVariableType::Float;
		return;
	}
	if (PredefinedVariableKeyEquals(Key, PROPERTY_NAME_OF(FShidenPredefinedSystemVariable::WaitTimeOnAutoMode)))
	{
		FloatValue = ShidenSubsystem->PredefinedSystemVariable.WaitTimeOnAutoMode;
		VariableType = EShidenVariableType::Float;
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

	TArray<FString> result;

	const FRegexPattern GetVariablePattern = FRegexPattern(FString(TEXT("(?<!\\\\)\\{\\s*([^ \\}]+?)\\s*(?<!\\\\)\\}")));
	FRegexMatcher matcher(GetVariablePattern, text);

	while (matcher.FindNext())
	{
		result.AddUnique(matcher.GetCaptureGroup(1));
	}

	return result;
}

FString ReplaceTextWithVariable(const FString ProcessName, const FString text)
{
	if (!text.Contains(TEXT("{")) || !text.Contains(TEXT("}")))
	{
		return text;
	}

	FString resultText = text;

	const FRegexPattern ReplaceTextPattern = FRegexPattern(FString(TEXT("((?<!\\\\)\\{ *[^ \\}]+ *(?<!\\\\)\\})")));
	FRegexMatcher matcher(ReplaceTextPattern, text);

	while (matcher.FindNext())
	{
		FString str = matcher.GetCaptureGroup(1);
		FString variableName = str.Mid(1, str.Len() - 2).TrimStartAndEnd();

		FString variableKind, replacementText, variableKey;
		bool bReturnValue;
		variableName.Split(TEXT("::"), &variableKind, &variableKey, ESearchCase::CaseSensitive);

		if (variableKey.IsEmpty())
		{
			// UserVariable
			UShidenVariableFunctionLibrary::FindUserVariableAsString(variableName.Replace(TEXT("\\:"), TEXT(":")), replacementText, bReturnValue);
			if (!bReturnValue)
			{
				replacementText = TEXT("Error");
			}
		}
		else if (variableKind == TEXT("System"))
		{
			UShidenVariableFunctionLibrary::FindSystemVariableAsString(variableKey.Replace(TEXT("\\:"), TEXT(":")), replacementText, bReturnValue);
			if (!bReturnValue)
			{
				replacementText = TEXT("Error");
			}
		}
		else if (variableKind == TEXT("Local"))
		{
			UShidenVariableFunctionLibrary::FindLocalVariableAsString(ProcessName, variableKey.Replace(TEXT("\\:"), TEXT(":")), replacementText, bReturnValue);
			if (!bReturnValue)
			{
				replacementText = TEXT("Error");
			}
		}
		else if (variableKind == TEXT("Predefined"))
		{
			UShidenVariableFunctionLibrary::FindPredefinedSystemVariableAsString(variableKey.Replace(TEXT("\\:"), TEXT(":")), replacementText, bReturnValue);
			if (!bReturnValue)
			{
				replacementText = TEXT("Error");
			}
		}
		else
		{
			replacementText = TEXT("Error");
		}

		resultText.ReplaceInline(*str, *replacementText, ESearchCase::CaseSensitive);
	}

	return resultText;
}

SHIDENCORE_API FShidenCommand UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(const FString ProcessName, const FShidenCommand Command)
{
	FShidenCommand Result = Command;
	Result.Arg1 = ReplaceTextWithVariable(ProcessName, Command.Arg1);
	Result.Arg2 = ReplaceTextWithVariable(ProcessName, Command.Arg2);
	Result.Arg3 = ReplaceTextWithVariable(ProcessName, Command.Arg3);
	Result.Arg4 = ReplaceTextWithVariable(ProcessName, Command.Arg4);
	Result.Arg5 = ReplaceTextWithVariable(ProcessName, Command.Arg5);
	Result.Arg6 = ReplaceTextWithVariable(ProcessName, Command.Arg6);
	Result.Arg7 = ReplaceTextWithVariable(ProcessName, Command.Arg7);
	Result.Arg8 = ReplaceTextWithVariable(ProcessName, Command.Arg8);
	Result.Arg9 = ReplaceTextWithVariable(ProcessName, Command.Arg9);
	Result.Arg10 = ReplaceTextWithVariable(ProcessName, Command.Arg10);
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

SHIDENCORE_API void UShidenVariableFunctionLibrary::InitPredefindSystemVariable()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->PredefinedSystemVariable = ShidenProjectConfig->PredefinedSystemVariable;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSkipSpeedRate(float& SkipSpeedRate)
{
	SkipSpeedRate = 0.0f;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	SkipSpeedRate = ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetVoiceVolumeRate(float& VoiceVolumeRate)
{
	VoiceVolumeRate = 0.0f;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	VoiceVolumeRate = ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSeVolumeRate(float& SeVolumeRate)
{
	SeVolumeRate = 0.0f;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	SeVolumeRate = ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetBgmVolumeRate(float& BgmVolumeRate)
{
	BgmVolumeRate = 0.0f;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	BgmVolumeRate = ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetShowMouseCursorWhenStarting(bool& bShowMouseCursorWhenStarting)
{
	bShowMouseCursorWhenStarting = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	bShowMouseCursorWhenStarting = ShidenSubsystem->PredefinedSystemVariable.bShowMouseCursorWhenStarting;
}


SHIDENCORE_API void UShidenVariableFunctionLibrary::GetLanguageIndex(int& LanguageIndex)
{
	LanguageIndex = 0;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	LanguageIndex = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSecondsToWaitForEachLetter(float& SecondsToWaitForEachLetter)
{
	SecondsToWaitForEachLetter = 0.0f;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	SecondsToWaitForEachLetter = ShidenSubsystem->PredefinedSystemVariable.SecondsToWaitForEachLetter;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetWaitTimeOnAutoMode(float& WaitTimeOnAutoMode)
{
	WaitTimeOnAutoMode = 0.0f;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	WaitTimeOnAutoMode = ShidenSubsystem->PredefinedSystemVariable.WaitTimeOnAutoMode;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetStopVoiceWhenNextDialogueStart(bool& bStopVoiceWhenNextDialogueStart)
{
	bStopVoiceWhenNextDialogueStart = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	bStopVoiceWhenNextDialogueStart = ShidenSubsystem->PredefinedSystemVariable.bStopVoiceWhenNextDialogueStart;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetMappingContext(UInputMappingContext*& MappingContext)
{
	MappingContext = nullptr;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	MappingContext = ShidenSubsystem->PredefinedSystemVariable.MappingContext;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetUseAddToPlayerScreenInsteadOfAddToViewport(bool& bUseAddToPlayerScreenInsteadOfAddToViewport)
{
	bUseAddToPlayerScreenInsteadOfAddToViewport = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	bUseAddToPlayerScreenInsteadOfAddToViewport = ShidenSubsystem->PredefinedSystemVariable.bUseAddToPlayerScreenInsteadOfAddToViewport;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetAutoRemoveWidgetOnEnd(bool& bAutoRemoveWidgetOnEnd)
{
	bAutoRemoveWidgetOnEnd = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	bAutoRemoveWidgetOnEnd = ShidenSubsystem->PredefinedSystemVariable.bAutoRemoveWidgetOnEnd;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetSkipUnread(bool& bSkipUnread)
{
	bSkipUnread = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	bSkipUnread = ShidenSubsystem->PredefinedSystemVariable.bSkipUnread;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetTargetFrameRate(int& TargetFrameRate)
{
	TargetFrameRate = 0;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	TargetFrameRate = ShidenSubsystem->PredefinedSystemVariable.TargetFrameRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::GetMarkedReadLines(TMap<FGuid, FShidenReadLines>& MarkedReadLines)
{
	MarkedReadLines.Empty();

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	MarkedReadLines = ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSkipSpeedRate(const float SkipSpeedRate)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate = SkipSpeedRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetVoiceVolumeRate(const float VoiceVolumeRate)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate = VoiceVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSeVolumeRate(const float SeVolumeRate)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate = SeVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetBgmVolumeRate(const float BgmVolumeRate)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate = BgmVolumeRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetShowMouseCursorWhenStarting(const bool bShowMouseCursorWhenStarting)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.bShowMouseCursorWhenStarting = bShowMouseCursorWhenStarting;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetLanguageIndex(const int LanguageIndex)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.LanguageIndex = LanguageIndex;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSecondsToWaitForEachLetter(const float SecondsToWaitForEachLetter)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.SecondsToWaitForEachLetter = SecondsToWaitForEachLetter;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetWaitTimeOnAutoMode(const float WaitTimeOnAutoMode)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.WaitTimeOnAutoMode = WaitTimeOnAutoMode;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetStopVoiceWhenNextDialogueStart(const bool bStopVoiceWhenNextDialogueStart)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.bStopVoiceWhenNextDialogueStart = bStopVoiceWhenNextDialogueStart;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetMappingContext(UInputMappingContext* MappingContext)
{
	if (!MappingContext)
	{
		return;
	}

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.MappingContext = MappingContext;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetUseAddToPlayerScreenInsteadOfAddToViewport(const bool bUseAddToPlayerScreenInsteadOfAddToViewport)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.bUseAddToPlayerScreenInsteadOfAddToViewport = bUseAddToPlayerScreenInsteadOfAddToViewport;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetAutoRemoveWidgetOnEnd(const bool bAutoRemoveWidgetOnEnd)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.bAutoRemoveWidgetOnEnd = bAutoRemoveWidgetOnEnd;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetSkipUnread(const bool bSkipUnread)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.bSkipUnread = bSkipUnread;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetTargetFrameRate(const int TargetFrameRate)
{
	if (TargetFrameRate <= 0)
	{
		return;
	}

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.TargetFrameRate = TargetFrameRate;
}

SHIDENCORE_API void UShidenVariableFunctionLibrary::SetMarkedReadLines(const TMap<FGuid, FShidenReadLines>& MarkedReadLines)
{
	if (MarkedReadLines.Num() == 0)
	{
		return;
	}

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines = MarkedReadLines;
}


SHIDENCORE_API void AddPredefinedVariable(const EShidenVariableType& Type, const FString Key, const bool bBooleanValue, const int32 IntegerValue, const float FloatValue, bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	if (Key.IsEmpty())
	{
		ErrorMessage = TEXT("Invalid Key Name");
		return;
	}

	FString CleanedKey = Key.Replace(TEXT(" "), TEXT(""));

	if (CleanedKey.Equals(TEXT("SkipSpeedRate"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetSkipSpeedRate(FloatValue);
	}
	else if (CleanedKey.Equals(TEXT("VoiceVolumeRate"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetVoiceVolumeRate(FloatValue);
	}
	else if (CleanedKey.Equals(TEXT("SeVolumeRate"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetSeVolumeRate(FloatValue);
	}
	else if (CleanedKey.Equals(TEXT("BgmVolumeRate"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetBgmVolumeRate(FloatValue);
	}
	else if (CleanedKey.Equals(TEXT("ShowMouseCursorWhenStarting"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetShowMouseCursorWhenStarting(bBooleanValue);
	}
	else if (CleanedKey.Equals(TEXT("LanguageIndex"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Integer)
		{
			ErrorMessage = TEXT("Type must be integer");
			return;
		}
		UShidenVariableFunctionLibrary::SetLanguageIndex(IntegerValue);
	}
	else if (CleanedKey.Equals(TEXT("SecondsToWaitForEachLetter"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetSecondsToWaitForEachLetter(FloatValue);
	}
	else if (CleanedKey.Equals(TEXT("WaitTimeOnAutoMode"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Float)
		{
			ErrorMessage = TEXT("Type must be float");
			return;
		}
		UShidenVariableFunctionLibrary::SetWaitTimeOnAutoMode(FloatValue);
	}
	else if (CleanedKey.Equals(TEXT("StopVoiceWhenNextDialogueStart"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetStopVoiceWhenNextDialogueStart(bBooleanValue);
	}
	else if (CleanedKey.Equals(TEXT("MappingContext"), ESearchCase::IgnoreCase))
	{
		ErrorMessage = TEXT("MappingContext not supported");
		return;
	}
	else if (CleanedKey.Equals(TEXT("UseAddToPlayerScreenInsteadOfAddToViewport"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetUseAddToPlayerScreenInsteadOfAddToViewport(bBooleanValue);
	}
	else if (CleanedKey.Equals(TEXT("AutoRemoveWidgetOnEnd"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetAutoRemoveWidgetOnEnd(bBooleanValue);
	}
	else if (CleanedKey.Equals(TEXT("SkipUnread"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Boolean)
		{
			ErrorMessage = TEXT("Type must be boolean");
			return;
		}
		UShidenVariableFunctionLibrary::SetSkipUnread(bBooleanValue);
	}
	else if (CleanedKey.Equals(TEXT("TargetFrameRate"), ESearchCase::IgnoreCase))
	{
		if (Type != EShidenVariableType::Integer)
		{
			ErrorMessage = TEXT("Type must be integer");
			return;
		}
		UShidenVariableFunctionLibrary::SetTargetFrameRate(IntegerValue);
	}
	else if (CleanedKey.Equals(TEXT("MarkedReadLines"), ESearchCase::IgnoreCase))
	{
		ErrorMessage = TEXT("MarkedReadLines not supported");
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

SHIDENCORE_API void AddLocalVariable(const FString ProcessName, const EShidenVariableType& Type, const FString Key, const bool& bBooleanValue, const FString StringValue, const int IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value)
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

SHIDENCORE_API void AddSystemVariable(const EShidenVariableType& Type, const FString Key, const bool& bBooleanValue, const FString StringValue, const int IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value)
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

SHIDENCORE_API void AddUserVariable(const EShidenVariableType& Type, const FString Key, const bool& bBooleanValue, const FString StringValue, const int IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value)
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

SHIDENCORE_API void UShidenVariableFunctionLibrary::AddVariable(const FString ProcessName, const EShidenVariableKind Kind, const EShidenVariableType Type, const FString Key, const bool bBooleanValue, const FString StringValue, const int IntegerValue, const float FloatValue, const FVector2D Vector2Value, const FVector Vector3Value, bool& bSuccess, FString& ErrorMessage)
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
			AddPredefinedVariable(Type, Key, bBooleanValue, IntegerValue, FloatValue, bSuccess, ErrorMessage);
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

SHIDENCORE_API void UShidenVariableFunctionLibrary::EvaluateCondition(const EShidenVariableType Type, const FString Operator, const bool bABooleanValue, const FString AStringValue, const int AIntegerValue, const float AFloatValue, const FVector2D AVector2Value, const FVector AVector3Value,
	const bool bBBooleanValue, const FString BStringValue, const int BIntegerValue, const float BFloatValue, const FVector2D BVector2Value, const FVector BVector3Value, bool& bResult, bool& bSuccess, FString& ErrorMessage)
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
