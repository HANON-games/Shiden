// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableType.generated.h"

UENUM(BlueprintType)
enum class EShidenVariableType : uint8
{
	Boolean,
	String,
	Integer,
	Float,
	Vector2 UMETA(DisplayName = "Vector2"),
	Vector3 UMETA(DisplayName = "Vector3")
};