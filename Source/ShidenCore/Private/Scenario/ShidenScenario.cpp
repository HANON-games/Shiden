// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Scenario/ShidenScenario.h"

SHIDENCORE_API void UShidenScenario::PostDuplicate(const bool bDuplicateForPie)
{
	Super::PostDuplicate(bDuplicateForPie);
	ScenarioId = FGuid::NewGuid();
}
