// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "GameFramework/SaveGame.h"
#include "Variable/ShidenVariable.h"
#include "Scenario/ShidenScenarioProgressStack.h"
#include "Scenario/ShidenScenarioProperties.h"
#include "Variable/ShidenLocalVariable.h"
#include "ShidenUserSaveGame.generated.h"

UCLASS(Category = "Shiden Visual Novel|Save Game")
class SHIDENCORE_API UShidenUserSaveGame : public USaveGame
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime UpdatedAt;
};
