// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenFlexCalculationOperatorType.generated.h"

UENUM(BlueprintType, Category = "SVNInternal")
enum class EShidenFlexCalculationOperatorType : uint8
{
	Numeral,
	NonNumeral,
	Vector
};
