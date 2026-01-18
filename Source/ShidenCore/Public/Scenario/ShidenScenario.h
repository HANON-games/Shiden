// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommand.h"
#include "Engine/DataAsset.h"
#include "UI/ShidenWidget.h"
#include "Variable/ShidenMacroParameter.h"
#include "Variable/ShidenVariableDefinition.h"

#include "ShidenScenario.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenScenario : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGuid ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Note;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenCommand> Commands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenMacroParameter> MacroParameterDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenVariableDefinition> LocalVariableDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSubclassOf<UShidenWidget> WidgetClassOverride;

	UShidenScenario()
		: ScenarioId(FGuid::NewGuid())
	{
	}

	virtual void PostDuplicate(const bool bDuplicateForPie) override;
};
