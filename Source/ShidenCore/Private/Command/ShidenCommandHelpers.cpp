// Copyright (c) 2026 HANON. All Rights Reserved.

#include "ShidenCommandHelpers.h"
#include "Expression/ShidenExpressionBlueprintLibrary.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Scenario/ShidenScenario.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenStructuredLog.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

namespace ShidenCommandHelpers
{
	bool TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage)
	{
		static const TMap<FString, EEasingFunc::Type> EasingFuncMap = {
			{TEXT("linear"), EEasingFunc::Linear},
			{TEXT("step"), EEasingFunc::Step},
			{TEXT("sinusoidal in"), EEasingFunc::SinusoidalIn},
			{TEXT("sinusoidal out"), EEasingFunc::SinusoidalOut},
			{TEXT("sinusoidal in out"), EEasingFunc::SinusoidalInOut},
			{TEXT("ease in"), EEasingFunc::EaseIn},
			{TEXT("ease out"), EEasingFunc::EaseOut},
			{TEXT("ease in out"), EEasingFunc::EaseInOut},
			{TEXT("expo in"), EEasingFunc::ExpoIn},
			{TEXT("expo out"), EEasingFunc::ExpoOut},
			{TEXT("expo in out"), EEasingFunc::ExpoInOut},
			{TEXT("circular in"), EEasingFunc::CircularIn},
			{TEXT("circular out"), EEasingFunc::CircularOut},
			{TEXT("circular in out"), EEasingFunc::CircularInOut}
		};

		if (const EEasingFunc::Type* FoundCurve = EasingFuncMap.Find(EasingFuncStr.ToLower()))
		{
			EasingFunc = *FoundCurve;
			return true;
		}

		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EEasingFunc::Type."), *EasingFuncStr);
		return false;
	}

	bool TryConvertToAudioFaderCurve(const FString& AudioFaderCurveStr, EAudioFaderCurve& AudioFaderCurve, FString& ErrorMessage)
	{
		static const TMap<FString, EAudioFaderCurve> CurveMap = {
			{TEXT("linear"), EAudioFaderCurve::Linear},
			{TEXT("logarithmic"), EAudioFaderCurve::Logarithmic},
			{TEXT("sin (s-curve)"), EAudioFaderCurve::SCurve},
			{TEXT("sin (equal power)"), EAudioFaderCurve::Sin}
		};

		if (const EAudioFaderCurve* FoundCurve = CurveMap.Find(AudioFaderCurveStr.ToLower()))
		{
			AudioFaderCurve = *FoundCurve;
			return true;
		}

		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EAudioFaderCurve."), *AudioFaderCurveStr);
		AudioFaderCurve = EAudioFaderCurve::Linear;
		return false;
	}

	bool TryGetCurrentOriginalCommand(const FString& ProcessName, FShidenCommand*& OriginalCommand, FString& ErrorMessage)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
		const FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
		if (!ProgressStack || ProgressStack->IsEmpty())
		{
			ErrorMessage = TEXT("ScenarioProgressStack is empty.");
			return false;
		}

		const FShidenScenarioProgress CurrentProgress = ProgressStack->Stack.Last();
		UShidenScenario* Scenario = nullptr;
		if (!UShidenScenarioBlueprintLibrary::TryGetScenario(CurrentProgress.ScenarioId, Scenario))
		{
			ErrorMessage = TEXT("GetScenario failed.");
			return false;
		}

		if (!Scenario->Commands.IsValidIndex(CurrentProgress.CurrentIndex))
		{
			ErrorMessage = TEXT("Command index out of bounds.");
			return false;
		}

		OriginalCommand = &Scenario->Commands[CurrentProgress.CurrentIndex];
		return true;
	}
}

