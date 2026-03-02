// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenAutoSaveCommand.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Engine/Texture2D.h"
#include "Brushes/SlateNoResource.h"
#include "Engine/Engine.h"

void UShidenAutoSaveCommand::ParseFromCommand(const FShidenCommand& Command, FAutoSaveCommandArgs& Args)
{
	Args.SlotName = Command.GetArg(TEXT("SlotName"));
	Args.OverwriteThumbnail = Command.GetOptionalArg(TEXT("OverwriteThumbnail"));
}

void UShidenAutoSaveCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                           FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (GEngine->GetEngineSubsystem<UShidenSubsystem>()->PredefinedSystemVariable.bIsGalleryMode)
	{
		Status = EShidenProcessStatus::Next;
		return;
	}

	if (!IsWidgetInitialized(ShidenWidget))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	ParseFromCommand(Command, Args);

	Status = TryExecuteAutoSave(Args, ProcessName, ShidenWidget, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

bool UShidenAutoSaveCommand::IsWidgetInitialized(const UShidenWidget* ShidenWidget)
{
	const FVector2D DesiredSize = ShidenWidget->GetDesiredSize();
	return DesiredSize.X > 0 && DesiredSize.Y > 0;
}

bool UShidenAutoSaveCommand::TryExecuteAutoSave(const FAutoSaveCommandArgs& Args, const FString& ProcessName, UShidenWidget* ShidenWidget,
                                                FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		ErrorMessage = FString::Printf(TEXT("Failed to peek scenario %s."), *ProcessName);
		return false;
	}

	const int32 CurrentIndex = ProgressStack->GetCurrentScenarioIndex();

	// Temporarily advance the scenario index so that the game does not resume from the AutoSave command
	ProgressStack->UpdateCurrentScenarioIndex(CurrentIndex + 1);

	if (!Args.OverwriteThumbnail.IsSet())
	{
		ShidenWidget->SaveGameWithScreenCapture(Args.SlotName);
	}
	else if (Args.OverwriteThumbnail.GetValue() == TEXT("None"))
	{
		ShidenWidget->SaveGame(Args.SlotName, Cast<UTexture2D>(FSlateNoResource().GetResourceObject()));
	}
	else
	{
		UObject* Thumbnail = nullptr;
		if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(Args.OverwriteThumbnail.GetValue(), Thumbnail))
		{
			ProgressStack->UpdateCurrentScenarioIndex(CurrentIndex);
			ErrorMessage = FString::Printf(TEXT("Failed to load thumbnail asset %s."), *Args.OverwriteThumbnail.GetValue());
			return false;
		}

		const TObjectPtr<const UTexture2D> ThumbnailTexture = Cast<UTexture2D>(Thumbnail);
		if (!ThumbnailTexture)
		{
			ProgressStack->UpdateCurrentScenarioIndex(CurrentIndex);
			ErrorMessage = FString::Printf(TEXT("Thumbnail asset %s is not a valid Texture2D."), *Args.OverwriteThumbnail.GetValue());
			return false;
		}

		ShidenWidget->SaveGame(Args.SlotName, ThumbnailTexture);
	}

	ProgressStack->UpdateCurrentScenarioIndex(CurrentIndex);
	return true;
}
