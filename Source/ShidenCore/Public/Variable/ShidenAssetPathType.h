// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenAssetPathType.generated.h"

UENUM(BlueprintType)
enum class EShidenAssetPathType : uint8
{
	None,
	Any,
	SlateBrush UMETA(DisplayName = "SlateBrush"),
	SoundBase UMETA(DisplayName = "SoundBase"),
	MediaSource UMETA(DisplayName = "MediaSource"),
	Texture UMETA(DisplayName = "Texture"),
	ForceFeedbackEffect UMETA(DisplayName = "ForceFeedbackEffect"),
};
