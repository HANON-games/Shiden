// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenario.h"
#include "ShidenRunMacroCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenRunMacroCommand : public UShidenCommandObject
{
    GENERATED_BODY()

    struct FRunMacroCommandArgs
    {
        UShidenScenario* Scenario;
        FGuid ScenarioId;
    };

    virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                               UShidenWidget* Widget,
                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                               const float DeltaTime, UObject* CallerObject,
                                               EShidenProcessStatus& Status, FString& BreakReason,
                                               FString& NextScenarioName, FString& ErrorMessage) override;

    virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                               bool bIsCurrentCommand, EShidenPreviewStatus& Status,
                                               FString& ErrorMessage) override;

    static bool TryParseCommand(const FShidenCommand& Command, FRunMacroCommandArgs& Args, FString& ErrorMessage);
    
    FRunMacroCommandArgs Args;
};
