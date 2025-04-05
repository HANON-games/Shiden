// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "GameFramework/SaveGame.h"
#include "System/ShidenReadLines.h"
#include "Variable/ShidenVariable.h"
#include "Variable/ShidenPredefinedSystemVariable.h"
#include "ShidenSystemSaveGame.generated.h"

UCLASS(Category = "Shiden Visual Novel|Save Game")
class SHIDENCORE_API UShidenSystemSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenVariable SystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenPredefinedSystemVariable PredefinedSystemVariable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FGuid, FShidenReadLines> ScenarioReadLines;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime UpdatedAt;
};
