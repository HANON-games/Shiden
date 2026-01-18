// Copyright (c) 2026 HANON. All Rights Reserved.

#include "System/ShidenCommandRedirector.h"
#include "System/ShidenPluginVersion.h"

TArray<FShidenCommandRedirector> UShidenCommandRedirectors::GetBuiltIn(const FShidenPluginVersion& CurrentVersion)
{
	TArray<FShidenCommandRedirector> BuiltIn;

	if (CurrentVersion < FShidenPluginVersion(0, 16, 15))
	{
		BuiltIn.Add({
			.TargetCommandName = TEXT("Sound"),
			.ArgumentNameMapping = {{TEXT("WaitForCompletion"), TEXT("WaitForFadeCompletion")}}
		});
	}

	if (CurrentVersion < FShidenPluginVersion(0, 16, 18))
	{
		BuiltIn.Add({
			.TargetCommandName = TEXT("AdjustBgmVolume"),
			.NewCommandName = TEXT("AdjustBGMVolume")
		});
	}

	if (CurrentVersion < FShidenPluginVersion(1, 10, 0))
	{
		BuiltIn.Add({
			.TargetCommandName = TEXT("RunMacroAsParallel"),
			.NewCommandName = TEXT("RunMacroInParallel")
		});
		BuiltIn.Add({
			.TargetCommandName = TEXT("WaitClick"),
			.NewCommandName = TEXT("WaitForClick")
		});
	}

	return BuiltIn;
}