namespace ShidenConditionalCommandHelpers
{
	bool TryParseVariableCondition(const FShidenCommand& Command, FVariableConditionArgs& Args, FString& ErrorMessage)
	{
		const FString VariableKindStr = Command.GetArg(TEXT("VariableKind"));
		Args.VariableName = Command.GetArg(TEXT("VariableName"));
		Args.Operator = Command.GetArg(TEXT("Operator"));
		Args.RightHandValue = Command.GetArg(TEXT("RightHandValue"));

		if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(VariableKindStr, Args.VariableKind))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *VariableKindStr);
			return false;
		}

		return true;
	}

	bool TryEvaluateVariableCondition(const FString& ProcessName, const FVariableConditionArgs& Args, bool& bResult, FString& ErrorMessage)
	{
		EShidenVariableType VariableType;
		bool bBooleanValue;
		FString StringValue;
		int32 IntegerValue;
		float FloatValue;
		FVector2D Vector2Value;
		FVector Vector3Value;

		if (!UShidenVariableBlueprintLibrary::TryFindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType, bBooleanValue,
		                                                      StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, ErrorMessage))
		{
			return false;
		}

		switch (VariableType)
		{
		case EShidenVariableType::Boolean:
			{
				const bool bRightHandValue = Args.RightHandValue.ToBool();
				return UShidenVariableBlueprintLibrary::TryEvaluateBoolean(Args.Operator, bBooleanValue, bRightHandValue, bResult, ErrorMessage);
			}
		case EShidenVariableType::String:
		case EShidenVariableType::AssetPath:
			{
				return UShidenVariableBlueprintLibrary::TryEvaluateString(Args.Operator, StringValue, Args.RightHandValue, bResult, ErrorMessage);
			}
		case EShidenVariableType::Integer:
			{
				const int32 RightHandValueInt = FCString::Atoi(*Args.RightHandValue);
				return UShidenVariableBlueprintLibrary::TryEvaluateInteger(Args.Operator, IntegerValue, RightHandValueInt, bResult, ErrorMessage);
			}
		case EShidenVariableType::Float:
			{
				const float RightHandValueFloat = FCString::Atof(*Args.RightHandValue);
				return UShidenVariableBlueprintLibrary::TryEvaluateFloat(Args.Operator, FloatValue, RightHandValueFloat, bResult, ErrorMessage);
			}
		case EShidenVariableType::Vector2:
			{
				FVector2D RightHandValueVec2;
				if (!RightHandValueVec2.InitFromString(Args.RightHandValue))
				{
					ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Args.RightHandValue);
					return false;
				}
				return UShidenVariableBlueprintLibrary::TryEvaluateVector2(Args.Operator, Vector2Value, RightHandValueVec2, bResult, ErrorMessage);
			}
		case EShidenVariableType::Vector3:
			{
				FVector RightHandValueVec3;
				if (!RightHandValueVec3.InitFromString(Args.RightHandValue))
				{
					ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector."), *Args.RightHandValue);
					return false;
				}
				return UShidenVariableBlueprintLibrary::TryEvaluateVector3(Args.Operator, Vector3Value, RightHandValueVec3, bResult, ErrorMessage);
			}
		default:
			SHIDEN_WARNING("Unknown variable type for variable '{name}'.", *Args.VariableName);
			ErrorMessage = FString::Printf(TEXT("Unknown variable type for variable '%s'."), *Args.VariableName);
			return false;
		}
	}

	bool TryParseExpressionCondition(const FShidenCommand& Command, FExpressionConditionArgs& Args, FString& ErrorMessage)
	{
		Args.Expression = Command.GetArg(TEXT("Expression"));

		if (Args.Expression.IsEmpty())
		{
			ErrorMessage = TEXT("Expression is empty");
			return false;
		}

		return true;
	}

	bool TryEvaluateExpressionCondition(const FString& ProcessName, const FExpressionConditionArgs& Args, bool& bResult, FString& ErrorMessage)
	{
		return UShidenExpressionBlueprintLibrary::TryEvaluateExpressionBoolean(ProcessName, Args.Expression, bResult, ErrorMessage);
	}

	bool TrySkipToNextIfBranch(const FString& ProcessName, const int32 StartIndex, FString& CommandName,
	                           int32& ResultIndex, FString& ErrorMessage)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

		FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
		if (!ProgressStack || ProgressStack->IsEmpty())
		{
			ErrorMessage = TEXT("ScenarioProgressStack is empty.");
			return false;
		}

		const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
		UShidenScenario* Scenario = nullptr;
		if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
		{
			ErrorMessage = TEXT("GetScenario failed.");
			return false;
		}

		int32 Depth = 0;
		for (int32 Index = StartIndex; Index < Scenario->Commands.Num(); Index++)
		{
			const FShidenCommand& Command = Scenario->Commands[Index];
			if (!Command.bEnabled)
			{
				continue;
			}

			if (Command.CommandName == TEXT("If") || Command.CommandName == TEXT("IfExpression"))
			{
				Depth++;
			}
			else if (Command.CommandName == TEXT("ElseIf") || Command.CommandName == TEXT("ElseIfExpression") || Command.CommandName == TEXT("Else"))
			{
				if (Depth == 0)
				{
					CommandName = Command.CommandName;
					ResultIndex = Index;
					return true;
				}
			}
			else if (Command.CommandName == TEXT("EndIf"))
			{
				if (Depth == 0)
				{
					CommandName = Command.CommandName;
					ResultIndex = Index;
					return true;
				}
				// Since we already returned when Depth == 0, Depth > 0 is guaranteed here, so it won't go below 0
				Depth--;
			}
		}

		ErrorMessage = TEXT("Failed to find ElseIf, ElseIfExpression, Else, or EndIf command.");
		return false;
	}

	bool TryFindEndIfIndex(const FString& ProcessName, const int32 StartIndex, int32& ResultIndex, FString& ErrorMessage)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

		FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
		if (!ProgressStack || ProgressStack->IsEmpty())
		{
			ErrorMessage = TEXT("ScenarioProgressStack is empty.");
			return false;
		}

		const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
		UShidenScenario* Scenario = nullptr;
		if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
		{
			ErrorMessage = TEXT("GetScenario failed.");
			return false;
		}

		int32 Depth = 0;
		for (int32 Index = StartIndex; Index < Scenario->Commands.Num(); Index++)
		{
			const FShidenCommand& Command = Scenario->Commands[Index];
			if (!Command.bEnabled)
			{
				continue;
			}

			if (Command.CommandName == TEXT("If") || Command.CommandName == TEXT("IfExpression"))
			{
				Depth++;
			}
			else if (Command.CommandName == TEXT("EndIf"))
			{
				if (Depth == 0)
				{
					ResultIndex = Index;
					return true;
				}
				// Since we already returned when Depth == 0, Depth > 0 is guaranteed here, so it won't go below 0
				Depth--;
			}
		}

		ErrorMessage = TEXT("Failed to find EndIf command.");
		return false;
	}

	bool TryFindEndLoopWhileIndex(const FString& ProcessName, const int32 StartIndex, int32& ResultIndex, FString& ErrorMessage)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

		FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
		if (!ProgressStack || ProgressStack->IsEmpty())
		{
			ErrorMessage = TEXT("ScenarioProgressStack is empty.");
			return false;
		}

		const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
		UShidenScenario* Scenario = nullptr;
		if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
		{
			ErrorMessage = TEXT("GetScenario failed.");
			return false;
		}

		int32 Depth = 0;
		for (int32 Index = StartIndex; Index < Scenario->Commands.Num(); Index++)
		{
			const FShidenCommand& Command = Scenario->Commands[Index];
			if (!Command.bEnabled)
			{
				continue;
			}

			if (Command.CommandName == TEXT("LoopWhile") || Command.CommandName == TEXT("LoopWhileExpression"))
			{
				Depth++;
			}
			else if (Command.CommandName == TEXT("EndLoopWhile"))
			{
				if (Depth == 0)
				{
					ResultIndex = Index;
					return true;
				}
				// Since we already returned when Depth == 0, Depth > 0 is guaranteed here, so it won't go below 0
				Depth--;
			}
		}

		ErrorMessage = TEXT("EndLoopWhile is not found.");
		return false;
	}
}
