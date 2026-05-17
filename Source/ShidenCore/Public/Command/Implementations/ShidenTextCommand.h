// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenTextCommand.generated.h"

UENUM(BlueprintType)
enum class EShidenTextCommandState : uint8
{
	WaitingForAnimation UMETA(
		DisplayName = "Waiting For Animation",
		ToolTip = "Waiting for the text window open or close animation to complete. / テキストウィンドウの開閉アニメーション完了を待っている状態です。"),

	PausingText UMETA(
		DisplayName = "Pausing Text",
		ToolTip = "Text playback is paused because the menu is open or the text is hidden. / メニューが開いている、またはテキストが非表示のため、テキスト再生を一時停止している状態です。"),

	ProcessingText UMETA(
		DisplayName = "Processing Text",
		ToolTip = "Text is being shown, and voice or text blip sounds may play as needed. / テキストを表示中で、必要に応じて voice や text blip を再生している状態です。"),

	WaitingForInput UMETA(
		DisplayName = "Waiting For Input",
		ToolTip = "All text has been shown, and the command is waiting for input. / 全文表示後、入力を待っている状態です。")
};

UENUM(BlueprintType)
enum class EShidenTextCommandVoiceState : uint8
{
	None UMETA(
		DisplayName = "None",
		ToolTip = "Voice has not started playing for the current text. / 現在のテキストの voice がまだ再生されていない状態です。"),

	Playing UMETA(
		DisplayName = "Playing",
		ToolTip = "Voice is currently playing for the current text. / 現在のテキストの voice を再生中の状態です。"),

