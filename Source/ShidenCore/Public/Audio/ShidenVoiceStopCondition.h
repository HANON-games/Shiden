// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenVoiceStopCondition.generated.h"

UENUM(BlueprintType)
enum class EShidenVoiceStopCondition : uint8
{
	Never,
	NextTextOrVoice,
	NextVoice
};
