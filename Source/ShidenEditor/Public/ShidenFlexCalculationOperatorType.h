// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenFlexCalculationOperatorType.generated.h"

UENUM(BlueprintType, Category = "SVNInternal")
enum class EShidenFlexCalculationOperatorType : uint8
{
	Numeral,
	NonNumeral,
	Vector
};
