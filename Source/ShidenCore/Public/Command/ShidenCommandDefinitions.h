// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenCommandDefinition.h"
#include "Engine/DataAsset.h"
#include "ShidenCommandDefinitions.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenCommandDefinitions : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap<FString, FShidenCommandDefinition> CommandDefinitions = TMap<FString, FShidenCommandDefinition>();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	DECLARE_EVENT(UShidenCommandDefinitions, FOnChanged);

	FOnChanged OnCommandDefinitionsChanged;
#endif
};
