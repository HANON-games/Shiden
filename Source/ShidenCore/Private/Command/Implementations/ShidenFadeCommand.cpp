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

void UShidenFadeCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TTuple<FString, FString>& Property : ScenarioProperties)
	{
		FString SlotName, PropertyName;
		Property.Key.Split(TEXT("::"), &SlotName, &PropertyName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		if (PropertyName != TEXT("Color"))
		{
			continue;
		}

		FColor Color;
		if (!Color.InitFromString(Property.Value))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector."), *Property.Value);
			return;
		}

		if (const bool bIsFadeOut = Color.A == 1.0f; !bIsFadeOut)
		{
			continue;
		}

		const FString ZOrderStr = ScenarioProperties.FindRef(FString::Printf(TEXT("%s::ZOrder"), *SlotName));

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

		bool bSuccess = false;
		StartFade(Args, ShidenWidget, TEXT("Default"), bSuccess, ErrorMessage);
		if (!bSuccess)
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

	bool bSuccess = false;
	StartFade(Args, ShidenWidget, ProcessName, bSuccess, ErrorMessage);
	Status = bSuccess ? EShidenPreProcessStatus::Complete : EShidenPreProcessStatus::Error;
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

	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::Color"), *Args.LayerName),
	                                                          TargetColor.ToString());
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::ZOrder"), *Args.LayerName),
	                                                          FString::FromInt(Args.ZOrder));

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

	bool bSuccess = false;
	StartFade(Args, ShidenWidget, TEXT("Default"), bSuccess, ErrorMessage);
	Status = bSuccess ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

void UShidenFadeCommand::StartFade(const FFadeCommandArgs& Args, UShidenWidget* ShidenWidget, const FString& OwnerProcessName, bool& bSuccess,
                                   FString& ErrorMessage)
{
	const bool bIsFadeOut = IsFadeOut(Args.FadeType);
	ShidenWidget->StartFade(Args.LayerName, Args.FadeFunction, Args.FadeDuration, FLinearColor(Args.TargetColor), bIsFadeOut, Args.BlendExp,
	                        Args.Steps,
	                        OwnerProcessName, Args.ZOrder, bSuccess, ErrorMessage);
}

bool UShidenFadeCommand::IsFadeOut(const FString& FadeTypeStr)
{
	return FadeTypeStr.Compare(TEXT("FadeOut"), ESearchCase::IgnoreCase) == 0;
}

bool UShidenFadeCommand::TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage)
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
