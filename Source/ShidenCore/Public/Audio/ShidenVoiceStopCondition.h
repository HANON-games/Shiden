// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenVoiceStopCondition.generated.h"

UENUM(BlueprintType)
enum class EShidenVoiceStopCondition : uint8
{
	Never,
	NextTextOrVoice,
	NextVoice
};
