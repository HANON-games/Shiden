// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/ShidenCommandObject.h"
#include "Serialization/JsonSerializer.h"

SHIDENCORE_API void UShidenCommandObject::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
                                                                             UShidenWidget* ShidenWidget,
                                                                             const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                             UObject* CallerObject,
                                                                             EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	Status = EShidenInitFromSaveDataStatus::Complete;
	ErrorMessage = FString();
}

SHIDENCORE_API void UShidenCommandObject::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                        const bool bIsCurrentCommand, EShidenPreviewStatus& Status,
                                                                        FString& ErrorMessage)
{
	Status = EShidenPreviewStatus::Complete;
	ErrorMessage = FString();
}

SHIDENCORE_API void UShidenCommandObject::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                           UShidenWidget* ShidenWidget,
                                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                           UObject* CallerObject, EShidenPreProcessStatus& Status,
                                                                           FString& ErrorMessage)
{
	Status = EShidenPreProcessStatus::Complete;
	ErrorMessage = FString();
}

SHIDENCORE_API void UShidenCommandObject::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                        UShidenWidget* ShidenWidget,
                                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                        const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                        FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	Status = EShidenProcessStatus::Next;
	BreakReason = FString();
	NextScenarioName = FString();
	ErrorMessage = FString();
}
