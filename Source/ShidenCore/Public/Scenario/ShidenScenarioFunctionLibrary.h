// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "System/ShidenSubsystem.h"
#include "Variable/ShidenVariableFunctionLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetRegistry/AssetData.h"
#include "System/ShidenLoadingAssetInfo.h"
#include "ShidenScenarioFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenScenarioFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario")
	static bool IsScenarioPlaying();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void SetCurrentScenarioIndex(const FString& ProcessName, const int32 CurrentIndex);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void RequestCancelScenario(const FString& ProcessName, const FString& Reason, EShidenCancelType CancelType);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (Reason = ""))
	static void RequestCancelScenarioAll(const FString& Reason, const EShidenCancelType CancelType);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void IsCancelRequested(const FString& ProcessName, bool& bIsCancelRequested, FString& CancelReason, EShidenCancelType& CancelType);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static void MarkAsRead();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario", meta = (ProcessName = "Default"))
	static bool IsRead();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Scenario")
	static bool CanSkipCommand();

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void PopScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress, bool& bIsLastElement);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void PushScenario(const FString& ProcessName, const UShidenScenario* Scenario);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void RemoveScenario(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void PeekScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress, FShidenCancelInfo& CancelInfo, bool& bIsLastElement, bool& bIsMacro, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static void ToNext(const FString& ProcessName, int& NextIndex);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Scenario", meta = (ProcessName = "Default"))
	static bool IsEndOfScenario(const FString& ProcessName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void GetScenarioFromCache(const FGuid& ScenarioId, UShidenScenario*& Scenario, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void GetScenarioByIdOrObjectPath(const FString& ScenarioIdOrObjectPath, FGuid& ScenarioId, UShidenScenario*& Scenario, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (bReplaceOnlyReadOnlyVariable = false))
	static void ConstructCommand(const FString& ProcessName, const FShidenCommand& OriginalCommand, FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario", meta = (bReplaceOnlyReadOnlyVariable = false))
	static void ConstructCommandForLoad(const FShidenVariable& TempLocalVariables, const FShidenCommand& OriginalCommand, FShidenCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Utility")
	static void GetCommandFromCache(UObject* Outer, const FString& ProcessName, const FSoftObjectPath& CommandSoftObjectPath, UShidenCommandObject*& Command, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void LoadScenarioAssetPaths(UObject* CallerObject, const UShidenScenario* Scenario, TArray<FShidenLoadingAssetInfo>& AssetInfo, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void LoadScenarioAssets(UObject* Outer, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void InitFromSaveData(const UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface> ShidenManager, UObject* CallerObject, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Scenario")
	static void GetScenarioByPackagePath(const FString& PackagePath, UShidenScenario*& OutScenario, bool& bSuccess);
};
