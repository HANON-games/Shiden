// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenFadeCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"

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
	Args.bUseGlobalFade = Command.GetArgAsBool(TEXT("UseGlobalFade"));

	if (Args.LayerName.StartsWith(TEXT("Global$")))
	{
		ErrorMessage = TEXT("LayerName cannot start with 'Global$' (reserved prefix).");
		return false;
	}

	return ShidenCommandHelpers::TryConvertToEasingFunc(FadeFunctionStr, Args.FadeFunction, ErrorMessage);
}

void UShidenFadeCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TTuple<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
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
		const FString UseGlobalFadeStr = PropertyMap.FindRef(TEXT("UseGlobalFade"));
		const bool bUseGlobalFade = UseGlobalFadeStr.Equals(TEXT("true"), ESearchCase::IgnoreCase);
		const FString LayerName = bUseGlobalFade && Property.Key.StartsWith(TEXT("Global$"))
			? Property.Key.RightChop(7)
			: Property.Key;

		Args = FFadeCommandArgs
		{
			.LayerName = LayerName,
			.FadeType = TEXT("FadeOut"),
			.FadeDuration = 0,
			.FadeFunction = EEasingFunc::Linear,
			.TargetColor = FVector(Color.R, Color.G, Color.B),
			.Steps = 2,
			.BlendExp = 2.0f,
			.bWaitForCompletion = true,
			.ZOrder = FCString::Atoi(*ZOrderStr),
			.bUseGlobalFade = bUseGlobalFade
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
	if (Args.bWaitForCompletion)
	{
		const bool bIsFadeCompleted = Args.bUseGlobalFade
			                              ? UShidenBlueprintLibrary::IsScreenFadeCompleted(Args.LayerName)
			                              : ShidenWidget->IsFadeCompleted(Args.LayerName);

		if (!bIsFadeCompleted)
		{
			Status = EShidenProcessStatus::DelayUntilNextTick;
			return;
		}
	}
	
	const bool bIsFadeOut = IsFadeOut(Args.FadeType);
	const FLinearColor TargetColor(Args.TargetColor.X, Args.TargetColor.Y, Args.TargetColor.Z, bIsFadeOut ? 1 : 0);
	const FString LayerKey = Args.bUseGlobalFade ? TEXT("Global$") + Args.LayerName : Args.LayerName;

	UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(Command.CommandName, LayerKey, {
		{TEXT("Color"), TargetColor.ToString()},
		{TEXT("ZOrder"), FString::FromInt(Args.ZOrder)},
		{TEXT("UseGlobalFade"), Args.bUseGlobalFade ? TEXT("true") : TEXT("false")}
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

bool UShidenFadeCommand::TryStartFade(const FFadeCommandArgs& Args, UShidenWidget* ShidenWidget, const FString& OwnerProcessName,
                                      FString& ErrorMessage)
{
	const bool bIsFadeOut = IsFadeOut(Args.FadeType);

	if (Args.bUseGlobalFade)
	{
		const FLinearColor TargetColorLinear(Args.TargetColor.X, Args.TargetColor.Y, Args.TargetColor.Z);

#if WITH_EDITOR
		return UShidenBlueprintLibrary::TryStartScreenFadePreview(ShidenWidget, Args.LayerName, Args.FadeDuration, Args.FadeFunction, TargetColorLinear, bIsFadeOut, Args.Steps, Args.BlendExp, Args.ZOrder);
#else
		return UShidenBlueprintLibrary::TryStartScreenFade(ShidenWidget, Args.LayerName, Args.FadeDuration, Args.FadeFunction, TargetColorLinear, bIsFadeOut, Args.Steps, Args.BlendExp, Args.ZOrder, false);
#endif
	}

	return ShidenWidget->TryStartFade(Args.LayerName, Args.FadeFunction, Args.FadeDuration, FLinearColor(Args.TargetColor), bIsFadeOut, Args.BlendExp,
	                                  Args.Steps, OwnerProcessName, Args.ZOrder, ErrorMessage);
}

bool UShidenFadeCommand::IsFadeOut(const FString& FadeTypeStr)
{
	return FadeTypeStr.Compare(TEXT("FadeOut"), ESearchCase::IgnoreCase) == 0;
}
