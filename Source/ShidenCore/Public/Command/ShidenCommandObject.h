// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Scenario/ShidenManagerInterface.h"
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
	void RestoreFromSaveData(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* Widget,
	                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                         UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* Widget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
	                                                EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command")
	void PreviewCommand(const FShidenCommand& Command, UShidenWidget* Widget,
	                    const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                    bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage);

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status,
	                                           FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command", meta = (ProcessName = "Default"))
	void PreProcessCommand(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                       const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                       UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage);

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
	                                              EShidenPreProcessStatus& Status, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Command", meta = (ProcessName = "Default"))
	void ProcessCommand(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                    const TScriptInterface<IShidenManagerInterface>& ShidenManager, float DeltaTime,
	                    UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                    FString& NextScenarioName, FString& ErrorMessage);

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage);
};
