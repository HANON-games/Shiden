// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShidenSubsystem.h"
#include "ShidenVariableFunctionLibrary.h"
#include "ShidenScenarioFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenScenarioFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario")
	static bool IsScenarioPlaying();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void PopScenario(const FString ProcessName, FShidenScenarioProgress& ScenarioProgress, bool& bIsLastElement);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void PushScenario(const FString ProcessName, const UShidenScenario* Scenario);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void RemoveScenario(const FString ProcessName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void SetCurrentScenarioIndex(const FString ProcessName, const int32 currentIndex);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void PeekScenario(const FString ProcessName, FShidenScenarioProgress& ScenarioProgress, FShidenCancelInfo& CancelInfo, bool& bIsLastElement, bool& bIsMacro, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void ToNext(const FString ProcessName, int& NextIndex);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void RequestCancelScenario(const FString ProcessName, const FString Reason, const EShidenCancelType CancelType);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario")
	static void RequestCancelScenarioAll(const FString Reason, const EShidenCancelType CancelType);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void IsCancelRequested(const FString ProcessName, bool& bIsCancelRequested, FString& CancelReason, EShidenCancelType& CancelType);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void MarkReadLine(const FString ProcessName, const FGuid CommandGuid);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static bool IsAlreadyRead(const FString ProcessName, const FGuid CommandGuid);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void CanSkipCommand(bool& bReturnValue);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static bool IsEndOfScenario(const FString ProcessName);

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Internal|Scenario")
	static void GetScenarioFromCache(const FGuid ScenarioId, UShidenScenario*& Scenario, bool& bSuccess);
};