// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenCallWidgetFunctionCommand.h"
#include "Misc/OutputDeviceNull.h"

void UShidenCallWidgetFunctionCommand::ParseFromCommand(const FShidenCommand& Command, FCallWidgetFunctionArgs& Args)
{
	Args.FunctionName = Command.GetArg(TEXT("FunctionName"));
	Args.Arguments.Empty(10);

	for (int32 i = 1; i <= 10; ++i)
	{
		const FString Value = Command.GetArg(FString::Printf(TEXT("Arg %d"), i));
		if (!Value.IsEmpty())
		{
			Args.Arguments.Add(Value);
		}
	}
}

void UShidenCallWidgetFunctionCommand::ProcessCommand_Implementation(const FString& ProcessName,
                                                                     const FShidenCommand& Command, UShidenWidget* Widget,
                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                     const float DeltaTime, UObject* CallerObject,
                                                                     EShidenProcessStatus& Status, FString& BreakReason,
                                                                     FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	const FString Parameters = FString::Join(Args.Arguments, TEXT(" ")).TrimStartAndEnd();
	const FString Cmd = Parameters.IsEmpty()
		                    ? Args.FunctionName
		                    : FString::Printf(TEXT("%s %s"), *Args.FunctionName, *Parameters);

	FOutputDeviceNull Null;
	Widget->CallFunctionByNameWithArguments(*Cmd, Null, nullptr, true);

	Status = EShidenProcessStatus::Next;
}
