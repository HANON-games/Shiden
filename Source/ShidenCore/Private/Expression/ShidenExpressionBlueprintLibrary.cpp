// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Expression/ShidenExpressionBlueprintLibrary.h"
#include "Expression/ShidenExpressionEvaluator.h"
#include "Internationalization/Regex.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionInteger(const FString& ProcessName, const FString& Expression, int32& Result, FString& ErrorMessage)
{
	return TryEvaluateExpressionIntegerRaw(ReplaceVariablesInExpression(ProcessName, Expression), Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionFloat(const FString& ProcessName, const FString& Expression, float& Result, FString& ErrorMessage)
{
	return TryEvaluateExpressionFloatRaw(ReplaceVariablesInExpression(ProcessName, Expression), Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionBoolean(const FString& ProcessName, const FString& Expression, bool& Result, FString& ErrorMessage)
{
	return TryEvaluateExpressionBooleanRaw(ReplaceVariablesInExpression(ProcessName, Expression), Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionString(const FString& ProcessName, const FString& Expression, FString& Result, FString& ErrorMessage)
{
	return TryEvaluateExpressionStringRaw(ReplaceVariablesInExpression(ProcessName, Expression), Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionVector2(const FString& ProcessName, const FString& Expression, FVector2D& Result, FString& ErrorMessage)
{
	return TryEvaluateExpressionVector2Raw(ReplaceVariablesInExpression(ProcessName, Expression), Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionVector3(const FString& ProcessName, const FString& Expression, FVector& Result, FString& ErrorMessage)
{
	return TryEvaluateExpressionVector3Raw(ReplaceVariablesInExpression(ProcessName, Expression), Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryValidateExpression(const FString& Expression, FString& ErrorMessage)
{
	return FShidenExpressionEvaluator::TryValidate(Expression, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionIntegerRaw(const FString& Expression, int32& Result, FString& ErrorMessage)
{
	FShidenExpressionValue Value;
	if (!FShidenExpressionEvaluator::TryEvaluate(Expression, Value, ErrorMessage))
	{
		return false;
	}

	return Value.TryToInteger(Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionFloatRaw(const FString& Expression, float& Result, FString& ErrorMessage)
{
	FShidenExpressionValue Value;
	if (!FShidenExpressionEvaluator::TryEvaluate(Expression, Value, ErrorMessage))
	{
		return false;
	}

	return Value.TryToNumeric(Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionBooleanRaw(const FString& Expression, bool& Result, FString& ErrorMessage)
{
	FShidenExpressionValue Value;
	if (!FShidenExpressionEvaluator::TryEvaluate(Expression, Value, ErrorMessage))
	{
		return false;
	}

	return Value.TryToBoolean(Result, ErrorMessage);
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionStringRaw(const FString& Expression, FString& Result, FString& ErrorMessage)
{
	FShidenExpressionValue Value;
	if (!FShidenExpressionEvaluator::TryEvaluate(Expression, Value, ErrorMessage))
	{
		return false;
	}

	Result = Value.ToString();
	return true;
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionVector2Raw(const FString& Expression, FVector2D& Result, FString& ErrorMessage)
{
	FShidenExpressionValue Value;
	if (!FShidenExpressionEvaluator::TryEvaluate(Expression, Value, ErrorMessage))
	{
		return false;
	}

	if (Value.Type != EShidenExpressionValueType::Vector2)
	{
		ErrorMessage = TEXT("Result is not a Vector2");
		return false;
	}

	Result = Value.Vector2Value;
	return true;
}

bool UShidenExpressionBlueprintLibrary::TryEvaluateExpressionVector3Raw(const FString& Expression, FVector& Result, FString& ErrorMessage)
{
	FShidenExpressionValue Value;
	if (!FShidenExpressionEvaluator::TryEvaluate(Expression, Value, ErrorMessage))
	{
		return false;
	}

	if (Value.Type != EShidenExpressionValueType::Vector3)
	{
		ErrorMessage = TEXT("Result is not a Vector3");
		return false;
	}

	Result = Value.Vector3Value;
	return true;
}

bool UShidenExpressionBlueprintLibrary::TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey)
{
	if (const FShidenScenarioProgressStack* Stack = GEngine->GetEngineSubsystem<UShidenSubsystem>()->ScenarioProgressStack.Find(ProcessName))
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

FString UShidenExpressionBlueprintLibrary::ReplaceVariablesInExpression(const FString& ProcessName, const FString& Expression)
{
	if (!Expression.Contains(TEXT("{")) || !Expression.Contains(TEXT("}")))
	{
		return Expression;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	FString ResultText = Expression;
	FRegexMatcher Matcher(UShidenVariableBlueprintLibrary::GetReplaceTextPattern(), Expression);

	while (Matcher.FindNext())
	{
		FString Str = Matcher.GetCaptureGroup(1);
		FString VariableName = Str.Mid(1, Str.Len() - 2).TrimStartAndEnd();
		FString VariableKind, ReplacementText, VariableKey;
		VariableName.Split(TEXT("::"), &VariableKind, &VariableKey, ESearchCase::CaseSensitive);

		FShidenVariableDefinition VariableDefinition;

		if (VariableKey.IsEmpty())
		{
			// User variable (no scope prefix)
			if (!ShidenSubsystem->UserVariable.TryGetDefinition(VariableName.Replace(TEXT("\\:"), TEXT(":")), VariableDefinition))
			{
				ReplacementText = TEXT("");
			}
			else
			{
				switch (VariableDefinition.Type)
				{
				case EShidenVariableType::Vector2:
					{
						FVector2D Vector2DValue;
						if (ShidenSubsystem->UserVariable.TryGet(VariableDefinition.Name, Vector2DValue))
						{
							// Vector2Dを[X, Y]形式に置き換え
							ReplacementText = FString::Printf(TEXT("[%g, %g]"), Vector2DValue.X, Vector2DValue.Y);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				case EShidenVariableType::Vector3:
					{
						FVector VectorValue;
						if (ShidenSubsystem->UserVariable.TryGet(VariableDefinition.Name, VectorValue))
						{
							// Vector3を[X, Y, Z]形式に置き換え
							ReplacementText = FString::Printf(TEXT("[%g, %g, %g]"), VectorValue.X, VectorValue.Y, VectorValue.Z);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				case EShidenVariableType::String:
					{
						FString StringValue;
						if (ShidenSubsystem->UserVariable.TryGet(VariableDefinition.Name, StringValue))
						{
							// 文字列をダブルクオーテーションで囲む
							ReplacementText = FString::Printf(TEXT("\"%s\""), *StringValue);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				default:
					{
						EShidenVariableType Type;
						if (!ShidenSubsystem->UserVariable.TryGetAsString(VariableDefinition.Name, Type, ReplacementText))
						{
							ReplacementText = TEXT("Error");
						}
					}
				}
			}
		}
		else if (VariableKind == TEXT("System"))
		{
			if (!ShidenSubsystem->SystemVariable.TryGetDefinition(VariableKey.Replace(TEXT("\\:"), TEXT(":")), VariableDefinition))
			{
				ReplacementText = TEXT("");
			}
			else
			{
				switch (VariableDefinition.Type)
				{
				case EShidenVariableType::Vector2:
					{
						FVector2D Vector2DValue;
						if (ShidenSubsystem->SystemVariable.TryGet(VariableDefinition.Name, Vector2DValue))
						{
							// Vector2Dを[X, Y]形式に置き換え
							ReplacementText = FString::Printf(TEXT("[%g, %g]"), Vector2DValue.X, Vector2DValue.Y);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				case EShidenVariableType::Vector3:
					{
						FVector VectorValue;
						if (ShidenSubsystem->SystemVariable.TryGet(VariableDefinition.Name, VectorValue))
						{
							// Vector3を[X, Y, Z]形式に置き換え
							ReplacementText = FString::Printf(TEXT("[%g, %g, %g]"), VectorValue.X, VectorValue.Y, VectorValue.Z);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				case EShidenVariableType::String:
					{
						FString StringValue;
						if (ShidenSubsystem->SystemVariable.TryGet(VariableDefinition.Name, StringValue))
						{
							// 文字列をダブルクオーテーションで囲む
							ReplacementText = FString::Printf(TEXT("\"%s\""), *StringValue);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				default:
					{
						EShidenVariableType Type;
						if (!ShidenSubsystem->SystemVariable.TryGetAsString(VariableDefinition.Name, Type, ReplacementText))
						{
							ReplacementText = TEXT("Error");
						}
					}
				}
			}
		}
		else if (VariableKind == TEXT("Local"))
		{
			FString ScopeKey;
			if (!TryCreateScopeKey(ProcessName, ScopeKey)
				|| !ShidenSubsystem->LocalVariable.TryGetDefinition(ScopeKey, VariableKey.Replace(TEXT("\\:"), TEXT(":")), VariableDefinition))
			{
				ReplacementText = TEXT("");
			}
			else
			{
				switch (VariableDefinition.Type)
				{
				case EShidenVariableType::Vector2:
					{
						FVector2D Vector2DValue;
						if (ShidenSubsystem->LocalVariable.TryGet(ScopeKey, VariableDefinition.Name, Vector2DValue))
						{
							// Vector2Dを[X, Y]形式に置き換え
							ReplacementText = FString::Printf(TEXT("[%g, %g]"), Vector2DValue.X, Vector2DValue.Y);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				case EShidenVariableType::Vector3:
					{
						FVector VectorValue;
						if (ShidenSubsystem->LocalVariable.TryGet(ScopeKey, VariableDefinition.Name, VectorValue))
						{
							// Vector3を[X, Y, Z]形式に置き換え
							ReplacementText = FString::Printf(TEXT("[%g, %g, %g]"), VectorValue.X, VectorValue.Y, VectorValue.Z);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				case EShidenVariableType::String:
					{
						FString StringValue;
						if (ShidenSubsystem->LocalVariable.TryGet(ScopeKey, VariableDefinition.Name, StringValue))
						{
							// 文字列をダブルクオーテーションで囲む
							ReplacementText = FString::Printf(TEXT("\"%s\""), *StringValue);
						}
						else
						{
							ReplacementText = TEXT("Error");
						}
						break;
					}
				default:
					{
						EShidenVariableType Type;
						if (!ShidenSubsystem->LocalVariable.TryGetAsString(ScopeKey, VariableDefinition.Name, Type, ReplacementText))
						{
							ReplacementText = TEXT("Error");
						}
					}
				}
			}
		}
		else if (VariableKind == TEXT("Predefined"))
		{
			// NOTE: ShidenPredefinedSystemVariables has no vector2, vector3 type variables, so no need to handle them here.
			EShidenVariableType Type;
			if (!UShidenVariableBlueprintLibrary::TryFindPredefinedSystemVariableAsString(VariableKey.Replace(TEXT("\\:"), TEXT(":")), Type,
			                                                                              ReplacementText))
			{
				ReplacementText = TEXT("Error");
			}
			// 文字列型の場合はダブルクオーテーションで囲む
			else if (Type == EShidenVariableType::String)
			{
				ReplacementText = FString::Printf(TEXT("\"%s\""), *ReplacementText);
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
