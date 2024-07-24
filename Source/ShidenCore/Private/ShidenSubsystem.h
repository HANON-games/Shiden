// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "ShidenVariable.h"
#include "ShidenPredefinedSystemVariable.h"
#include "ShidenProjectConfig.h"
#include "ShidenScenarioProgressStack.h"
#include "ShidenScenarioProperties.h"
#include "ShidenBacklogItem.h"
#include "ShidenWidget.h"
#include "ShidenCommandDefinition.h"
#include "ShidenScenario.h"
#include "ShidenCommandObject.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "ShidenSubsystem.generated.h"

UCLASS(Category = "Shiden Visual Novel|Subsystem")
class SHIDENCORE_API UShidenSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Subsystem")
	bool IsPreviewMode() const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Subsystem")
	void SetPreviewMode(bool bMode);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Subsystem")
	bool IsAutoTextMode() const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Subsystem")
	void SetAutoTextMode(bool bMode);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	FShidenVariable SystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	FShidenVariable UserVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FString, FShidenVariable> LocalVariables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FString, FShidenScenarioProperties> CurrentScenarioProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FString, FShidenScenarioProgressStack> ScenarioProgressStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TArray<FShidenBacklogItem> BacklogItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TSubclassOf<class UShidenWidget> CurrentWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FString, FShidenCommandDefinition> CommandDefinitionCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FGuid, TObjectPtr<UShidenScenario>> ScenarioCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FString, TObjectPtr<UObject>> AssetCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap<FString, TObjectPtr<UShidenCommandObject>> CommandCache;

	UFUNCTION()
	void SetDefaultPredefinedSystemVariables() {
		const UShidenProjectConfig* ShidenProjectConfig = GetDefault <UShidenProjectConfig>();
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
		CurrentWidgetClass = nullptr;
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
		CurrentWidgetClass = nullptr;
		SetDefaultPredefinedSystemVariables();
#if WITH_EDITOR
		FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UShidenSubsystem::BeginPlay);
#endif
	}

private:
	bool bPreviewMode = false;
	bool bAutoTextMode = false;
};