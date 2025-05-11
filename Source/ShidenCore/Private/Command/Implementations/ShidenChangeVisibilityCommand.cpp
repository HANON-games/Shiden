// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenChangeVisibilityCommand.h"

bool UShidenChangeVisibilityCommand::TryParseCommand(const FShidenCommand& Command, FChangeVisibilityCommandArgs& Args, FString& ErrorMessage)
{
	Args.Name = Command.GetArg(TEXT("Name"));
	const FString VisibilityStr = Command.GetArg(TEXT("Visibility"));
	return TryConvertToVisibility(VisibilityStr, Args.Visibility, ErrorMessage);
}

void UShidenChangeVisibilityCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* ShidenWidget,
                                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                        UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                        FString& ErrorMessage)
{
	for (const TTuple<FString, FString>& Pair : ScenarioProperties)
	{
		Args.Name = Pair.Key;
		const FString& VisibilityStr = Pair.Value;
		if (!TryConvertToVisibility(VisibilityStr, Args.Visibility, ErrorMessage))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to ESlateVisibility."), *VisibilityStr);
			Status = EShidenInitFromSaveDataStatus::Error;
		}

		if (!TryChangeVisibility(Args, ShidenWidget, false, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenChangeVisibilityCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                   UShidenWidget* ShidenWidget,
                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                   const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                   FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryChangeVisibility(Args, ShidenWidget, true, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenChangeVisibilityCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                   bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryChangeVisibility(Args, ShidenWidget, false, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenChangeVisibilityCommand::TryChangeVisibility(const FChangeVisibilityCommandArgs& Args, UShidenWidget* ShidenWidget,
                                                         const bool bRegisterProperty, FString& ErrorMessage)
{
	bool bSuccess;
	ShidenWidget->SetVisibilityByName(Args.Name, Args.Visibility, bRegisterProperty, bSuccess);

	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change visibility. Widget %s not found."), *Args.Name);
		return false;
	}

	return true;
}

bool UShidenChangeVisibilityCommand::TryConvertToVisibility(const FString& Visibility, ESlateVisibility& Result, FString& ErrorMessage)
{
	static const TMap<FString, ESlateVisibility> VisibilityMap = {
		{TEXT("Visible"), ESlateVisibility::Visible},
		{TEXT("Hidden"), ESlateVisibility::Hidden},
		{TEXT("Collapsed"), ESlateVisibility::Collapsed},
		{TEXT("Not Hit-Testable (Self & All Children)"), ESlateVisibility::HitTestInvisible},
		{TEXT("Not Hit-Testable (Self Only)"), ESlateVisibility::SelfHitTestInvisible}
	};

	if (const ESlateVisibility* Found = VisibilityMap.Find(Visibility))
	{
		Result = *Found;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to ESlateVisibility."), *Visibility);
	return false;
}
