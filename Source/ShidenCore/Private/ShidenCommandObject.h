// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "ShidenScenarioManagerInterface.h"
#include "ShidenWidget.h"
#include "ShidenCommand.h"
#include "ShidenCommandObject.generated.h"

UENUM(BlueprintType)
enum class EShidenInitFromSaveDataStatus : uint8
{
	Completed = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenPreProcessStatus : uint8
{
	Completed = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenPreviewStatus : uint8
{
	Completed = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenProcessStatus : uint8
{
	Next = 0,
	Break,
	Completed,
	Error,
	DelayUntilNextTick
};

UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class SHIDENCORE_API UShidenCommandObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void InitFromSaveData(const TMap<FString, FString>& ScenarioProperties, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	virtual void InitFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
	{
		Status = EShidenInitFromSaveDataStatus::Completed;
		ErrorMessage = TEXT("");
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void PreviewCommmand(const FShidenCommand& Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage);

	virtual void PreviewCommmand_Implementation(const FShidenCommand& Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
	{
		Status = EShidenPreviewStatus::Completed;
		ErrorMessage = TEXT("");
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void PreProcessCommmand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage);

	virtual void PreProcessCommmand_Implementation(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
	{
		Status = EShidenPreProcessStatus::Completed;
		ErrorMessage = TEXT("");
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void ProcessCommmand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime, const UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage);

	virtual void ProcessCommmand_Implementation(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime, const UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
	{
		Status = EShidenProcessStatus::Next;
		BreakReason = TEXT("");
		NextScenarioName = TEXT("");
		ErrorMessage = TEXT("");
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void GetAssetPaths(const FShidenCommand Command, TSet<FString>& AssetObjectPaths, const UObject* CallerObject);

	virtual void GetAssetPaths_Implementation(const FShidenCommand Command, TSet<FString>& AssetObjectPaths, const UObject* CallerObject)
	{
		AssetObjectPaths.Empty();
	}
};
