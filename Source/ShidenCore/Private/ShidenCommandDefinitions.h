// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShidenCommandDefinition.h"
#include "Engine/DataAsset.h"
#include "ShidenCommandDefinitions.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenCommandDefinitions : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap< FString, FShidenCommandDefinition > CommandDefinitions = TMap< FString, FShidenCommandDefinition >();
};
