// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShidenVariable.h"
#include "ShidenPredefinedSystemVariable.h"
#include "ShidenSystemSaveGame.generated.h"

UCLASS(Category = "Shiden Visual Novel|Save Game")
class SHIDENCORE_API UShidenSystemSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenVariable SystemVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenPredefinedSystemVariable PredefinedSystemVariables;
};
