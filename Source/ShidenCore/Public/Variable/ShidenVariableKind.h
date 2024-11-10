// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableKind.generated.h"

UENUM(BlueprintType)
enum class EShidenVariableKind : uint8
{
	UserVariable,
	LocalVariable,
	SystemVariable,
	PredefinedSystemVariable
};