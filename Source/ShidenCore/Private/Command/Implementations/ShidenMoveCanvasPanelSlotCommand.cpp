// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenMoveCanvasPanelSlotCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Components/CanvasPanelSlot.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenMoveCanvasPanelSlotCommand::TryParseCommand(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                        FMoveCanvasPanelSlotCommandArgs& Args, FString& ErrorMessage)
{
	Args.SlotName = Command.GetArg(TEXT("SlotName"));
	const FString EasingFunctionStr = Command.GetArg(TEXT("EasingFunction"));
	Args.Duration = Command.GetArgAsFloat(TEXT("Duration"));
	Args.ChangeType = Command.GetArg(TEXT("ChangeType"));
	Args.Steps = Command.GetArgAsInt(TEXT("Steps"));
	Args.BlendExp = Command.GetArgAsFloat(TEXT("BlendExp"));
	Args.OverwritePosition = Command.GetOptionalArgAsVector2D(TEXT("OverwritePosition"));
	Args.OverwriteSize = Command.GetOptionalArgAsVector2D(TEXT("OverwriteSize"));
	Args.bWaitForCompletion = Command.GetArgAsBool(TEXT("WaitForCompletion"));
	Args.OverwriteZOrder = Command.GetOptionalArgAsInt(TEXT("OverwriteZOrder"));

	if (!TryResolveEndValue(Args, ShidenWidget, Args.EndPosition, Args.EndSize, ErrorMessage))
	{
		return false;
	}

	return ShidenCommandHelpers::TryConvertToEasingFunc(EasingFunctionStr, Args.EasingFunction, ErrorMessage);
}

void UShidenMoveCanvasPanelSlotCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
                                                                           UShidenWidget* ShidenWidget,
                                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                           UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                           FString& ErrorMessage)
{
	for (const TPair<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
		FString SlotName = Property.Key;

		TMap<FString, FString> PropertyMap;
		if (!Property.Value.TryConvertToStringMap(PropertyMap))
		{
			continue;
		}

		UCanvasPanelSlot* Slot;
		if (!ShidenWidget->TryFindCanvasPanelSlot(SlotName, Slot))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *SlotName);
			return;
		}

		if (const FString* PositionStr = PropertyMap.Find(TEXT("Position")))
		{
			FVector2D Position;
			if (!Position.InitFromString(*PositionStr))
			{
				Status = EShidenInitFromSaveDataStatus::Error;
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), **PositionStr);
				return;
			}
			Slot->SetPosition(Position);
		}

		if (const FString* SizeStr = PropertyMap.Find(TEXT("Size")))
		{
			FVector2D Size;
			if (!Size.InitFromString(*SizeStr))
			{
				Status = EShidenInitFromSaveDataStatus::Error;
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), **SizeStr);
				return;
			}
			Slot->SetSize(Size);
		}

		if (const FString* ZOrderStr = PropertyMap.Find(TEXT("ZOrder")))
		{
			const int32 ZOrder = FCString::Atoi(**ZOrderStr);
			Slot->SetZOrder(ZOrder);
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
	if (!TryParseCommand(Command, ShidenWidget, Args, ErrorMessage))
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

void UShidenMoveCanvasPanelSlotCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                      const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                      const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                      FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion && !ShidenWidget->IsCanvasPanelSlotMoveCompleted(Args.SlotName))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	FShidenScenarioProperty ScenarioProperty;
	UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(Command.CommandName, Args.SlotName, ScenarioProperty);
	TMap<FString, FString> ScenarioProperties;
	ScenarioProperty.TryConvertToStringMap(ScenarioProperties);

	if (Args.OverwritePosition.IsSet())
	{
		ScenarioProperties.Add(TEXT("Position"), Args.EndPosition.ToString());
	}

	if (Args.OverwriteSize.IsSet())
	{
		ScenarioProperties.Add(TEXT("Size"), Args.EndSize.ToString());
	}

	UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(Command.CommandName, Args.SlotName, ScenarioProperties);

	Status = EShidenProcessStatus::Next;
}

void UShidenMoveCanvasPanelSlotCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
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

	if (!TryOverwriteZOrder(Args, ShidenWidget, false, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryStartMoveSlot(Args, ShidenWidget, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenMoveCanvasPanelSlotCommand::TryResolveEndValue(const FMoveCanvasPanelSlotCommandArgs& Args, UShidenWidget* ShidenWidget,
                                                           FVector2D& ResultPosition, FVector2D& ResultSize, FString& ErrorMessage)
{
	if (Args.ChangeType == TEXT("AbsoluteValue"))
	{
		ResultPosition = Args.OverwritePosition.Get(FVector2D::ZeroVector);
		ResultSize = Args.OverwriteSize.Get(FVector2D::ZeroVector);
		return true;
	}

	UCanvasPanelSlot* Slot;
	if (!ShidenWidget->TryFindCanvasPanelSlot(Args.SlotName, Slot))
	{
		ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *Args.SlotName);
		return false;
	}

	FShidenCanvasPanelSlotMoveParams Params;
	const bool bSuccess = ShidenWidget->TryFindCanvasPanelMoveParams(Args.SlotName, Params);

	ResultPosition = bSuccess && Params.bChangePosition
		                 ? Params.EndPosition + Args.OverwritePosition.Get(FVector2D::ZeroVector)
		                 : Slot->GetPosition() + Args.OverwritePosition.Get(FVector2D::ZeroVector);

	ResultSize = bSuccess && Params.bChangeSize
		             ? Params.EndSize + Args.OverwriteSize.Get(FVector2D::ZeroVector)
		             : Slot->GetSize() + Args.OverwriteSize.Get(FVector2D::ZeroVector);

	return true;
}

bool UShidenMoveCanvasPanelSlotCommand::TryStartMoveSlot(const FMoveCanvasPanelSlotCommandArgs& Args, UShidenWidget* ShidenWidget,
                                                         const FString& ProcessName, FString& ErrorMessage)
{
	UCanvasPanelSlot* Slot;
	if (!ShidenWidget->TryFindCanvasPanelSlot(Args.SlotName, Slot))
	{
		ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *Args.SlotName);
		return false;
	}

	return ShidenWidget->TryStartCanvasPanelSlotMove(Args.SlotName, Slot, Args.EasingFunction, Args.Duration,
	                                                 Args.OverwritePosition.IsSet(), Args.EndPosition, Args.OverwriteSize.IsSet(), Args.EndSize,
	                                                 Args.BlendExp, Args.Steps, ProcessName, ErrorMessage);
}

bool UShidenMoveCanvasPanelSlotCommand::TryOverwriteZOrder(const FMoveCanvasPanelSlotCommandArgs& Args, const UShidenWidget* ShidenWidget,
                                                           const bool bRegisterCurrentProperty, FString& ErrorMessage)
{
	if (!Args.OverwriteZOrder.IsSet())
	{
		return true;
	}

	UCanvasPanelSlot* Slot;
	if (!ShidenWidget->TryFindCanvasPanelSlot(Args.SlotName, Slot))
	{
		ErrorMessage = FString::Printf(TEXT("CanvasPanelSlot %s not found."), *Args.SlotName);
		return false;
	}

	Slot->SetZOrder(Args.OverwriteZOrder.GetValue());

	if (bRegisterCurrentProperty)
	{
		FShidenScenarioProperty ScenarioProperty;
		UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(TEXT("MoveCanvasPanelSlot"), Args.SlotName, ScenarioProperty);
		TMap<FString, FString> ScenarioProperties;
		ScenarioProperty.TryConvertToStringMap(ScenarioProperties);

		ScenarioProperties.Add(TEXT("ZOrder"), FString::FromInt(Args.OverwriteZOrder.GetValue()));

		UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(TEXT("MoveCanvasPanelSlot"), Args.SlotName, ScenarioProperties);
	}

	return true;
}
