// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenCommandRedirector.h"
#include "ShidenPluginVersion.h"

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

	return BuiltIn;
}
