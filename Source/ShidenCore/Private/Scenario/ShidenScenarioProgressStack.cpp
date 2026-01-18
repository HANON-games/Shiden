// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Scenario/ShidenScenarioProgressStack.h"

void FShidenScenarioProgressStack::UpdateCurrentScenarioIndex(const int32 Index)
{
	if (!IsEmpty())
	{
		Stack.Last().CurrentIndex = Index;
	}
}

int32 FShidenScenarioProgressStack::GetCurrentScenarioIndex() const noexcept
{
	return IsEmpty() ? INDEX_NONE : Stack.Last().CurrentIndex;
}

bool FShidenScenarioProgressStack::IsEmpty() const noexcept
{
	return Stack.Num() == 0;
}
