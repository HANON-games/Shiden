// Copyright (c) 2024 HANON. All Rights Reserved.

#include "System/ShidenSubsystem.h"

SHIDENCORE_API bool UShidenSubsystem::IsAutoTextMode() const
{
	return bAutoTextMode;
}

SHIDENCORE_API void UShidenSubsystem::SetAutoTextMode(const bool bMode)
{
	bAutoTextMode = bMode;
}
