// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenVoiceStopCondition.generated.h"

UENUM(BlueprintType)
enum class EShidenVoiceStopCondition : uint8
{
	None,
	NextTextOrVoice,
	NextVoice
};
