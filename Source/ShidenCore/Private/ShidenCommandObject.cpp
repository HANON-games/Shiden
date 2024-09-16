// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenCommandObject.h"

SHIDENCORE_API void UShidenCommandObject::InitFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	Status = EShidenInitFromSaveDataStatus::Completed;
	ErrorMessage = TEXT("");
}

SHIDENCORE_API void UShidenCommandObject::PreviewCommmand_Implementation(const FShidenCommand& Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	Status = EShidenPreviewStatus::Completed;
	ErrorMessage = TEXT("");
}

SHIDENCORE_API void UShidenCommandObject::PreProcessCommmand_Implementation(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	Status = EShidenPreProcessStatus::Completed;
	ErrorMessage = TEXT("");
}

SHIDENCORE_API void UShidenCommandObject::ProcessCommmand_Implementation(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime, const UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	Status = EShidenProcessStatus::Next;
	BreakReason = TEXT("");
	NextScenarioName = TEXT("");
	ErrorMessage = TEXT("");
}

SHIDENCORE_API void UShidenCommandObject::GetAssetPaths_Implementation(const FShidenCommand Command, TSet<FString>& AssetObjectPaths, const UObject* CallerObject)
{
	AssetObjectPaths.Empty();
}
