// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenChangeMaterialScalarParameterCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenChangeMaterialScalarParameterCommand::TryParseCommand(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                  FChangeMaterialScalarParameterCommandArgs& Args, FString& ErrorMessage)
{
	Args.Target = Command.GetArg(TEXT("Target"));
	Args.TargetName = Command.GetArg(TEXT("TargetName"));
	Args.ParameterName = Command.GetArg(TEXT("ParameterName"));
	const FString EasingFuncStr = Command.GetArg(TEXT("EasingFunction"));
	Args.Duration = Command.GetArgAsFloat(TEXT("Duration"));
	Args.ChangeType = Command.GetArg(TEXT("ChangeType"));
	const float OriginalEndValue = Command.GetArgAsFloat(TEXT("EndValue"));
	Args.Steps = Command.GetArgAsInt(TEXT("Steps"));
	Args.BlendExp = Command.GetArgAsFloat(TEXT("BlendExp"));
	Args.bWaitForCompletion = Command.GetArgAsBool(TEXT("WaitForCompletion"));

	if (!TryResolveEndValue(Args, OriginalEndValue, ShidenWidget, Args.EndValue, ErrorMessage))
	{
		return false;
	}

	return ShidenCommandHelpers::TryConvertToEasingFunc(EasingFuncStr, Args.EasingFunction, ErrorMessage);
}

void UShidenChangeMaterialScalarParameterCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
                                                                                     UShidenWidget* ShidenWidget,
                                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                     UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                                     FString& ErrorMessage)
{
	for (const TPair<FString, FShidenScenarioProperty>& Pair : ScenarioProperties)
	{
		const auto [TargetType, TargetName, ParameterName] = ShidenMaterialParameterHelpers::ParseScenarioPropertyKey(Pair.Key);

		Args = FChangeMaterialScalarParameterCommandArgs
		{
			.Target = TargetType,
			.TargetName = TargetName,
			.ParameterName = ParameterName,
			.EasingFunction = EEasingFunc::Linear,
			.Duration = 0,
			.ChangeType = TEXT("AbsoluteValue"),
			.EndValue = FCString::Atof(*Pair.Value.GetValueAsString()),
			.Steps = 0,
			.BlendExp = 0,
			.bWaitForCompletion = true
		};

		if (!TryStartChangeParameter(Args, ShidenWidget, TEXT("Default"), ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenChangeMaterialScalarParameterCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                                   UShidenWidget* ShidenWidget,
                                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                   UObject* CallerObject, EShidenPreProcessStatus& Status,
                                                                                   FString& ErrorMessage)
{
	if (!TryParseCommand(Command, ShidenWidget, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryStartChangeParameter(Args, ShidenWidget, ProcessName, ErrorMessage)
		         ? EShidenPreProcessStatus::Complete
		         : EShidenPreProcessStatus::Error;
}

void UShidenChangeMaterialScalarParameterCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                                UShidenWidget* ShidenWidget,
                                                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                const float DeltaTime, UObject* CallerObject,
                                                                                EShidenProcessStatus& Status, FString& BreakReason,
                                                                                FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion)
	{
		const bool bShouldDelay = Args.Target == TEXT("Image")
			                          ? !ShidenWidget->IsImageMaterialParameterChangeCompleted(Args.TargetName, Args.ParameterName)
			                          : !ShidenWidget->IsRetainerBoxMaterialParameterChangeCompleted(Args.TargetName, Args.ParameterName);

		if (bShouldDelay)
		{
			Status = EShidenProcessStatus::DelayUntilNextTick;
			return;
		}
	}

	const FString Key = ShidenMaterialParameterHelpers::MakeScenarioPropertyKey(Args.Target, Args.TargetName, Args.ParameterName);
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Key, FString::SanitizeFloat(Args.EndValue));
	Status = EShidenProcessStatus::Next;
}

void UShidenChangeMaterialScalarParameterCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                const bool bIsCurrentCommand, EShidenPreviewStatus& Status,
                                                                                FString& ErrorMessage)
{
	if (!TryParseCommand(Command, ShidenWidget, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		Args.Duration = 0;
	}

	Status = TryStartChangeParameter(Args, ShidenWidget, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenChangeMaterialScalarParameterCommand::TryResolveEndValue(const FChangeMaterialScalarParameterCommandArgs& Args,
                                                                     const float OriginalEndValue, UShidenWidget* ShidenWidget,
                                                                     float& ResultValue, FString& ErrorMessage)
{
	if (Args.ChangeType == TEXT("AbsoluteValue"))
	{
		ResultValue = OriginalEndValue;
		return true;
	}

	const FString MaterialParamsKey = ShidenWidget->MakeMaterialParamsKey(Args.TargetName, Args.ParameterName);

	if (Args.Target == TEXT("Image"))
	{
		FShidenImageMaterialScalarParams Params;
		if (ShidenWidget->TryFindImageMaterialScalarParams(MaterialParamsKey, Params))
		{
			ResultValue = OriginalEndValue + Params.EndValue;
			return true;
		}

		UImage* Image;
		if (!ShidenWidget->TryFindImage(Args.TargetName, Image))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *Args.TargetName);
			return false;
		}

		const TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = Image->GetDynamicMaterial();
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
		FShidenRetainerBoxMaterialScalarParams Params;
		if (ShidenWidget->TryFindRetainerBoxMaterialScalarParams(MaterialParamsKey, Params))
		{
			ResultValue = OriginalEndValue + Params.EndValue;
			return true;
		}

		URetainerBox* RetainerBox;
		if (!ShidenWidget->TryFindRetainerBox(Args.TargetName, RetainerBox))
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

	ErrorMessage = FString::Printf(TEXT("Invalid target type %s."), *Args.Target);
	return false;
}

bool UShidenChangeMaterialScalarParameterCommand::TryStartChangeParameter(const FChangeMaterialScalarParameterCommandArgs& Args,
                                                                          UShidenWidget* ShidenWidget, const FString& ProcessName,
                                                                          FString& ErrorMessage)
{
	if (Args.Target == TEXT("Image"))
	{
		UImage* Image;
		if (!ShidenWidget->TryFindImage(Args.TargetName, Image))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *Args.TargetName);
			return false;
		}

		return ShidenWidget->TryStartImageMaterialScalarChange(Args.TargetName, Image, FName(Args.ParameterName),
		                                                       Args.EasingFunction, Args.Duration, Args.EndValue, Args.BlendExp, Args.Steps,
		                                                       ProcessName, ErrorMessage);
	}

	if (Args.Target == TEXT("RetainerBox"))
	{
		URetainerBox* RetainerBox;
		if (!ShidenWidget->TryFindRetainerBox(Args.TargetName, RetainerBox))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find retainer box %s."), *Args.TargetName);
			return false;
		}

		return ShidenWidget->TryStartRetainerBoxMaterialScalarChange(Args.TargetName, RetainerBox, FName(Args.ParameterName),
		                                                             Args.EasingFunction, Args.Duration, Args.EndValue, Args.BlendExp, Args.Steps,
		                                                             ProcessName, ErrorMessage);
	}

	ErrorMessage = FString::Printf(TEXT("Invalid target type %s."), *Args.Target);
	return false;
}
