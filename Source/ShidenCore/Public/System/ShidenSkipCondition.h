// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenSkipCondition.generated.h"

UENUM(BlueprintType)
enum class EShidenSkipCondition : uint8
{
	ReadOnly,
	All,
	None
};
