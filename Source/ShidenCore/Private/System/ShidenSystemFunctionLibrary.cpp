// Copyright (c) 2024 HANON. All Rights Reserved.

#include "System/ShidenSystemFunctionLibrary.h"
#include "System/ShidenSubsystem.h"
#include "Engine/Engine.h"

SHIDENCORE_API bool UShidenSystemFunctionLibrary::IsAutoTextMode()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->bAutoTextMode;
}

SHIDENCORE_API void UShidenSystemFunctionLibrary::SetAutoTextMode(const bool bMode)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->bAutoTextMode = bMode;
}
