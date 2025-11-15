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

		TArray<FString> Texts;
	};

	GENERATED_BODY()

public:
	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
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
	TArray<FString> CurrentTexts;

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

	/**
	 * Updates internal timing calculations for text display.
	 * @param DeltaTime Time elapsed since the last frame in seconds
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	void UpdateTime(float DeltaTime);

	/**
	 * Determines if voice audio should be played based on path and current voice state.
	 * @param VoicePath The file path to the voice audio
	 * @param VoiceState The current state of voice playback
	 * @return True if voice should be played
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool ShouldPlayVoice(const FString& VoicePath, EShidenTextCommandVoiceState VoiceState);

	/**
	 * Determines if text blip sound effects should be played during text display.
	 * @param TextBlipPath The file path to the text blip audio
	 * @param VoicePath The file path to the voice audio
	 * @return True if text blip should be played
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool ShouldPlayTextBlip(const FString& TextBlipPath, const FString& VoicePath);

	/**
	 * Checks if an asset path string is empty.
	 * @param VoicePath The asset path to check
	 * @return True if the path is empty
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool IsAssetPathEmpty(const FString& VoicePath);

	/**
	 * Checks if the current text display animation has completed.
	 * @return True if all text has been displayed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool IsTextDisplayComplete() const noexcept;

	/**
	 * Attempts to close a text window widget based on the specified text type.
	 * @param TextWidget The text widget to close
	 * @param TextType The type of text display
	 * @param ErrorMessage Output parameter containing error details if the operation fails
	 * @return True if the text window was successfully closed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryCloseTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage);

	/**
	 * Attempts to open a text window widget based on the specified text type.
	 * @param TextWidget The text widget to open
	 * @param TextType The type of text display
	 * @param ErrorMessage Output parameter containing error details if the operation fails
	 * @return True if the text window was successfully opened
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryOpenTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage);

	/**
	 * Finds a text widget by name within the Shiden widget.
	 * @param ShidenWidget The Shiden widget to search within
	 * @param TextWidgetName The name of the text widget to find
	 * @param TextWidget Output parameter containing the found text widget
	 * @param ErrorMessage Output parameter containing error details if the widget is not found
	 * @return True if the text widget was successfully found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryFindTextWidget(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, UShidenTextWidget*& TextWidget,
	                       FString& ErrorMessage);

	/**
	 * Parses a property key string to extract text widget name and text type information.
	 * @param PropertyKey The property key string to parse
	 * @param TextWidgetName Output parameter containing the extracted text widget name
	 * @param TextType Output parameter containing the extracted text type
	 * @param ErrorMessage Output parameter containing error details if parsing fails
	 * @return True if the property key was successfully parsed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryParsePropertyKey(const FString& PropertyKey, FString& TextWidgetName, FString& TextType, FString& ErrorMessage);

	/**
	 * Attempts to restore a text widget's state from saved data during load operations.
	 * @param ShidenWidget The Shiden widget containing the text widget
	 * @param TextWidgetName The name of the text widget to restore
	 * @param TextType The type of text display
	 * @param TextValue The saved text content to restore
	 * @param ErrorMessage Output parameter containing error details if restoration fails
	 * @return True if the text widget state was successfully restored
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryRestoreTextWidget(UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType, const FString& TextValue,
	                          FString& ErrorMessage);

	/**
	 * Determines if text processing should be paused.
	 * @param ShidenWidget The Shiden widget to check
	 * @return True if text processing should be paused
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool ShouldPauseTextProcess(const UShidenWidget* ShidenWidget);

	/**
	 * Determines if the current voice playback should be stopped.
	 * @param VoicePath The file path to the voice audio
	 * @param bDisableAutoStopPreviousVoices If true, prevents automatic stopping of previous voices
	 * @return True if voice playback should be stopped
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool ShouldStopVoice(const FString& VoicePath, const bool bDisableAutoStopPreviousVoices);

	/**
	 * Updates the current voice playback state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	void UpdateVoiceState();

	/**
	 * Handles player input state for text progression and skip functionality.
	 * @param ShidenWidget The Shiden widget for input checking
	 * @param ShidenManager The Shiden manager interface
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	void HandleInputState(const UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	/**
	 * Attempts to set the text window visibility state.
	 * @param ShidenWidget The Shiden widget containing the text window
	 * @param ErrorMessage Output parameter containing error details if the operation fails
	 * @return True if the text window visibility was successfully set
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TrySetTextWindowVisible(UShidenWidget* ShidenWidget, FString& ErrorMessage);

	/**
	 * Attempts to process voice audio playback for the current text command.
	 * @param VoiceTrackId The audio track ID for voice playback
	 * @param VoicePath The file path to the voice audio
	 * @param ShidenManager The Shiden manager interface
	 * @param ErrorMessage Output parameter containing error details if playback fails
	 * @return True if voice playback was successfully processed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryProcessVoicePlayback(int32 VoiceTrackId, const FString& VoicePath, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                             FString& ErrorMessage);

	/**
	 * Attempts to process text blip sound effect playback during text display.
	 * @param TextBlipPath The file path to the text blip audio
	 * @param ShidenManager The Shiden manager interface
	 * @param ErrorMessage Output parameter containing error details if playback fails
	 * @return True if text blip playback was successfully processed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryProcessTextBlipPlayback(const FString& TextBlipPath, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                FString& ErrorMessage);

	/**
	 * Attempts to update text display progress during animation.
	 * @param TextWidgetName The name of the text widget to update
	 * @param TextType The type of text display
	 * @param bInstantTextDisplay If true, displays all text immediately without animation
	 * @param ShidenWidget The Shiden widget containing the text widget
	 * @param DeltaTime Time elapsed since the last frame in seconds
	 * @param bTextUpdated Output parameter indicating if text was actually updated
	 * @param ErrorMessage Output parameter containing error details if update fails
	 * @return True if text progress was successfully updated
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryUpdateTextProgress(const FString& TextWidgetName, const FString& TextType, bool bInstantTextDisplay, UShidenWidget* ShidenWidget,
	                           float DeltaTime, bool& bTextUpdated, FString& ErrorMessage);

	/**
	 * Determines if the text command can transition to complete state.
	 * @param bWaitForInput If true, waits for player input before completing
	 * @return True if the command can transition to complete state
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool CanTransitionToComplete(const bool bWaitForInput) const;

	/**
	 * Attempts to preview text content for editor.
	 * @param ShidenWidget The Shiden widget containing the text widget
	 * @param TextWidgetName The name of the text widget to preview in
	 * @param TextType The type of text display
	 * @param Texts Array of text strings to preview
	 * @param bContinueFromThePreviousText If true, continues text from previous content
	 * @param ErrorMessage Output parameter containing error details if preview fails
	 * @return True if text preview was successfully displayed
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryPreviewText(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
	                    const TArray<FString>& Texts, bool bContinueFromThePreviousText, FString& ErrorMessage);

	/**
	 * Attempts to retrieve the complete text content from a text widget.
	 * @param TextWidget The text widget to get content from
	 * @param TextType The type of text display
	 * @param Text Output parameter containing the retrieved text content
	 * @param ErrorMessage Output parameter containing error details if retrieval fails
	 * @return True if text content was successfully retrieved
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	bool TryGetFullText(UShidenTextWidget* TextWidget, const FString& TextType, FString& Text, FString& ErrorMessage);

	/**
	 * Event called when a text window is opened or closed, allowing for custom handling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shiden Visual Novel|Command|Text", meta = (BlueprintProtected))
	void OnTextWindowOpenedOrClosed();

private:
	FTextCommandArgs Args;

	UPROPERTY()
	TSoftObjectPtr<UInputAction> NextInputAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenNext.IA_ShidenNext")));

	UPROPERTY()
	TSoftObjectPtr<UInputAction> SkipInputAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenSkip.IA_ShidenSkip")));

	static void ParseCommand(const FShidenCommand& Command, FTextCommandArgs& OutArgs);

	void UpdateSkipState(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* ShidenWidget);

	void UpdateTalkState(const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	float CalculateWaitTime(const int32 CurrentIndex);

	UInputAction* GetSkipInputAction() const;

	UInputAction* GetNextInputAction() const;

	static bool TryGetLanguageIndex(int32& LanguageIndex, FString& ErrorMessage);

	static FString GetTextByLanguageIndex(const TArray<FString>& Texts, const int32 LanguageIndex);
};
