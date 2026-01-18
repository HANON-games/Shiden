// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenImageCommand.h"
#include "Command/ShidenCommandHelpers.h"
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

	return ShidenCommandHelpers::TryConvertToEasingFunc(FadeFunctionStr, Args.FadeFunction, ErrorMessage);
}

void UShidenImageCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
                                                             UShidenWidget* ShidenWidget,
                                                             const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                             UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TPair<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
		FString SlotName = Property.Key;

		TMap<FString, FString> PropertyMap;
		if (!Property.Value.TryConvertToStringMap(PropertyMap))
		{
			continue;
		}

		UImage* Image;
		if (!ShidenWidget->TryFindImage(SlotName, Image))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *SlotName);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		const FString* PathStr = PropertyMap.Find(TEXT("Path"));
		if (!PathStr)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find Path for image %s."), *SlotName);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
		const FString& ImagePath = *PathStr;
		const bool bImagePathIsEmpty = ImagePath.IsEmpty() || ImagePath == TEXT("None");
		if (!bImagePathIsEmpty)
		{
			UObject* Asset;
			if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(ImagePath, Asset))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to load asset %s."), *ImagePath);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}

			TObjectPtr<USlateBrushAsset> SlateBrushAsset = Cast<USlateBrushAsset>(Asset);
			if (!SlateBrushAsset)
			{
				ErrorMessage = FString::Printf(TEXT("Asset %s is not USlateBrushAsset."), *ImagePath);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			Image->SetBrushFromAsset(SlateBrushAsset);
		}

		if (const FString* ColorStr = PropertyMap.Find(TEXT("Color")))
		{
			FLinearColor Color;
			if (!Color.InitFromString(*ColorStr))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FLinearColor."), **ColorStr);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			Image->SetColorAndOpacity(Color);
		}

		const TObjectPtr<UCanvasPanelSlot> Slot = Cast<UCanvasPanelSlot>(Image->Slot);
		if (!Slot)
		{
			ErrorMessage = FString::Printf(TEXT("Image %s is not in a canvas panel slot."), *SlotName);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (const FString* PositionStr = PropertyMap.Find(TEXT("Position")))
		{
			FVector2D Position;
			if (!Position.InitFromString(*PositionStr))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), **PositionStr);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			Slot->SetPosition(Position);
		}

		if (const FString* SizeStr = PropertyMap.Find(TEXT("Size")))
		{
			FVector2D Size;
			if (!Size.InitFromString(*SizeStr))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), **SizeStr);
				Status = EShidenInitFromSaveDataStatus::Error;
				return;
			}
			Slot->SetSize(Size);
		}

		if (const FString* SizeToContentStr = PropertyMap.Find(TEXT("SizeToContent")))
		{
			const bool bOverwriteSizeToContent = SizeToContentStr->Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
			Slot->SetAutoSize(bOverwriteSizeToContent);
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenImageCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                           UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryShowImage(Args, ShidenWidget, true, ProcessName, ErrorMessage)
		         ? EShidenPreProcessStatus::Complete
		         : EShidenPreProcessStatus::Error;
}

void UShidenImageCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                        UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                        const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                        FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion && !ShidenWidget->IsImageFadeCompleted(Args.SlotName))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	FShidenScenarioProperty ScenarioProperty;
	UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(Command.CommandName, Args.SlotName, ScenarioProperty);
	TMap<FString, FString> ScenarioProperties;
	ScenarioProperty.TryConvertToStringMap(ScenarioProperties);

	ScenarioProperties.Add(TEXT("Path"), Args.ImagePath);

	const bool bImagePathIsEmpty = Args.ImagePath.IsEmpty() || Args.ImagePath == TEXT("None");
	const FLinearColor ResultColor(1.f, 1.f, 1.f, bImagePathIsEmpty ? 0.f : 1.f);
	ScenarioProperties.Add(TEXT("Color"), ResultColor.ToString());

	if (!Args.OverwritePosition.IsEmpty())
	{
		ScenarioProperties.Add(TEXT("Position"), Args.OverwritePosition);
	}
	if (!Args.OverwriteSize.IsEmpty())
	{
		ScenarioProperties.Add(TEXT("Size"), Args.OverwriteSize);
	}
	if (!Args.OverwriteSizeToContent.IsEmpty())
	{
		ScenarioProperties.Add(TEXT("SizeToContent"), Args.OverwriteSizeToContent);
	}

	UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(Command.CommandName, Args.SlotName, ScenarioProperties);

	Status = EShidenProcessStatus::Next;
}

void UShidenImageCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                        EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryShowImage(Args, ShidenWidget, bIsCurrentCommand, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenImageCommand::TryShowImage(const FImageCommandArgs& Args, UShidenWidget* ShidenWidget, const bool Animate,
                                       const FString& OwnerProcessName,
                                       FString& ErrorMessage)
{
	UImage* Image;
	if (!ShidenWidget->TryFindImage(Args.SlotName, Image))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find image %s."), *Args.SlotName);
		return false;
	}

	const bool bImagePathIsEmpty = Args.ImagePath.IsEmpty() || Args.ImagePath == TEXT("None");
	if (!bImagePathIsEmpty)
	{
		UObject* Asset;
		if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(Args.ImagePath, Asset))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to load asset %s."), *Args.ImagePath);
			return false;
		}

		const TObjectPtr<USlateBrushAsset> SlateBrushAsset = Cast<USlateBrushAsset>(Asset);
		if (!SlateBrushAsset)
		{
			ErrorMessage = FString::Printf(TEXT("Asset %s is not USlateBrushAsset."), *Args.ImagePath);
			return false;
		}

		const TObjectPtr<UCanvasPanelSlot> Slot = Cast<UCanvasPanelSlot>(Image->Slot);
		if (!Slot)
		{
			ErrorMessage = FString::Printf(TEXT("Image %s is not in a canvas panel slot."), *Args.SlotName);
			return false;
		}

		if (!Args.OverwritePosition.IsEmpty())
		{
			FVector2D Position;
			if (!Position.InitFromString(Args.OverwritePosition))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Args.OverwritePosition);
				return false;
			}
			Slot->SetPosition(Position);
		}

		if (!Args.OverwriteSize.IsEmpty())
		{
			FVector2D Size;
			if (!Size.InitFromString(Args.OverwriteSize))
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

	return ShidenWidget->TryStartImageFade(Args.SlotName, Image, Args.FadeFunction, Animate ? Args.FadeDuration : 0.0f,
	                                       true, bImagePathIsEmpty, Args.BlendExp, Args.Steps, OwnerProcessName, bImagePathIsEmpty,
	                                       ErrorMessage);
}
