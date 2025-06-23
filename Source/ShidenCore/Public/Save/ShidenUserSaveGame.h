// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Save/ShidenBaseSaveGame.h"
#include "Variable/ShidenVariable.h"
#include "Scenario/ShidenScenarioProgressStack.h"
#include "Scenario/ShidenScenarioProperties.h"
#include "Variable/ShidenLocalVariable.h"
#include "ShidenUserSaveGame.generated.h"

UCLASS()
class SHIDENCORE_API UShidenUserSaveGame : public UShidenBaseSaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenVariable UserVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenLocalVariable LocalVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FString, FShidenScenarioProgressStack> ScenarioProgressStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FString, FShidenScenarioProperties> ScenarioProperties;
};
