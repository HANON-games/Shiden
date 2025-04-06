// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenImageCommand.h"
#include "Components/CanvasPanelSlot.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "System/ShidenBlueprintLibrary.h"

bool UShidenImageCommand::TryParseCommand(const FShidenCommand& Command, FImageCommandArgs& Args, FString& ErrorMessage)
{
	Args.SlotName = Command.GetArg(TEXT("SlotName"));
	Args.ImagePath = Command.GetArg(TEXT("Image"));
	Args.OverwritePosition = Command.GetArg(TEXT("OverwritePosition"));
	Args.OverwriteSize = Command.GetArg(TEXT("OverwriteSize"));
	Args.OverwriteSizeToContent = Command.GetArg(TEXT("OverwriteSizeToContent"));
	const FString FadeFunctionStr = Command.GetArg(TEXT("FadeFunction"));
	Args.FadeDuration = Command.GetArgAsFloat(TEXT("FadeDuration"));
	Args.Steps = Command.GetArgAsInt(TEXT("Steps"));
	Args.BlendExp = Command.GetArgAsFloat(TEXT("BlendExp"));
	Args.bWaitForCompletion = Command.GetArgAsBool(TEXT("WaitForCompletion"));
	
	return TryConvertToEasingFunc(FadeFunctionStr, Args.FadeFunction, ErrorMessage);
}

void UShidenImageCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties,
                                                             UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                             UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TPair<FString, FString>& Property : ScenarioProperties)
	{
		FString SlotName;
		FString PropertyName;
		bool bSuccess = Property.Key.Split(TEXT("::"), &SlotName, &PropertyName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to split %s."), *Property.Key);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		UImage* Image;
		Widget->FindImage(SlotName, Image, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *SlotName);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (PropertyName == TEXT("Path"))
		{
			bool bImagePathIsEmpty = Property.Value.IsEmpty() || Property.Value == TEXT("None");
			if (!bImagePathIsEmpty)
			{
				UObject* Asset;
				UShidenBlueprintLibrary::GetOrLoadAsset(Property.Value, Asset, bSuccess);
				if (!bSuccess)
				{
					ErrorMessage = FString::Printf(TEXT("Failed to load asset %s."), *Property.Value);
					Status = EShidenInitFromSaveDataStatus::Error;
					return;
				}

				USlateBrushAsset* SlateBrushAsset = Cast<USlateBrushAsset>(Asset);
				if (!SlateBrushAsset)
				{
					ErrorMessage = FString::Printf(TEXT("Asset %s is not USlateBrushAsset."), *Property.Value);
					Status = EShidenInitFromSaveDataStatus::Error;
					return;
				}
				Image->SetBrushFromAsset(SlateBrushAsset);
			}
			continue;
		}

		if (PropertyName == TEXT("Color"))
		{
			FLinearColor Color;
			bSuccess = Color.InitFromString(Property.Value);
			if (!bSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FLinearColor."), *Property.Value);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			Image->SetColorAndOpacity(Color);
			continue;
		}

		if (PropertyName == TEXT("Position"))
		{
			FVector2D Position;
			bSuccess = Position.InitFromString(Property.Value);
			if (!bSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Property.Value);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Image->Slot);
			Slot->SetPosition(Position);
			continue;
		}

		if (PropertyName == TEXT("Size"))
		{
			FVector2D Size;
			bSuccess = Size.InitFromString(Property.Value);
			if (!bSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Property.Value);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Image->Slot);
			Slot->SetSize(Size);
			continue;
		}

		if (PropertyName == TEXT("SizeToContent"))
		{
			const bool bOverwriteSizeToContent = Property.Value.Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
			UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Image->Slot);
			Slot->SetAutoSize(bOverwriteSizeToContent);
			continue;
		}

		Status = EShidenInitFromSaveDataStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Unknown property %s."), *PropertyName);
		return;
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenImageCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                           UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryShowImage(Args, Widget, true, ProcessName, ErrorMessage)
		         ? EShidenPreProcessStatus::Complete
		         : EShidenPreProcessStatus::Error;
}

void UShidenImageCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                        UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                        const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                        FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion && !Widget->IsImageFadeCompleted(Args.SlotName))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::Path"), *Args.SlotName), Args.ImagePath);

	const bool bImagePathIsEmpty = Args.ImagePath.IsEmpty() || Args.ImagePath == TEXT("None");
	const FColor ResultColor(1.f, 1.f, 1.f, bImagePathIsEmpty ? 0.f : 1.f);
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::Color"), *Args.SlotName), ResultColor.ToString());

	if (!Args.OverwritePosition.IsEmpty())
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::Position"), *Args.SlotName), Args.OverwritePosition);
	}
	if (!Args.OverwriteSize.IsEmpty())
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::Size"), *Args.SlotName), Args.OverwriteSize);
	}
	if (!Args.OverwriteSizeToContent.IsEmpty())
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, FString::Printf(TEXT("%s::SizeToContent"), *Args.SlotName), Args.OverwriteSizeToContent);
	}

	Status = EShidenProcessStatus::Next;
}

void UShidenImageCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                        EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryShowImage(Args, Widget, bIsCurrentCommand, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenImageCommand::TryShowImage(const FImageCommandArgs& Args, UShidenWidget* Widget, const bool& Animate, const FString& OwnerProcessName,
                                       FString& ErrorMessage)
{
	UImage* Image;
	bool bSuccess;
	Widget->FindImage(Args.SlotName, Image, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *Args.SlotName);
		return false;
	}

	const bool bImagePathIsEmpty = Args.ImagePath.IsEmpty() || Args.ImagePath == TEXT("None");
	if (!bImagePathIsEmpty)
	{
		UObject* Asset;
		UShidenBlueprintLibrary::GetOrLoadAsset(Args.ImagePath, Asset, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to load asset %s."), *Args.ImagePath);
			return false;
		}

		USlateBrushAsset* SlateBrushAsset = Cast<USlateBrushAsset>(Asset);
		if (!SlateBrushAsset)
		{
			ErrorMessage = FString::Printf(TEXT("Asset %s is not USlateBrushAsset."), *Args.ImagePath);
			return false;
		}

		UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Image->Slot);
		if (!Args.OverwritePosition.IsEmpty())
		{
			FVector2D Position;
			bSuccess = Position.InitFromString(Args.OverwritePosition);
			if (!bSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Args.OverwritePosition);
				return false;
			}
			Slot->SetPosition(Position);
		}

		if (!Args.OverwriteSize.IsEmpty())
		{
			FVector2D Size;
			bSuccess = Size.InitFromString(Args.OverwriteSize);
			if (!bSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Args.OverwriteSize);
				return false;
			}
			Slot->SetSize(Size);
		}

		if (!Args.OverwriteSizeToContent.IsEmpty())
		{
			Slot->SetAutoSize(Args.OverwriteSizeToContent.Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0);
		}

		Image->SetBrushFromAsset(SlateBrushAsset);
	}

	Widget->StartImageFade(Args.SlotName, Image, Args.FadeFunction, Animate ? Args.FadeDuration : 0.0f,
	                       true, bImagePathIsEmpty, Args.BlendExp, Args.Steps, OwnerProcessName, bImagePathIsEmpty,
	                       bSuccess, ErrorMessage);
	return true;
}

bool UShidenImageCommand::TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage)
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
