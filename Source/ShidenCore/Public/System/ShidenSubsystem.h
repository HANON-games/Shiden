// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "ShidenReadLines.h"
#include "Subsystems/EngineSubsystem.h"
#include "Containers/Ticker.h"
#include "Components/Border.h"
#include "Widgets/SWidget.h"
#include "UI/ShidenFadeParams.h"
#include "Variable/ShidenVariable.h"
#include "Variable/ShidenPredefinedSystemVariable.h"
#include "Scenario/ShidenScenarioProgressStack.h"
#include "Scenario/ShidenScenarioProperties.h"
#include "Scenario/ShidenScenario.h"
#include "System/ShidenBacklogItem.h"
#include "Command/ShidenCommandDefinition.h"
#include "Command/ShidenCommandObject.h"
#include "Engine/World.h"
#include "UObject/Object.h"
#include "Variable/ShidenLocalVariable.h"
#include "Audio/ShidenSoundInfo.h"
#include "ShidenSubsystem.generated.h"

USTRUCT()
struct SHIDENCORE_API FShidenGlobalBGMInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY()
	FShidenSoundInfo SoundInfo;
};

USTRUCT()
struct SHIDENCORE_API FShidenScreenFadeLayer
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UBorder> Widget;

	UPROPERTY()
	FShidenFadeParams Params;
};

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

	UPROPERTY()
	TMap<FString, FShidenScreenFadeLayer> ScreenFadeLayers;

	UPROPERTY()
	TMap<int32, FShidenGlobalBGMInfo> GlobalBGMComponents;

	FTSTicker::FDelegateHandle TickerHandle;

	bool TickScreenFade(const float DeltaTime);

	void SetDefaultPredefinedSystemVariables();

#if WITH_EDITOR
	void BeginPlay(bool);
#endif

	UShidenSubsystem();

private:
	void InitializeSubsystemState();
};
