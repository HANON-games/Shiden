// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenCommandRedirector.h"

TArray<FShidenCommandRedirector> UShidenCommandRedirectors::GetBuiltIn()
{
	static TArray<FShidenCommandRedirector> BuiltIn =
		TArray<FShidenCommandRedirector>
		{
			{
				.TargetCommandName = TEXT("Sound"),
				.ArgumentNameMapping = {{TEXT("WaitForCompletion"), TEXT("WaitForFadeCompletion")}}
			}
		};
	return BuiltIn;
}
