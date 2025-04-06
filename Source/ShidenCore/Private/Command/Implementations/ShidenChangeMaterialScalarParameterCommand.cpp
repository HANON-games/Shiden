// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenChangeMaterialScalarParameterCommand.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenChangeMaterialScalarParameterCommand::TryParseCommand(const FShidenCommand& Command, UShidenWidget* Widget,
                                                                  FChangeMaterialScalarParameterCommandArgs& Args, FString& ErrorMessage)
{
	Args.Target = Command.GetArg("Target");
	Args.TargetName = Command.GetArg("TargetName");
	Args.ParameterName = Command.GetArg("ParameterName");
	const FString EasingFuncStr = Command.GetArg("EasingFunction");
	Args.Duration = Command.GetArgAsFloat("Duration");
	Args.ChangeType = Command.GetArg("ChangeType");
	const float OriginalEndValue = Command.GetArgAsFloat("EndValue");
	Args.Steps = Command.GetArgAsInt("Steps");
	Args.BlendExp = Command.GetArgAsFloat("BlendExp");
	Args.bWaitForCompletion = Command.GetArgAsBool("WaitForCompletion");

	if (!TryAddCurrentValue(Args, OriginalEndValue, Widget, Args.EndValue, ErrorMessage))
	{
		return false;
	}

	return TryConvertToEasingFunc(EasingFuncStr, Args.EasingFunction, ErrorMessage);
}

void UShidenChangeMaterialScalarParameterCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* Widget,
                                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                     UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                                     FString& ErrorMessage)
{
	for (const TPair<FString, FString>& Pair : ScenarioProperties)
	{
		FString TargetType, TargetName, ParameterName;
		Tie(TargetType, TargetName, ParameterName) = ParseScenarioPropertyKey(Pair.Key);

		Args = FChangeMaterialScalarParameterCommandArgs
		{
			.Target = TargetType,
			.TargetName = TargetName,
			.ParameterName = ParameterName,
			.EasingFunction = EEasingFunc::Linear,
			.Duration = 0,
			.ChangeType = TEXT("AbsoluteValue"),
			.EndValue = FCString::Atof(*Pair.Value),
			.Steps = 0,
			.BlendExp = 0,
			.bWaitForCompletion = true
		};

		if (!TryStartChangeParameter(Args, Widget, TEXT("Default"), ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenChangeMaterialScalarParameterCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
                                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                   UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Widget, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryStartChangeParameter(Args, Widget, ProcessName, ErrorMessage)
		         ? EShidenPreProcessStatus::Complete
		         : EShidenPreProcessStatus::Error;
}

void UShidenChangeMaterialScalarParameterCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                                UShidenWidget* Widget,
                                                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                const float DeltaTime, UObject* CallerObject,
																				EShidenProcessStatus& Status, FString& BreakReason,
                                                                                FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion)
	{
		const bool bShouldDelay = Args.Target == TEXT("Image")
			                          ? !Widget->IsImageMaterialParameterChangeCompleted(Args.TargetName, Args.ParameterName)
			                          : !Widget->IsRetainerBoxMaterialParameterChangeCompleted(Args.TargetName, Args.ParameterName);

		if (bShouldDelay)
		{
			Status = EShidenProcessStatus::DelayUntilNextTick;
			return;
		}
	}

	const FString Key = MakeScenarioPropertyKey(Args.Target, Args.TargetName, Args.ParameterName);
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Key, FString::SanitizeFloat(Args.EndValue));
	Status = EShidenProcessStatus::Next;
}

void UShidenChangeMaterialScalarParameterCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                const bool bIsCurrentCommand, EShidenPreviewStatus& Status,
                                                                                FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Widget, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		Args.Duration = 0;
	}

	Status = TryStartChangeParameter(Args, Widget, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenChangeMaterialScalarParameterCommand::TryAddCurrentValue(const FChangeMaterialScalarParameterCommandArgs& Args, const float OriginalEndValue,
                                                                     UShidenWidget* Widget, float& ResultValue, FString& ErrorMessage)
{
	if (Args.ChangeType != TEXT("AddToCurrent"))
	{
		ResultValue = OriginalEndValue;
		return true;
	}

	const FString MaterialParamsKey = Widget->MakeMaterialParamsKey(Args.TargetName, Args.ParameterName);
	FShidenImageMaterialScalarParams Params;
	bool bSuccess;
	Widget->FindImageMaterialScalarParams(MaterialParamsKey, Params, bSuccess);
	if (bSuccess)
	{
		ResultValue = OriginalEndValue + Params.EndValue;
		return true;
	}

	if (Args.Target == TEXT("Image"))
	{
		UImage* Image;
		Widget->FindImage(Args.TargetName, Image, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *Args.TargetName);
			return false;
		}

		const TObjectPtr<UObject> Resource = Image->GetBrush().GetResourceObject();
		const TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource);
		if (!DynamicMaterial)
		{
			ErrorMessage = FString::Printf(TEXT("Material of image \"%s\" is not found."), *Args.TargetName);
			return false;
		}

		float CurrentEndValue;
		DynamicMaterial->GetScalarParameterValue(FName(Args.ParameterName), CurrentEndValue);
		ResultValue = OriginalEndValue + CurrentEndValue;
		return true;
	}

	if (Args.Target == TEXT("RetainerBox"))
	{
		URetainerBox* RetainerBox;
		Widget->FindRetainerBox(Args.TargetName, RetainerBox, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find retainer box %s."), *Args.TargetName);
			return false;
		}

		const TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = RetainerBox->GetEffectMaterial();
		if (!DynamicMaterial)
		{
			ErrorMessage = FString::Printf(TEXT("Material of retainer box \"%s\" is not found."), *Args.TargetName);
			return false;
		}

		float CurrentEndValue;
		DynamicMaterial->GetScalarParameterValue(FName(Args.ParameterName), CurrentEndValue);
		ResultValue = OriginalEndValue + CurrentEndValue;
		return true;
	}

	return false;
}

