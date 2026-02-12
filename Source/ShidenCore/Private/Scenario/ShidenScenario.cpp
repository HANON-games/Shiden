// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Scenario/ShidenScenario.h"

void UShidenScenario::PostDuplicate(const bool bDuplicateForPie)
{
	Super::PostDuplicate(bDuplicateForPie);
	ScenarioId = FGuid::NewGuid();
}
