// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableKind.generated.h"

UENUM(BlueprintType)
enum class EShidenVariableKind : uint8
{
	UserVariable UMETA(DisplayName = "UserVariable"),
	LocalVariable UMETA(DisplayName = "LocalVariable"),
	SystemVariable UMETA(DisplayName = "SystemVariable"),
	PredefinedSystemVariable UMETA(DisplayName = "PredefinedSystemVariable"),
};
