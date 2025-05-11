// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenMoveCanvasPanelSlotCommand.h"
#include "Components/CanvasPanelSlot.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenMoveCanvasPanelSlotCommand::TryParseCommand(const FShidenCommand& Command, FMoveCanvasPanelSlotCommandArgs& Args,
                                                        FString& ErrorMessage)
{
	Args.SlotName = Command.GetArg(TEXT("SlotName"));
	const FString EasingFunctionStr = Command.GetArg(TEXT("EasingFunction"));
	Args.Duration = Command.GetArgAsFloat(TEXT("Duration"));
	Args.ChangeType = Command.GetArg(TEXT("ChangeType"));
	Args.OverwritePosition = Command.GetArgAsVector2D(TEXT("OverwritePosition"));
	Args.OverwriteSize = Command.GetArgAsVector2D(TEXT("OverwriteSize"));
	Args.Steps = Command.GetArgAsInt(TEXT("Steps"));
	Args.BlendExp = Command.GetArgAsFloat(TEXT("BlendExp"));
	const FString OverwriteZOrderStr = Command.GetArg(TEXT("OverwriteZOrder"));

	Args.bChangePosition = !Command.GetArg(TEXT("OverwritePosition")).IsEmpty();
	Args.bChangeSize = !Command.GetArg(TEXT("OverwriteSize")).IsEmpty();
	Args.bWaitForCompletion = Command.GetArg(TEXT("WaitForComplete")).ToBool();

	Args.bOverwriteZOrder = !OverwriteZOrderStr.IsEmpty();
	Args.OverwriteZOrder = Args.bOverwriteZOrder ? FCString::Atoi(*OverwriteZOrderStr) : 0;

	return TryConvertToEasingFunc(EasingFunctionStr, Args.EasingFunction, ErrorMessage);
}

void UShidenMoveCanvasPanelSlotCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties,
                                                                           UShidenWidget* ShidenWidget,
                                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                           UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                           FString& ErrorMessage)
{
	for (const TPair<FString, FString>& Property : ScenarioProperties)
	{
		FString SlotName;
		FString PropertyName;
		Property.Key.Split(TEXT("::"), &SlotName, &PropertyName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		UCanvasPanelSlot* Slot;
		bool bSuccess;
		ShidenWidget->FindCanvasPanelSlot(SlotName, Slot, bSuccess);
		if (!bSuccess)
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *SlotName);
			return;
		}

		if (PropertyName == TEXT("Position"))
		{
			FVector2d Position;
			Position.InitFromString(Property.Value);
			Slot->SetPosition(Position);
		}
		else if (PropertyName == TEXT("Size"))
		{
			FVector2d Size;
			Size.InitFromString(Property.Value);
			Slot->SetSize(Size);
		}
		else if (PropertyName == TEXT("ZOrder"))
		{
			const int32 ZOrder = FCString::Atoi(*Property.Value);
			Slot->SetZOrder(ZOrder);
		}
		else
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Invalid property key %s."), *Property.Key);
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenMoveCanvasPanelSlotCommand::PreProcessCommand_Implementation(const FString& ProcessName,
                                                                         const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                         UObject* CallerObject,
                                                                         EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	if (!TryOverwriteZOrder(Args, ShidenWidget, true, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryStartMoveSlot(Args, ShidenWidget, ProcessName, ErrorMessage)
		         ? EShidenPreProcessStatus::Complete
		         : EShidenPreProcessStatus::Error;
}

void UShidenMoveCanvasPanelSlotCommand::ProcessCommand_Implementation(const FString& ProcessName,
                                                                      const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                      const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                      const float DeltaTime,
                                                                      UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                                      FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion && !ShidenWidget->IsCanvasPanelSlotMoveCompleted(Args.SlotName))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (Args.bChangePosition)
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName,
		                                                          Args.SlotName + TEXT("::Position"), Command.GetArg(TEXT("OverwritePosition")));
	}

	if (Args.bChangeSize)
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName,
		                                                          Args.SlotName + TEXT("::Size"), Command.GetArg(TEXT("OverwriteSize")));
	}

	Status = EShidenProcessStatus::Next;
}

void UShidenMoveCanvasPanelSlotCommand::PreviewCommand_Implementation(const FShidenCommand& Command,
                                                                      UShidenWidget* ShidenWidget,
                                                                      const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                      const bool bIsCurrentCommand, EShidenPreviewStatus& Status,
                                                                      FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		Args.Duration = 0;
	}

	if (!TryOverwriteZOrder(Args, ShidenWidget, false, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryStartMoveSlot(Args, ShidenWidget, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}


bool UShidenMoveCanvasPanelSlotCommand::TryStartMoveSlot(const FMoveCanvasPanelSlotCommandArgs& Args, UShidenWidget* ShidenWidget,
                                                         const FString& ProcessName, FString& ErrorMessage)
{
	bool bSuccess;
	UCanvasPanelSlot* Slot;
	ShidenWidget->FindCanvasPanelSlot(Args.SlotName, Slot, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *Args.SlotName);
		return false;
	}

	FVector2d Position = Args.OverwritePosition;
	FVector2d Size = Args.OverwriteSize;

	if (Args.ChangeType.Compare(TEXT("AddToCurrent"), ESearchCase::IgnoreCase) == 0)
	{
		FShidenCanvasPanelSlotMoveParams Params;
		ShidenWidget->FindCanvasPanelMoveParams(Args.SlotName, Params, bSuccess);

		Position += bSuccess && Params.bChangePosition
			            ? Params.EndPosition
			            : Slot->GetPosition();

		Size += bSuccess && Params.bChangeSize
			        ? Params.EndSize
			        : Slot->GetSize();
	}

	ShidenWidget->StartCanvasPanelSlotMove(Args.SlotName, Slot, Args.EasingFunction, Args.Duration,
	                                       Args.bChangePosition, Position, Args.bChangeSize, Size,
	                                       Args.BlendExp, Args.Steps, ProcessName, bSuccess, ErrorMessage);

	return bSuccess;
}

bool UShidenMoveCanvasPanelSlotCommand::TryOverwriteZOrder(const FMoveCanvasPanelSlotCommandArgs& Args, const UShidenWidget* ShidenWidget,
                                                           const bool bRegisterCurrentProperty, FString& ErrorMessage)
{
	if (!Args.bOverwriteZOrder)
	{
		return true;
	}

	bool bSuccess;
	UCanvasPanelSlot* Slot;
	ShidenWidget->FindCanvasPanelSlot(Args.SlotName, Slot, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *Args.SlotName);
		return false;
	}

	Slot->SetZOrder(Args.OverwriteZOrder);

	if (bRegisterCurrentProperty)
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(TEXT("MoveCanvasPanelSlot"),
		                                                          FString::Printf(TEXT("%s::ZOrder"), *Args.SlotName),
		                                                          FString::FromInt(Args.OverwriteZOrder));
	}

	return true;
}

bool UShidenMoveCanvasPanelSlotCommand::TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage)
{
	static const TMap<FString, EEasingFunc::Type> EasingFuncMap = {
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

	if (const EEasingFunc::Type* Found = EasingFuncMap.Find(EasingFuncStr.ToLower()))
	{
		EasingFunc = *Found;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Easing function %s is not supported."), *EasingFuncStr);
	return false;
}