	Completed UMETA(
		DisplayName = "Completed",
		ToolTip = "Voice playback for the current text has finished, or there is no voice to play. / 現在のテキストの voice 再生が完了した、または再生する voice がない状態です。"),
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Current state of the text command, such as waiting for animation, showing text, or waiting for input. / アニメーション待ち、テキスト表示中、入力待ちなど、TextCommand の現在の状態を表します。"))
	EShidenTextCommandState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Text to display for each language. When ContinueFromThePreviousText is enabled, text from the previous command is included. / 言語ごとに表示するテキストです。ContinueFromThePreviousText が有効な場合は、前のコマンドのテキストも含まれます。"))
	TArray<FString> CurrentTexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Text currently being displayed for the active language. / 現在の言語で表示中のテキストです。"))
	FString CurrentText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Number of parsed characters currently shown from CurrentText. / CurrentText のうち、現在表示済みの解析後文字数です。"))
	int32 CurrentTextIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Total parsed character count of CurrentText. / CurrentText の解析後の総文字数です。"))
	int32 TextLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Remaining time before the next character can be shown, including delay from wait tags. After all text is shown, auto mode uses this as the wait time before finishing the command. / 次の文字を表示できるようになるまでの残り時間です。wait タグによる待ち時間も含みます。全文表示後のオートモードでは、コマンド終了までの待ち時間として使われます。"))
	float WaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Remaining time before the next text blip can play in time interval mode. / time interval モードで、次の text blip を再生できるまでの残り時間です。"))
	float TextBlipWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Remaining character count before the next text blip can play in character interval mode. / character interval モードで、次の text blip を再生できるまでの残り文字数です。"))
	int32 TextBlipCharacterCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Elapsed time since this text command started processing. / この TextCommand の処理開始からの経過時間です。"))
	float TotalElapsedTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Duration of the currently playing voice sound. / 現在再生中の voice の再生時間です。"))
	float VoiceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "Current playback state of the voice for this text. / このテキストで再生する voice の現在の再生状態です。"))
	EShidenTextCommandVoiceState CurrentVoiceState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "True while the next input is being held down. / 次へ進む入力が押されている間 true になります。"))
	bool bTalkButtonPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "True only on the frame when the next input starts being pressed. / 次へ進む入力が押され始めたフレームだけ true になります。"))
	bool bTalkButtonPressStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "True while the skip input is being held down. / スキップ入力が押されている間 true になります。"))
	bool bSkipButtonPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "True only on the frame when the skip input starts being pressed. / スキップ入力が押され始めたフレームだけ true になります。"))
	bool bSkipButtonPressStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "True while waiting for the text window open or close animation to finish. / テキストウィンドウの開閉アニメーション完了を待っている間 true になります。"))
	bool bIsWaitingForAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default",
		meta = (ToolTip = "True when CurrentText contains a wait tag. / CurrentText に wait タグが含まれている場合 true になります。"))
	bool bHasWaitTagInCurrentText;

	/**
	 * Splits a saved text key in the form TextWidgetName::TextType.
	 * 保存テキストのキー TextWidgetName::TextType を TextWidgetName と TextType に分けます。
	 * @param PropertyKey Saved text key to split. / 分割する保存テキストのキーです。
	 * @param TextWidgetName Receives the text widget name. / テキストウィジェット名を受け取ります。
	 * @param TextType Receives the text type. / テキスト種別を受け取ります。
	 * @param ErrorMessage Receives the error message when the key cannot be split. / キーを分割できない場合のエラーメッセージを受け取ります。
	 * @return True when the key was split successfully. / キーを正しく分割できた場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|RestoreFromSaveData", meta = (BlueprintProtected, DisplayName = "Parse Property Key"))
	UPARAM(DisplayName = "Success") bool TryParsePropertyKey(const FString& PropertyKey, FString& TextWidgetName, FString& TextType, FString& ErrorMessage);

	/**
	 * Restores the text window to the state saved in save data.
	 * Opens the target text window and shows the text that was visible when the game was saved.
	 * セーブデータに保存されたテキストウィンドウの状態を復元します。
	 * 対象のテキストウィンドウを開き、セーブ時に表示されていたテキストを表示します。
	 * @param ShidenWidget Widget that contains the text widget. / テキストウィジェットを含む Shiden ウィジェットです。
	 * @param TextWidgetName Text widget name to restore. / 復元するテキストウィジェット名です。
	 * @param TextType Text type to restore. / 復元するテキスト種別です。
	 * @param TextValue Text saved in save data. / セーブデータに保存されていたテキストです。
	 * @param ErrorMessage Receives the error message when restore fails. / 復元に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the saved text state was restored. / 保存されていたテキスト状態を復元できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|RestoreFromSaveData", meta = (BlueprintProtected, DisplayName = "Restore Text Widget"))
	UPARAM(DisplayName = "Success") bool TryRestoreTextWidget(UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType, const FString& TextValue,
	                                                          FString& ErrorMessage);

	/**
	 * Reads the next and skip inputs for this frame.
	 * Updates the pressed and press-start flags used by text playback.
	 * このフレームの次へ進む入力とスキップ入力を読み取ります。
	 * テキスト再生で使う押下中フラグと押し始めフラグを更新します。
	 * @param ShidenWidget Widget used to read the skip button state. / スキップボタン状態の読み取りに使う Shiden ウィジェットです。
	 * @param ShidenManager Manager used to read input actions. / 入力アクションの読み取りに使う Shiden manager です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected))
	void HandleInputState(const UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	/**
	 * Initializes state for a new text command.
	 * The next ProcessCommand tick will wait for the text window open or close animation to finish.
	 * Resets CurrentState, CurrentVoiceState, WaitTime, TotalElapsedTime, VoiceDuration, bIsWaitingForAnimation, and the flag for wait tags in the current text.
	 * 新しい TextCommand の状態を初期化します。
	 * 次の ProcessCommand tick は、テキストウィンドウの開閉アニメーション完了を待つ状態から始まります。
	 * CurrentState、CurrentVoiceState、WaitTime、TotalElapsedTime、VoiceDuration、bIsWaitingForAnimation、現在のテキストに <wait ...> タグがあるかどうかのフラグをリセットします。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreProcessCommand", meta = (BlueprintProtected))
	void InitializeVariables();

	/**
	 * Prepares the text that will be shown by this command.
	 * Sets CurrentTexts, CurrentText, CurrentTextIndex, TextLength, and the flag for wait tags in the current text.
	 * このコマンドで表示するテキストを準備します。
	 * CurrentTexts、CurrentText、CurrentTextIndex、TextLength、現在のテキストに <wait ...> タグがあるかどうかのフラグを設定します。
	 * @param CommandName Command name used when reading previous saved text. / 前回保存されたテキストを読むために使うコマンド名です。
	 * @param TextWidgetName Text widget name used for the saved text key. / 保存テキストのキーに使うテキストウィジェット名です。
	 * @param TextType Text type used for the saved text key. / 保存テキストのキーに使うテキスト種別です。
	 * @param Texts Text for each language, starting with the main Text argument. / 先頭を Text とする言語別テキスト配列です。
	 * @param bContinueFromThePreviousText If true, appends Texts to the previous saved text. / true の場合、前回保存されたテキストの後ろに Texts を追加します。
	 * @param ErrorMessage Receives the error message when preparation fails. / 準備に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the current text was prepared. / 表示するテキストを準備できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreProcessCommand", meta = (BlueprintProtected, DisplayName = "Initialize Current Text"))
	UPARAM(DisplayName = "Success") bool TryInitializeCurrentText(const FString& CommandName, const FString& TextWidgetName,
	                                                              const FString& TextType, const TArray<FString>& Texts,
	                                                              bool bContinueFromThePreviousText, FString& ErrorMessage);

	/**
	 * Makes the text layers visible so text windows can be shown.
	 * Sets TextBaseLayer and TextLayer visibility to SelfHitTestInvisible.
	 * テキストウィンドウを表示できるように、テキスト用レイヤーを表示します。
	 * TextBaseLayer と TextLayer の表示状態を SelfHitTestInvisible に変更します。
	 * @param ShidenWidget Widget that owns the text layers. / テキスト用レイヤーを持つ Shiden ウィジェットです。
	 * @param ErrorMessage Receives the error message when a layer cannot be shown. / レイヤーを表示できない場合のエラーメッセージを受け取ります。
	 * @return True when both layers were shown. / 両方のレイヤーを表示できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected, DisplayName = "Set Text Window Visible"))
	UPARAM(DisplayName = "Success") bool TrySetTextWindowVisible(UShidenWidget* ShidenWidget, FString& ErrorMessage);

	/**
	 * Opens or closes the text window for the prepared text.
	 * Opens the window when CurrentText has text, or closes it when CurrentText is empty.
	 * 準備済みテキストに合わせて、テキストウィンドウを開くか閉じます。
	 * CurrentText が空でなければウィンドウを開き、空なら閉じます。
	 * @param ShidenWidget Widget that contains the text widget. / テキストウィジェットを含む Shiden ウィジェットです。
	 * @param TextWidgetName Text widget to open or close. / 開閉するテキストウィジェット名です。
	 * @param TextType Text type to open or close. / 開閉するテキスト種別です。
	 * @param ErrorMessage Receives the error message when the window cannot be changed. / ウィンドウ状態を変更できない場合のエラーメッセージを受け取ります。
	 * @return True when the window state was changed or requested successfully. / ウィンドウ状態の変更を開始または完了できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreProcessCommand", meta = (BlueprintProtected, DisplayName = "Apply Text Window State"))
	UPARAM(DisplayName = "Success") bool TryApplyTextWindowState(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
	                                                             FString& ErrorMessage);

	/**
	 * Checks whether previous voice sounds should be stopped before this text starts.
	 * このテキスト開始前に、前の voice を停止するべきか判定します。
	 * @param VoicePath Voice asset path for this text. / このテキストで再生する voice のアセットパスです。
	 * @param bDisableAutoStopPreviousVoices If true, previous voices are kept playing. / true の場合、前の voice を自動停止しません。
	 * @return True when PreProcessCommand should call StopSounds for voices. / PreProcessCommand が voice の StopSounds を呼ぶべき場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|PreProcessCommand", meta = (BlueprintProtected))
	UPARAM(DisplayName = "Success") bool ShouldStopVoice(const FString& VoicePath, const bool bDisableAutoStopPreviousVoices);

	/**
	 * Checks whether a voice can start now.
	 * 今 voice を開始できるか判定します。
	 * @param VoicePath Voice asset path to check. / 確認する voice のアセットパスです。
	 * @param VoiceState Current voice state. / 現在の voice 状態です。
	 * @return True when VoicePath is set and no voice has started yet. / VoicePath が設定されていて、まだ voice が開始されていない場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected))
	UPARAM(DisplayName = "Success") bool ShouldPlayVoice(const FString& VoicePath, EShidenTextCommandVoiceState VoiceState);

	/**
	 * Starts the voice for this text, or marks it complete when skipping.
	 * Calls PlaySound, sets VoiceDuration, and changes CurrentVoiceState.
	 * このテキストの voice を開始します。スキップ中は再生せず完了扱いにします。
	 * PlaySound を呼び、VoiceDuration と CurrentVoiceState を変更します。
	 * @param VoicePath Voice asset path to play. / 再生する voice のアセットパスです。
	 * @param VoiceTrackId Sound track ID used for the voice. / voice 再生に使うサウンドトラック ID です。
	 * @param ShidenManager Manager used to play sounds. / サウンド再生に使う Shiden manager です。
	 * @param ErrorMessage Receives the error message when voice playback fails. / voice 再生に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the voice was started or skipped as complete. / voice を開始できた、またはスキップにより完了扱いにできた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected, DisplayName = "Process Voice Playback"))
	UPARAM(DisplayName = "Success") bool TryProcessVoicePlayback(const FString& VoicePath, int32 VoiceTrackId, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                             FString& ErrorMessage);

	/**
	 * Advances the timers used by text, auto mode, skip, and text blips.
	 * Changes TotalElapsedTime, TextBlipWaitTime, and WaitTime.
	 * テキスト、オートモード、スキップ、text blip に使うタイマーを進めます。
	 * TotalElapsedTime、TextBlipWaitTime、WaitTime を変更します。
	 * @param DeltaTime Seconds since the previous frame. / 前フレームからの経過秒数です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	void UpdateTime(float DeltaTime);

	/**
	 * Marks the voice as complete after its duration has passed.
	 * Changes CurrentVoiceState from Playing to Completed.
	 * voice の再生時間が過ぎたら完了状態にします。
	 * CurrentVoiceState を Playing から Completed に変更します。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	void UpdateVoiceState();

	/**
	 * Moves out of WaitingForAnimation after the text window animation has finished.
	 * May change CurrentState to PausingText, ProcessingText, or WaitingForInput.
	 * テキストウィンドウのアニメーション終了後、WaitingForAnimation から次の状態へ進めます。
	 * CurrentState を PausingText、ProcessingText、WaitingForInput のいずれかに変更することがあります。
	 * @param ShidenWidget Widget used to check pause conditions. / pause 条件の確認に使う Shiden ウィジェットです。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	void HandleForWaitingForAnimationState(const UShidenWidget* ShidenWidget);

	/**
	 * Leaves PausingText when the menu is closed and the text layer is visible again.
	 * May change CurrentState to WaitingForAnimation, ProcessingText, or WaitingForInput.
	 * メニューが閉じ、テキストレイヤーが再表示されたら PausingText から戻します。
	 * CurrentState を WaitingForAnimation、ProcessingText、WaitingForInput のいずれかに変更することがあります。
	 * @param ShidenWidget Widget used to check pause conditions. / pause 条件の確認に使う Shiden ウィジェットです。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	void HandleForPausingTextState(const UShidenWidget* ShidenWidget);

	/**
	 * Updates one frame while text is being displayed.
	 * May pause the command, start voice, reveal more text, play a text blip, or move to WaitingForInput.
	 * テキスト表示中の 1 フレーム分の処理を行います。
	 * コマンドの pause、voice 開始、テキスト表示進行、text blip 再生、WaitingForInput への遷移を行うことがあります。
	 * @param ShidenWidget Widget that contains the text widget. / テキストウィジェットを含む Shiden ウィジェットです。
	 * @param ShidenManager Manager used to play sounds. / サウンド再生に使う Shiden manager です。
	 * @param DeltaTime Seconds since the previous frame. / 前フレームからの経過秒数です。
	 * @param TextWidgetName Text widget to update. / 更新するテキストウィジェット名です。
	 * @param TextType Text type to update. / 更新するテキスト種別です。
	 * @param bInstantTextDisplay If true, shows the full text immediately. / true の場合、全文をすぐに表示します。
	 * @param VoicePath Voice asset path for this text. / このテキストで再生する voice のアセットパスです。
	 * @param VoiceTrackId Sound track ID used for the voice. / voice 再生に使うサウンドトラック ID です。
	 * @param TextBlipPath Text blip asset path to play while revealing text. / テキスト表示中に再生する text blip のアセットパスです。
	 * @param ErrorMessage Receives the error message when processing fails. / 処理に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when this frame was processed without an error. / このフレームをエラーなく処理できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected, DisplayName = "Handle For Processing Text State"))
	UPARAM(DisplayName = "Success") bool TryHandleForProcessingTextState(UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                                     float DeltaTime, const FString& TextWidgetName, const FString& TextType,
	                                                                     bool bInstantTextDisplay, const FString& VoicePath, int32 VoiceTrackId,
	                                                                     const FString& TextBlipPath, FString& ErrorMessage);

	/**
	 * Checks whether WaitingForInput can finish this command.
	 * May change CurrentState to PausingText.
	 * WaitingForInput で、このコマンドを終了できるか判定します。
	 * CurrentState を PausingText に変更することがあります。
	 * @param ShidenWidget Widget used to check pause conditions. / pause 条件の確認に使う Shiden ウィジェットです。
	 * @param bWaitForInput If true, waits for the player before finishing. / true の場合、終了前にプレイヤー入力を待ちます。
	 * @param bShouldFinalize Receives true when ProcessCommand should call Finalize. / ProcessCommand が Finalize を呼ぶべき場合に true を受け取ります。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	void HandleWaitingForInputState(const UShidenWidget* ShidenWidget, bool bWaitForInput, bool& bShouldFinalize);

	/**
	 * Saves the final text state and adds the displayed text to the backlog.
	 * Writes ScenarioProperty and adds one backlog item.
	 * 最終的なテキスト状態を保存し、表示中のテキストをバックログに追加します。
	 * ScenarioProperty を書き込み、バックログ項目を 1 件追加します。
	 * @param Command Command to store in the backlog item. / バックログ項目に保存するコマンドです。
	 * @param ShidenWidget Widget that contains the text widget. / テキストウィジェットを含む Shiden ウィジェットです。
	 * @param TextWidgetName Text widget to read full texts from. / 全テキストを読み取るテキストウィジェット名です。
	 * @param TextType Text type used for the ScenarioProperty key. / ScenarioProperty のキーに使うテキスト種別です。
	 * @param ErrorMessage Receives the error message when finalize fails. / 終了処理に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when ScenarioProperty and backlog were updated. / ScenarioProperty とバックログを更新できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected, DisplayName = "Finalize"))
	UPARAM(DisplayName = "Success") bool TryFinalize(const FShidenCommand& Command, const UShidenWidget* ShidenWidget, const FString& TextWidgetName,
	                                                 const FString& TextType, FString& ErrorMessage);

	/**
	 * Shows the command text in the preview text widget.
	 * Changes the preview text shown by the target text widget.
	 * コマンドのテキストをプレビュー用テキストウィジェットに表示します。
	 * 対象テキストウィジェットに表示されるプレビューテキストを変更します。
	 * @param ShidenWidget Widget that contains the text widget. / テキストウィジェットを含む Shiden ウィジェットです。
	 * @param TextWidgetName Text widget that will show the preview. / プレビューを表示するテキストウィジェット名です。
	 * @param TextType Text type to preview. / プレビューするテキスト種別です。
	 * @param Texts Text for each language, starting with the main Text argument. / 先頭を Text とする言語別テキスト配列です。
	 * @param bContinueFromThePreviousText If true, appends this text after the currently previewed text. / true の場合、現在プレビュー中のテキストの後ろに追加して表示します。
	 * @param ErrorMessage Receives the error message when preview fails. / プレビューに失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the preview text was shown. / プレビューテキストを表示できた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreviewCommand", meta = (BlueprintProtected, DisplayName = "Preview Text"))
	UPARAM(DisplayName = "Success") bool TryPreviewText(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
	                                                    const TArray<FString>& Texts, bool bContinueFromThePreviousText, FString& ErrorMessage);

	/**
	 * Checks whether a text blip should play for the latest revealed text.
	 * 直近で表示されたテキストに対して text blip を再生するべきか判定します。
	 * @param TextBlipPath Text blip asset path. / text blip のアセットパスです。
	 * @param VoicePath Voice asset path for this text. / このテキストで再生する voice のアセットパスです。
	 * @return True when text is progressing, no voice is playing, TextBlipPath is set, and the blip interval is ready. / テキストが進行中で、voice がなく、TextBlipPath が設定され、blip 間隔が来ている場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	bool ShouldPlayTextBlip(const FString& TextBlipPath, const FString& VoicePath);

	/**
	 * Checks whether an asset path is empty or set to None.
	 * アセットパスが空、または None に設定されているか確認します。
	 * @param VoicePath Asset path to check. / 確認するアセットパスです。
	 * @return True when the path is empty or None. / パスが空、または None の場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected))
	bool IsAssetPathEmpty(const FString& VoicePath);

	/**
	 * Checks whether all characters in CurrentText have been shown.
	 * CurrentText のすべての文字が表示済みか確認します。
	 * @return True when CurrentTextIndex has reached TextLength. / CurrentTextIndex が TextLength に到達している場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	bool IsTextDisplayComplete() const noexcept;

	/**
	 * Requests the text widget to close the selected text window.
	 * Starts the close animation and clears the animation wait flag when the callback runs.
	 * テキストウィジェットに、指定したテキストウィンドウを閉じるよう依頼します。
	 * 閉じるアニメーションを開始し、コールバック実行時にアニメーション待ちフラグを解除します。
	 * @param TextWidget Text widget that owns the window. / ウィンドウを持つテキストウィジェットです。
	 * @param TextType Text type to close. / 閉じるテキスト種別です。
	 * @param ErrorMessage Receives the error message when close fails. / 閉じる処理に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the close request was accepted. / 閉じる依頼が受け付けられた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreProcessCommand", meta = (BlueprintProtected, DisplayName = "Close Text Window"))
	UPARAM(DisplayName = "Success") bool TryCloseTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage);

	/**
	 * Requests the text widget to open the selected text window.
	 * Starts the open animation and clears the animation wait flag when the callback runs.
	 * テキストウィジェットに、指定したテキストウィンドウを開くよう依頼します。
	 * 開くアニメーションを開始し、コールバック実行時にアニメーション待ちフラグを解除します。
	 * @param TextWidget Text widget that owns the window. / ウィンドウを持つテキストウィジェットです。
	 * @param TextType Text type to open. / 開くテキスト種別です。
	 * @param ErrorMessage Receives the error message when open fails. / 開く処理に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the open request was accepted. / 開く依頼が受け付けられた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreProcessCommand", meta = (BlueprintProtected, DisplayName = "Open Text Window"))
	UPARAM(DisplayName = "Success") bool TryOpenTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage);

	/**
	 * Finds a text widget by name inside the Shiden widget.
	 * Shiden ウィジェット内から、名前でテキストウィジェットを探します。
	 * @param ShidenWidget Widget to search in. / 検索対象の Shiden ウィジェットです。
	 * @param TextWidgetName Text widget name to find. / 検索するテキストウィジェット名です。
	 * @param TextWidget Receives the found text widget. / 見つかったテキストウィジェットを受け取ります。
	 * @param ErrorMessage Receives the error message when the widget is not found. / ウィジェットが見つからない場合のエラーメッセージを受け取ります。
	 * @return True when the text widget was found. / テキストウィジェットが見つかった場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected, DisplayName = "Find Text Widget"))
	UPARAM(DisplayName = "Success") bool TryFindTextWidget(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, UShidenTextWidget*& TextWidget,
	                                                       FString& ErrorMessage);

	/**
	 * Checks whether text playback should pause now.
	 * 今テキスト再生を一時停止するべきか判定します。
	 * @param ShidenWidget Widget used to check the menu and text layer. / メニューとテキストレイヤーの確認に使う Shiden ウィジェットです。
	 * @return True when the menu is open or TextBaseLayer is hidden. / メニューが開いている、または TextBaseLayer が非表示の場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	bool ShouldPauseTextProcess(const UShidenWidget* ShidenWidget);

	/**
	 * Plays one text blip sound.
	 * Calls PlaySound and resets the next blip wait value.
	 * text blip 音を 1 回再生します。
	 * PlaySound を呼び、次の blip までの待ち値をリセットします。
	 * @param TextBlipPath Text blip asset path to play. / 再生する text blip のアセットパスです。
	 * @param ShidenManager Manager used to play sounds. / サウンド再生に使う Shiden manager です。
	 * @param ErrorMessage Receives the error message when playback fails. / 再生に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the blip was played and the wait value was reset. / blip を再生し、待ち値をリセットできた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected, DisplayName = "Process Text Blip Playback"))
	UPARAM(DisplayName = "Success") bool TryProcessTextBlipPlayback(const FString& TextBlipPath, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                                FString& ErrorMessage);

	/**
	 * Reveals more characters in the text widget.
	 * Changes CurrentTextIndex, TextBlipCharacterCount, WaitTime, and the text shown by the widget.
	 * テキストウィジェットに表示する文字数を増やします。
	 * CurrentTextIndex、TextBlipCharacterCount、WaitTime、ウィジェットに表示されるテキストを変更します。
	 * @param TextWidgetName Text widget to update. / 更新するテキストウィジェット名です。
	 * @param TextType Text type to update. / 更新するテキスト種別です。
	 * @param bInstantTextDisplay If true, shows the full text immediately. / true の場合、全文をすぐに表示します。
	 * @param ShidenWidget Widget that contains the text widget. / テキストウィジェットを含む Shiden ウィジェットです。
	 * @param DeltaTime Seconds since the previous frame. / 前フレームからの経過秒数です。
	 * @param bTextUpdated Receives true when the displayed text changed this frame. / このフレームで表示テキストが変わった場合に true を受け取ります。
	 * @param ErrorMessage Receives the error message when update fails. / 更新に失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the update finished without an error. / エラーなく更新処理を終えた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected, DisplayName = "Update Text Progress"))
	UPARAM(DisplayName = "Success") bool TryUpdateTextProgress(const FString& TextWidgetName, const FString& TextType, bool bInstantTextDisplay, UShidenWidget* ShidenWidget,
	                                                           float DeltaTime, bool& bTextUpdated, FString& ErrorMessage);

	/**
	 * Checks whether the command can finish after all text is shown.
	 * 全文表示後に、このコマンドを終了できるか判定します。
	 * @param bWaitForInput If true, requires player input before finishing. / true の場合、終了にはプレイヤー入力が必要です。
	 * @return True when input, skip, auto mode, or no-wait settings allow the command to finish. / 入力、スキップ、オートモード、待たない設定により終了できる場合は true です。
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Command|ProcessCommand", meta = (BlueprintProtected))
	bool CanTransitionToFinalize(const bool bWaitForInput) const;

	/**
	 * Reads the full text currently held by a text widget.
	 * テキストウィジェットが現在持っている全文を読み取ります。
	 * @param TextWidget Text widget to read from. / 読み取り元のテキストウィジェットです。
	 * @param TextType Text type to read. / 読み取るテキスト種別です。
	 * @param Text Receives the full text. / 読み取った全文を受け取ります。
	 * @param ErrorMessage Receives the error message when read fails. / 読み取りに失敗した場合のエラーメッセージを受け取ります。
	 * @return True when the full text was read. / 全文を読み取れた場合は true です。
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command|PreviewCommand", meta = (BlueprintProtected, DisplayName = "Get Full Text"))
	UPARAM(DisplayName = "Success") bool TryGetFullText(UShidenTextWidget* TextWidget, const FString& TextType, FString& Text, FString& ErrorMessage);

	/**
	 * Called after the text window open or close animation finishes.
	 * The default implementation clears the animation wait flag.
	 * テキストウィンドウの開閉アニメーションが終わった後に呼ばれます。
	 * デフォルト実装ではアニメーション待ちフラグを解除します。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shiden Visual Novel|Command|Utility", meta = (BlueprintProtected))
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
};
