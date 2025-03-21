// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "Editor.h"
#endif
#include "ShidenReadLines.h"
#include "Subsystems/EngineSubsystem.h"
#include "Variable/ShidenVariable.h"
#include "Variable/ShidenPredefinedSystemVariable.h"
#include "Config/ShidenProjectConfig.h"
#include "Scenario/ShidenScenarioProgressStack.h"
#include "Scenario/ShidenScenarioProperties.h"
#include "Scenario/ShidenScenario.h"
#include "System/ShidenBacklogItem.h"
#include "Command/ShidenCommandDefinition.h"
#include "Command/ShidenCommandObject.h"
#include "Engine/World.h"
#include "UObject/Object.h"
#include "Variable/ShidenLocalVariable.h"
#include "ShidenSubsystem.generated.h"

UCLASS(Category = "Shiden Visual Novel|Subsystem")
class SHIDENCORE_API UShidenSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SvnInternal")
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenVariable SystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenVariable UserVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenLocalVariable LocalVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FShidenScenarioProperties> ScenarioProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FShidenScenarioProgressStack> ScenarioProgressStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TArray<FShidenBacklogItem> BacklogItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FShidenCommandDefinition> CommandDefinitionCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FGuid, TObjectPtr<UShidenScenario>> ScenarioCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, TObjectPtr<UObject>> AssetCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, TObjectPtr<UShidenCommandObject>> CommandCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FGuid, FShidenReadLines> ScenarioReadLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	bool bAutoTextMode = false;
	
	UFUNCTION()
	void SetDefaultPredefinedSystemVariables()
	{
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		PredefinedSystemVariable = FShidenPredefinedSystemVariable(ShidenProjectConfig->PredefinedSystemVariable);
	}

#if WITH_EDITOR
	void BeginPlay(bool bInBool)
	{
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
		UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
		LocalVariable = FShidenLocalVariable();
		ScenarioProperties = TMap<FString, FShidenScenarioProperties>();
		ScenarioProgressStack = TMap<FString, FShidenScenarioProgressStack>();
		BacklogItems = TArray<FShidenBacklogItem>();
		ScenarioReadLines = TMap<FGuid, FShidenReadLines>();
		SetDefaultPredefinedSystemVariables();
	}
#endif

	UShidenSubsystem()
	{
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
		SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
		UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
		LocalVariable = FShidenLocalVariable();
		ScenarioProperties = TMap<FString, FShidenScenarioProperties>();
		ScenarioProgressStack = TMap<FString, FShidenScenarioProgressStack>();
		BacklogItems = TArray<FShidenBacklogItem>();
		ScenarioReadLines = TMap<FGuid, FShidenReadLines>();
		SetDefaultPredefinedSystemVariables();
#if WITH_EDITOR
		FEditorDelegates::PreBeginPIE.AddUObject(this, &UShidenSubsystem::BeginPlay);
#endif
	}
};