bool UShidenChangeMaterialScalarParameterCommand::TryStartChangeParameter(const FChangeMaterialScalarParameterCommandArgs& Args,
                                                                          UShidenWidget* Widget, const FString& ProcessName, FString& ErrorMessage)
{
	if (Args.Target == TEXT("Image"))
	{
		UImage* Image;
		bool bSuccess;
		Widget->FindImage(Args.TargetName, Image, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *Args.TargetName);
			return false;
		}

		Widget->StartImageMaterialScalarChange(Args.TargetName, Image, FName(Args.ParameterName),
		                                       Args.EasingFunction, Args.Duration, Args.EndValue, Args.BlendExp, Args.Steps,
		                                       ProcessName, bSuccess, ErrorMessage);
		return bSuccess;
	}

	if (Args.Target == TEXT("RetainerBox"))
	{
		URetainerBox* RetainerBox;
		bool bSuccess;
		Widget->FindRetainerBox(Args.TargetName, RetainerBox, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find retainer box %s."), *Args.TargetName);
			return false;
		}

		Widget->StartRetainerBoxMaterialScalarChange(Args.TargetName, RetainerBox, FName(Args.ParameterName),
		                                             Args.EasingFunction, Args.Duration, Args.EndValue, Args.BlendExp, Args.Steps,
		                                             ProcessName, bSuccess, ErrorMessage);
		return bSuccess;
	}

	ErrorMessage = FString::Printf(TEXT("Invalid target type %s."), *Args.Target);
	return false;
}

FString UShidenChangeMaterialScalarParameterCommand::MakeScenarioPropertyKey(const FString& TargetType, const FString& TargetName, const FString& ParameterName)
{
	return FString::Printf(TEXT("%s::%s::%s"),
	                       *TargetType.Replace(TEXT(":"), TEXT("\\:")),
	                       *TargetName.Replace(TEXT(":"), TEXT("\\:")),
	                       *ParameterName.Replace(TEXT(":"), TEXT("\\:")));
}

TTuple<FString, FString, FString> UShidenChangeMaterialScalarParameterCommand::ParseScenarioPropertyKey(const FString& Key)
{
	TArray<FString> TempArray;
	Key.ParseIntoArray(TempArray, TEXT("::"), true);
	if (TempArray.Num() != 3)
	{
		return TTuple<FString, FString, FString>(TEXT(""), TEXT(""), TEXT(""));
	}

	const FString TargetType = TempArray[0].Replace(TEXT("\\:"), TEXT(":"));
	const FString TargetName = TempArray[1].Replace(TEXT("\\:"), TEXT(":"));
	const FString ParameterName = TempArray[2].Replace(TEXT("\\:"), TEXT(":"));
	return TTuple<FString, FString, FString>(TargetType, TargetName, ParameterName);
}


bool UShidenChangeMaterialScalarParameterCommand::TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage)
{
	static const TMap<FString, EEasingFunc::Type> CurveMap = {
		{TEXT("Linear"), EEasingFunc::Linear},
		{TEXT("Step"), EEasingFunc::Step},
		{TEXT("Sinusoidal in"), EEasingFunc::SinusoidalIn},
		{TEXT("Sinusoidal out"), EEasingFunc::SinusoidalOut},
		{TEXT("Sinusoidal in out"), EEasingFunc::SinusoidalInOut},
		{TEXT("Ease in"), EEasingFunc::EaseIn},
		{TEXT("Ease out"), EEasingFunc::EaseOut},
		{TEXT("Ease in out"), EEasingFunc::EaseInOut},
		{TEXT("Expo in"), EEasingFunc::ExpoIn},
		{TEXT("Expo out"), EEasingFunc::ExpoOut},
		{TEXT("Expo in out"), EEasingFunc::ExpoInOut},
		{TEXT("Circular in"), EEasingFunc::CircularIn},
		{TEXT("Circular out"), EEasingFunc::CircularOut},
		{TEXT("Circular in out"), EEasingFunc::CircularInOut}
	};

	if (const EEasingFunc::Type* FoundCurve = CurveMap.Find(EasingFuncStr))
	{
		EasingFunc = *FoundCurve;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EEasingFunc::Type."), *EasingFuncStr);
	return false;
}
