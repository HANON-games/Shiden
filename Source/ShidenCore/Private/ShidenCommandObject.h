// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShidenCommandObject.generated.h"

UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class SHIDENCORE_API UShidenCommandObject : public UObject
{
	GENERATED_BODY()
};
