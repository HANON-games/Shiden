// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenPlayScenarioStatus.generated.h"

UENUM(BlueprintType)
enum class EShidenPlayScenarioStatus : uint8
{
	Complete = 0,
	Break,
	Error
};
