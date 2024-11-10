// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Scenario/ShidenScenarioManagerInterface.h"
#include "UI/ShidenWidget.h"
#include "Command/ShidenCommand.h"
#include "ShidenCommandObject.generated.h"

UENUM(BlueprintType)
enum class EShidenInitFromSaveDataStatus : uint8
{
	Complete = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenPreProcessStatus : uint8
{
	Complete = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenPreviewStatus : uint8
{
	Complete = 0,
	Error
};

UENUM(BlueprintType)
enum class EShidenProcessStatus : uint8
{
	Next = 0,
	Break,
	Complete,
	Error,
	DelayUntilNextTick
};

UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class SHIDENCORE_API UShidenCommandObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void RestoreFromSaveData(const TMap<FString, FString>& ScenarioProperties, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void PreviewCommand(const FShidenCommand& Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage);

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void PreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage);

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void ProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime, const UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage);

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime, const UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void GetAssetPaths(const FShidenCommand Command, TSet<FString>& AssetObjectPaths, const UObject* CallerObject);

	virtual void GetAssetPaths_Implementation(const FShidenCommand Command, TSet<FString>& AssetObjectPaths, const UObject* CallerObject);
};
