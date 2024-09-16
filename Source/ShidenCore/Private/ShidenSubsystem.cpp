// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenSubsystem.h"

SHIDENCORE_API bool UShidenSubsystem::IsAutoTextMode() const
{
	return bAutoTextMode;
}

SHIDENCORE_API void UShidenSubsystem::SetAutoTextMode(bool bMode)
{
	bAutoTextMode = bMode;
}
