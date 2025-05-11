// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenTextCommand.generated.h"

UENUM(BlueprintType)
enum class EShidenTextCommandState : uint8
{
	WaitingForAnimation,
	PausingText,
	ProcessingText,
	WaitingForInput
};

UENUM(BlueprintType)
enum class EShidenTextCommandVoiceState : uint8
{
	None,
	Playing,
	Completed,
};

UCLASS()
class SHIDENCORE_API UShidenTextCommand : public UShidenCommandObject
{
	struct FTextCommandArgs
	{
		FString TextWidgetName;

		FString TextType;

		bool bWaitForInput;

		bool bInstantTextDisplay;

		FString VoicePath;

		FString TextBlipPath;

		int32 VoiceTrackId;

		bool bDisableAutoStopPreviousVoices;

		bool bContinueFromThePreviousText;

		FString Text;
	};

	GENERATED_BODY()

public:
	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage) override;

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
	                                           FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenTextCommandState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString CurrentText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 CurrentTextIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 TextLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float WaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float TextBlipWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 TextBlipCharacterCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float TotalElapsedTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float VoiceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenTextCommandVoiceState CurrentVoiceState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bTalkButtonPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bTalkButtonPressStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bSkipButtonPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bSkipButtonPressStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bIsWaitingForAnimation;

	UFUNCTION(BlueprintCallable, Category = "Default")
	void UpdateTime(float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool ShouldPlayVoice(const FString& VoicePath, EShidenTextCommandVoiceState VoiceState);
	
	UFUNCTION(BlueprintPure, Category = "Default")
	bool ShouldPlayTextBlip(const FString& TextBlipPath, const FString& VoicePath) const;

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool IsAssetPathEmpty(const FString& VoicePath);

	UFUNCTION(BlueprintPure, Category = "Default")
	bool IsTextDisplayComplete() const;

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool TryGetLanguageSpecificText(const FShidenCommand& Command, FString& Text, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryCloseTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryOpenTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool FindTextWidget(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, UShidenTextWidget*& TextWidget,
	                           FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool TryParsePropertyKey(const FString& PropertyKey, FString& TextWidgetName, FString& TextType, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	static bool TryRestoreTextWidget(UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType, const FString& TextValue,
	                                 FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool ShouldPauseTextProcess(const UShidenWidget* ShidenWidget);

	UFUNCTION(BlueprintPure, Category = "Default")
	static bool ShouldStopVoice(const FString& VoicePath, const bool bDisableAutoStopPreviousVoices);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void UpdateVoiceState();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void HandleInputState(const UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	UFUNCTION(BlueprintCallable, Category = "Default")
	static bool TrySetTextWindowVisible(UShidenWidget* ShidenWidget, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryProcessVoicePlayback(int32 VoiceTrackId, const FString& VoicePath, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                             FString& ErrorMessage);
	
	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryProcessTextBlipPlayback(const FString& TextBlipPath,
	                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                       FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryUpdateTextProgress(const FString& TextWidgetName, const FString& TextType, bool bInstantTextDisplay, UShidenWidget* ShidenWidget,
	                           float DeltaTime, bool& bTextUpdated, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Default")
	bool CanTransitionToComplete(bool bWaitForInput) const;

	UFUNCTION(BlueprintCallable, Category = "Default")
	static bool TryPreviewText(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
	                           const FString& Text, bool bContinueFromThePreviousText, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	static bool TryGetFullText(UShidenTextWidget* TextWidget, const FString& TextType, FString& Text, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	void OnTextWindowOpenedOrClosed();

private:
	FTextCommandArgs Args;

	static bool TryParseCommand(const FShidenCommand& Command, FTextCommandArgs& OutArgs, FString& ErrorMessage);

	void UpdateSkipState(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* ShidenWidget);

	void UpdateTalkState(const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	float CalculateWaitTime(const int32 CurrentIndex);

	static UInputAction* LoadInputActionFromPath(const FString& Path);
};
