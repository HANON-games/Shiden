// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenScenario.h"

SHIDENCORE_API void UShidenScenario::PostDuplicate(bool bDuplicateForPIE) {
	ScenarioId = FGuid::NewGuid();
}
