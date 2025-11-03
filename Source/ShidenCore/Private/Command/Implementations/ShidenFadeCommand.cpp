// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenFadeCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenFadeCommand::TryParseCommand(const FShidenCommand& Command, FFadeCommandArgs& Args, FString& ErrorMessage)
{
	Args.LayerName = Command.GetArg(TEXT("LayerName"));
	Args.FadeType = Command.GetArg(TEXT("FadeType"));
	Args.FadeDuration = Command.GetArgAsFloat(TEXT("FadeDuration"));
	const FString FadeFunctionStr = Command.GetArg(TEXT("FadeFunction"));
	Args.TargetColor = Command.GetArgAsVector(TEXT("TargetColor"));
	Args.Steps = Command.GetArgAsInt(TEXT("Steps"));
	Args.BlendExp = Command.GetArgAsFloat(TEXT("BlendExp"));
	Args.bWaitForCompletion = Command.GetArgAsBool(TEXT("WaitForCompletion"));
	Args.ZOrder = Command.GetArgAsInt(TEXT("ZOrder"));

	return TryConvertToEasingFunc(FadeFunctionStr, Args.FadeFunction, ErrorMessage);
}

void UShidenFadeCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TTuple<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
		const FString SlotName = Property.Key;

		TMap<FString, FString> PropertyMap;
		if (!Property.Value.TryConvertToStringMap(PropertyMap))
		{
			continue;
		}

		FLinearColor Color;
		const FString ColorStr = PropertyMap.FindRef(TEXT("Color"));
		if (!Color.InitFromString(ColorStr))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FLinearColor."), *ColorStr);
			return;
		}

		if (const bool bIsFadeOut = Color.A == 1.0f; !bIsFadeOut)
		{
			continue;
		}

		const FString ZOrderStr = PropertyMap.FindRef(TEXT("ZOrder"));

		Args = FFadeCommandArgs
		{
			.LayerName = SlotName,
			.FadeType = TEXT("FadeOut"),
			.FadeDuration = 0,
			.FadeFunction = EEasingFunc::Linear,
			.TargetColor = FVector(Color.R, Color.G, Color.B),
			.Steps = 2,
			.BlendExp = 2.0f,
			.bWaitForCompletion = true,
			.ZOrder = FCString::Atoi(*ZOrderStr)
		};

		if (!TryStartFade(Args, ShidenWidget, TEXT("Default"), ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenFadeCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                          UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                          UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryStartFade(Args, ShidenWidget, ProcessName, ErrorMessage) ? EShidenPreProcessStatus::Complete : EShidenPreProcessStatus::Error;
}

void UShidenFadeCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                       UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                       FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion && !ShidenWidget->IsFadeCompleted(Args.LayerName))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	const bool bIsFadeOut = IsFadeOut(Args.FadeType);
	const FLinearColor TargetColor(Args.TargetColor.X, Args.TargetColor.Y, Args.TargetColor.Z, bIsFadeOut ? 1 : 0);

	UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(Command.CommandName, Args.LayerName,
	                                                                {
		                                                                {TEXT("Color"), TargetColor.ToString()},
		                                                                {TEXT("ZOrder"), FString::FromInt(Args.ZOrder)},
	                                                                });

	Status = EShidenProcessStatus::Next;
}

void UShidenFadeCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                       EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	FShidenCommand TempCommand = Command;
	if (!bIsCurrentCommand)
	{
		TempCommand.Args.Add(TEXT("FadeDuration"), TEXT("0"));
	}

	if (!TryParseCommand(TempCommand, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryStartFade(Args, ShidenWidget, TEXT("Default"), ErrorMessage) ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

bool UShidenFadeCommand::TryStartFade(const FFadeCommandArgs& Args, UShidenWidget* ShidenWidget, const FString& OwnerProcessName, FString& ErrorMessage)
{
	const bool bIsFadeOut = IsFadeOut(Args.FadeType);
	return ShidenWidget->TryStartFade(Args.LayerName, Args.FadeFunction, Args.FadeDuration, FLinearColor(Args.TargetColor), bIsFadeOut, Args.BlendExp,
		Args.Steps, OwnerProcessName, Args.ZOrder, ErrorMessage);
}

bool UShidenFadeCommand::IsFadeOut(const FString& FadeTypeStr)
{
	return FadeTypeStr.Compare(TEXT("FadeOut"), ESearchCase::IgnoreCase) == 0;
}

bool UShidenFadeCommand::TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage)
{
	static const TMap<FString, EEasingFunc::Type> CurveMap = {
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

	if (const EEasingFunc::Type* FoundCurve = CurveMap.Find(EasingFuncStr.ToLower()))
	{
		EasingFunc = *FoundCurve;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EEasingFunc::Type."), *EasingFuncStr);
	return false;
}
