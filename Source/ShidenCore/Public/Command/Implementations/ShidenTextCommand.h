// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenTextCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenTextCommand : public UShidenCommandObject
{
    GENERATED_BODY()

    struct FTextCommandArgs
    {
        FString TextWidgetName;
        FString TextType;
        FString Text;
    	bool bWaitForInput;
    	bool bInstantTextDisplay;
    	FString VoicePath;
        int32 VoiceTrackId;
        bool bDisableAutoStopPreviousVoices;
        bool bContinueFromThePreviousText;
    };

    virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties,
                                                    UShidenWidget* Widget,
                                                    const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                    UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                    FString& ErrorMessage) override;

    virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                  UShidenWidget* Widget,
                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                  UObject* CallerObject, EShidenPreProcessStatus& Status,
                                                  FString& ErrorMessage) override;

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

    static bool TryParseCommand(const FShidenCommand& Command, FTextCommandArgs& Args, FString& ErrorMessage);
    
	static bool TryGetTextFromCommand(const FShidenCommand& Command, FString& Text, FString& ErrorMessage);
    
	static bool ShouldStopVoice(const bool& bIsVoicePathEmpty, const bool& bDisableAutoStopPreviousVoices);
    
	void UpdateInput(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* Widget);
    
	static UInputAction* LoadInputActionFromPath(const FString& Path);

    UFUNCTION()
    void OnTextWindowOpenedOrClosed();

    FTextCommandArgs Args;

	bool bTalkButtonPressStart = false;

	bool bTalkButtonPressed = false;

	bool bSkipButtonPressStart = false;

	bool bSkipButtonPressed = false;

	float WaitTime = 0.f;

	float TotalElapsedTime = 0.f;

	FString CurrentText = TEXT("");

	int32 CurrentTextIndex = -1;

	int32 TextLength = 0;

	bool bIsWaitingForAnimation = false;

	float VoiceDuration = 0.f;

	bool VoiceStarted = false;
};
