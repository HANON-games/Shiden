// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "GameFramework/SaveGame.h"
#include "ShidenBaseSaveGame.generated.h"

UCLASS(Abstract)
class SHIDENCORE_API UShidenBaseSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime UpdatedAt;
};
