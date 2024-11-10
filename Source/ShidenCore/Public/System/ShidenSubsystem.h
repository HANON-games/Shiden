// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

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
#include "ShidenSubsystem.generated.h"

UCLASS(Category = "Shiden Visual Novel|Subsystem")
class SHIDENCORE_API UShidenSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Subsystem")
	bool IsAutoTextMode() const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Subsystem")
	void SetAutoTextMode(bool bMode);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenVariable SystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FShidenVariable UserVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FShidenVariable> LocalVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FShidenScenarioProperties> CurrentScenarioProperties;

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

	UFUNCTION()
	void SetDefaultPredefinedSystemVariables() {
		const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault <UShidenProjectConfig>();
		PredefinedSystemVariable = ShidenProjectConfig->PredefinedSystemVariable;
	}

#if WITH_EDITOR
	void BeginPlay(UWorld* InWorld)
	{
		SystemVariable = FShidenVariable();
		UserVariable = FShidenVariable();
		LocalVariables = TMap<FString, FShidenVariable>();
		CurrentScenarioProperties = TMap<FString, FShidenScenarioProperties>();
		ScenarioProgressStack = TMap<FString, FShidenScenarioProgressStack>();
		BacklogItems = TArray<FShidenBacklogItem>();
		SetDefaultPredefinedSystemVariables();
	}
#endif

	UShidenSubsystem()
	{
		SystemVariable = FShidenVariable();
		UserVariable = FShidenVariable();
		LocalVariables = TMap<FString, FShidenVariable>();
		CurrentScenarioProperties = TMap<FString, FShidenScenarioProperties>();
		ScenarioProgressStack = TMap<FString, FShidenScenarioProgressStack>();
		BacklogItems = TArray<FShidenBacklogItem>();
		SetDefaultPredefinedSystemVariables();
#if WITH_EDITOR
		FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UShidenSubsystem::BeginPlay);
#endif
	}

private:
	bool bAutoTextMode = false;
};