// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenCallWidgetFunctionCommand.h"
#include "Misc/OutputDeviceNull.h"

void UShidenCallWidgetFunctionCommand::ParseFromCommand(const FShidenCommand& Command, FCallWidgetFunctionArgs& Args)
{
	static constexpr int32 MaxArguments = 10;
	
	Args.FunctionName = Command.GetArg(TEXT("FunctionName"));
	Args.Arguments.Empty(MaxArguments);

	for (int32 i = 1; i <= MaxArguments; ++i)
	{
		const FString Value = Command.GetArg(FString::Printf(TEXT("Arg %d"), i));
		if (!Value.IsEmpty())
		{
			Args.Arguments.Add(Value);
		}
	}
}

void UShidenCallWidgetFunctionCommand::ProcessCommand_Implementation(const FString& ProcessName,
                                                                     const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                     const float DeltaTime, UObject* CallerObject,
                                                                     EShidenProcessStatus& Status, FString& BreakReason,
                                                                     FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	if (Args.FunctionName.IsEmpty())
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("Failed to call widget function. Function name is empty.");
		return;
	}
	
	const FString Parameters = FString::Join(Args.Arguments, TEXT(" ")).TrimStartAndEnd();
	const FString Cmd = Parameters.IsEmpty()
		                    ? Args.FunctionName
		                    : FString::Printf(TEXT("%s %s"), *Args.FunctionName, *Parameters);

	FOutputDeviceNull Null;
	if (!ShidenWidget->CallFunctionByNameWithArguments(*Cmd, Null, nullptr, true))
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Failed to call widget function '%s'. Function may not exist or has incorrect parameters."), *Args.FunctionName);
		return;
	}
	
	Status = EShidenProcessStatus::Next;
}
