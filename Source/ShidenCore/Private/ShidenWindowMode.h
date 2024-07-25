// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenWindowMode.generated.h"

UENUM(BlueprintType)
enum class EShidenWindowMode : uint8
{
	None = 0,
	Texts,
	Options,
	TextAndOptions,
};
