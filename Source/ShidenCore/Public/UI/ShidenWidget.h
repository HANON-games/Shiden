// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Image.h"
#include "Components/RetainerBox.h"
#include "ShidenCanvasPanelSlotMoveParams.h"
#include "ShidenImageFadeParams.h"
#include "ShidenImageMaterialScalarParams.h"
#include "ShidenRetainerBoxMaterialScalarParams.h"
#include "Blueprint/WidgetTree.h"
#include "Animation/WidgetAnimation.h"
#include "MediaPlayer.h"
#include "ShidenFadeParams.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "ShidenTextWidget.h"
#include "ShidenWidget.generated.h"

class IShidenManagerInterface;

/*
 * The mode for capturing the scene.
 * In any mode, the MenuBaseLayer is hidden.
 * UIOnly: Captures only the UI.
 * UIOnlyWithoutTextBaseLayer: Captures only the UI, but hides the TextBaseLayer.
 * FullScene: Captures the UI and the background.
 * FullSceneWithoutTextBaseLayer: Captures the UI and the background, but hides the TextBaseLayer.
 */
UENUM(BlueprintType)
enum class EShidenCaptureScreenMode : uint8
{
	UIOnly = 0,
	UIOnlyWithoutTextBaseLayer,
	FullScene,
	FullSceneWithoutTextBaseLayer
};

USTRUCT(Blueprintable)
struct SHIDENCORE_API FShidenTextInputProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 MaxLength = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString AllowedCharacterRegex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString DefaultText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	FString HintText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	int32 MaxLines = 1;
};

USTRUCT(Blueprintable)
struct SHIDENCORE_API FShidenAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TObjectPtr<UUserWidget> TargetWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TObjectPtr<UWidgetAnimation> Animation;
};

UCLASS(Abstract, Blueprintable)
class SHIDENCORE_API UShidenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	bool bIsEndOfMedia = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenCaptureScreenMode CaptureScreenMode = EShidenCaptureScreenMode::UIOnlyWithoutTextBaseLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float CaptureScale = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FShidenTextInputProperties TextInputProperties;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	int32 SelectedOption = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	bool bIsTextSubmitted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FShidenCanvasPanelSlotMoveParams> CanvasPanelMoveParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FShidenImageFadeParams> ImageFadeParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FShidenImageMaterialScalarParams> ImageMaterialParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FShidenRetainerBoxMaterialScalarParams> RetainerBoxMaterialParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FShidenFadeParams> FadeParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UBorder>> FadeWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IShidenManagerInterface> ShidenManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	bool bIsSkipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	bool bCanOpenMediaPauseMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", meta = (AllowPrivateAccess = "true"))
	bool bPressSkipButton = false;

	UFUNCTION()
	void CaptureWidget(bool bShowTextBaseLayer, UTexture2D*& ResultTexture);

	UFUNCTION()
	void TakeScreenshot(bool bShowTextBaseLayer, UTexture2D*& ResultTexture);

	UFUNCTION()
	void UpdateWidgetCacheCore(const FString& Prefix, const UWidgetTree* Tree);

public:
	UFUNCTION()
	void FadeInOut(float DeltaTime);

	UFUNCTION()
	void ImageFadeInOut(float DeltaTime);

	UFUNCTION()
	void MoveCanvasPanelSlot(float DeltaTime);

	UFUNCTION()
	void ChangeRetainerBoxMaterial(float DeltaTime);

	UFUNCTION()
	void ChangeImageMaterial(float DeltaTime);

	UFUNCTION()
	virtual void NativePreConstruct() override;

	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	/**
	 * Saves the current game state to the specified slot with a thumbnail image.
	 * @param SlotName The save slot name
	 * @param Thumbnail The thumbnail image to associate with this save
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SaveGame(const FString& SlotName, const UTexture2D* Thumbnail);

	/**
	 * Saves the current game state with an automatically captured screen thumbnail.
	 * @param SlotName The save slot name
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SaveGameWithScreenCapture(const FString& SlotName);

	/**
	 * Captures the current screen content to a 2D texture.
	 * @param ResultTexture [out] The captured screen texture
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void CaptureScreenToTexture2D(UTexture2D*& ResultTexture);

	/**
	 * Loads the widget state for the save slot metadata.
	 * @return A map of metadata key-value pairs for the current save slot
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (ReturnDisplayName = "Map"))
	TMap<FString, FString> GetSaveSlotMetadata();

	/**
	 * Initializes the Shiden widget with the manager interface for normal gameplay.
	 * @param InShidenManager The Shiden manager interface
	 * @param bSuccess [out] Whether the widget was successfully initialized
	 * @param ErrorMessage [out] Error details if initialization fails
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void InitWidget(const TScriptInterface<IShidenManagerInterface>& InShidenManager, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Initializes the Shiden widget for editor preview.
	 * @param InShidenManager The Shiden manager interface
	 * @param bSuccess [out] Whether the preview widget was successfully initialized
	 * @param ErrorMessage [out] Error details if initialization fails
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void InitPreviewWidget(const TScriptInterface<IShidenManagerInterface>& InShidenManager, bool& bSuccess, FString& ErrorMessage);

	/**
	 * Event called when media playback is suspended.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaSuspended();

	/**
	 * Event called when media playback is resumed from a suspended state.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaResumed();

	/**
	 * Event called when media playback reaches the end of the content.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaReachedEnd();

	/**
	 * Event called when media playback is closed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaClosed();

	/**
	 * Event called when the user clicks on the media during playback.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaClicked();

	/**
	 * Opens the pause menu during media playback.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OpenMediaPauseMenu();

	/**
	 * Sets the available options.
	 * @param Options Array of option text strings to display
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Option")
	void SetOptions(const TArray<FString>& Options);

	/**
	 * Sets the input mode to text input for receiving typed text from the player.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeTextInput();

	/**
	 * Sets the input mode for option command.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeOptionSelection();

	/**
	 * Sets the input mode to game and UI for normal visual novel interaction.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeGameAndUI();

	/**
	 * Initializes text input with the specified properties and constraints.
	 * @param Properties The configuration properties for text input behavior
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Text Input")
	void InitTextInput(const FShidenTextInputProperties& Properties);

	/**
	 * Gets the current text input from the player.
	 * @return The text entered by the player
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Shiden Visual Novel|Text Input")
	FText GetTextInput();

	/**
	 * Starts a fade animation on the specified layer with customizable easing and timing.
	 * @param LayerName The name of the layer to apply the fade effect to
	 * @param Function The easing function type for the fade animation
	 * @param Duration The duration of the fade animation in seconds
	 * @param TargetColor The target color to fade to
	 * @param bIsFadeOut True for fade out, false for fade in
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param Steps The number of animation steps for precision
	 * @param OwnerProcessName The name of the process that owns this fade operation
	 * @param ZOrder The rendering order for this fade layer
	 * @param ErrorMessage [out] Error details if the operation fails
	 * @return True if the fade animation was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade", meta = (OwnerProcessName = "Default", DisplayName = "Start Fade"))
	UPARAM(DisplayName = "Success") bool TryStartFade(const FString& LayerName, const EEasingFunc::Type Function, const float Duration,
	                                                  const FLinearColor TargetColor, const bool bIsFadeOut, const float BlendExp, const int32 Steps,
	                                                  const FString& OwnerProcessName, const int32 ZOrder, FString& ErrorMessage);

	/**
	 * Checks if the fade animation on the specified layer has completed.
	 * @param LayerName The name of the layer to check
	 * @return True if the fade animation has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Fade")
	bool IsFadeCompleted(const FString& LayerName) const;

	/**
	 * Clears all active fade animations and resets fade states.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void ClearAllFade();

	/**
	 * Finds a text widget by name within the Shiden widget.
	 * @param TextWidgetName The name of the text widget to find
	 * @param TextWidget [out] The found text widget
	 * @return True if the text widget was successfully found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Text", meta = (DisplayName = "Find Text Widget"))
	UPARAM(DisplayName = "Success") bool TryFindTextWidget(const FString& TextWidgetName, UShidenTextWidget*& TextWidget) const;

	/**
	 * Clears all text content from all text widgets.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Text")
	void ClearAllTexts();

	/**
	 * Finds an image widget by name within the Shiden widget.
	 * @param ImageName The name of the image widget to find
	 * @param Image [out] The found image widget
	 * @return True if the image widget was successfully found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Image", meta = (DisplayName = "Find Image"))
	UPARAM(DisplayName = "Success") bool TryFindImage(const FString& ImageName, UImage*& Image) const;

	/**
	 * Clears all image content from all image widgets.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Image")
	void ClearAllImages();

	/**
	 * Starts a fade animation on the specified image with customizable parameters.
	 * @param ImageName The name of the image to apply the fade effect to
	 * @param Target The image widget to animate
	 * @param Function The easing function type for the fade animation
	 * @param Duration The duration of the fade animation in seconds
	 * @param bIsWhiteFade True for white fade, false for black fade
	 * @param bShouldBeTransparent True if the image should become transparent during fade
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param Steps The number of animation steps for precision
	 * @param OwnerProcessName The name of the process that owns this fade operation
	 * @param bClearImageOnCompleted True if the image should be cleared when fade completes
	 * @param ErrorMessage [out] Error details if the operation fails
	 * @return True if the image fade animation was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Image", meta = (OwnerProcessName = "Default", DisplayName = "Start Image Fade"))
	UPARAM(DisplayName = "Success") bool TryStartImageFade(const FString& ImageName, UPARAM(ref) UImage* Target,
	                                                       EEasingFunc::Type Function, float Duration, bool bIsWhiteFade, bool bShouldBeTransparent,
	                                                       float BlendExp, int32 Steps, const FString& OwnerProcessName, bool bClearImageOnCompleted,
	                                                       FString& ErrorMessage);

	/**
	 * Checks if the fade animation on the specified image has completed.
	 * @param ImageName The name of the image to check
	 * @return True if the image fade animation has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Image")
	bool IsImageFadeCompleted(const FString& ImageName) const;

	/**
	 * Gets the index of the currently selected option.
	 * @return The index of the selected option, or -1 if none selected
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Option")
	int32 GetSelectedOption() const noexcept;

	/**
	 * Checks if an option has been selected by the player.
	 * @return True if an option has been selected
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Option")
	bool IsOptionSelected() const noexcept;

	/**
	 * Attempts to play media content from the specified source path.
	 * @param MediaSourcePath The file path to the media content to play
	 * @param bCanOpenPauseMenu True if the pause menu can be opened during media playback
	 * @param MediaZOrder The rendering order for the media layer
	 * @return True if media playback was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Media", meta = (DisplayName = "Play Media"))
	UPARAM(DisplayName = "Success") bool TryPlayMedia(const FString& MediaSourcePath, const bool bCanOpenPauseMenu, const int32 MediaZOrder);

	/**
	 * Checks if the currently playing media has completed playback.
	 * @return True if media playback has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Media")
	bool IsMediaCompleted() const noexcept;

	/**
	 * Collapses the media player interface.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void CollapseMedia() const;

	/**
	 * Finds a retainer box widget by name within the Shiden widget.
	 * @param RetainerBoxName The name of the retainer box widget to find
	 * @param RetainerBox [out] The found retainer box widget
	 * @return True if the retainer box widget was successfully found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material", meta = (DisplayName = "Find Retainer Box"))
	UPARAM(DisplayName = "Success") bool TryFindRetainerBox(const FString& RetainerBoxName, URetainerBox*& RetainerBox) const;

	/**
	 * Creates a unique key string for material parameter tracking.
	 * @param TargetName The name of the target widget or object
	 * @param ParameterName The name of the material parameter
	 * @return A unique key string combining target and parameter names
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	static FString MakeMaterialParamsKey(const FString& TargetName, const FString& ParameterName);

	/**
	 * Starts the material scalar parameter change on an image's material.
	 * @param ImageName The name of the image to animate
	 * @param Target The image widget to animate
	 * @param ParameterName The name of the scalar parameter to animate
	 * @param Function The easing function type for the animation
	 * @param Duration The duration of the animation in seconds
	 * @param EndValue The target value for the scalar parameter
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param Steps The number of animation steps for precision
	 * @param OwnerProcessName The name of the process that owns this animation
	 * @param ErrorMessage [out] Error details if the operation fails
	 * @return True if the material parameter change was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Material", meta = (OwnerProcessName = "Default", DisplayName = "Start Image Material Scalar Change"))
	UPARAM(DisplayName = "Success") bool TryStartImageMaterialScalarChange(const FString& ImageName, UPARAM(ref) UImage* Target,
	                                                                       const FName& ParameterName, const EEasingFunc::Type Function,
	                                                                       const float Duration, const float EndValue, const float BlendExp,
	                                                                       const int32 Steps, const FString& OwnerProcessName, FString& ErrorMessage);

	/**
	 * Checks if the material parameter change on the specified image has completed.
	 * @param ImageName The name of the image to check
	 * @param ParameterName The name of the material parameter being animated
	 * @return True if the material parameter change has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	bool IsImageMaterialParameterChangeCompleted(const FString& ImageName, const FString& ParameterName) const;

	/**
	 * Starts an material scalar parameter change on a retainer box's material.
	 * @param RetainerBoxName The name of the retainer box to animate
	 * @param Target The retainer box widget to animate
	 * @param ParameterName The name of the scalar parameter to animate
	 * @param Function The easing function type for the animation
	 * @param Duration The duration of the animation in seconds
	 * @param EndValue The target value for the scalar parameter
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param Steps The number of animation steps for precision
	 * @param OwnerProcessName The name of the process that owns this animation
	 * @param ErrorMessage [out] Error details if the operation fails
	 * @return True if the material parameter change was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Material", meta = (OwnerProcessName = "Default", DisplayName = "Start Retainer Box Material Scalar Change"))
	UPARAM(DisplayName = "Success") bool TryStartRetainerBoxMaterialScalarChange(const FString& RetainerBoxName, UPARAM(ref) URetainerBox* Target,
	                                                                             const FName& ParameterName, const EEasingFunc::Type Function,
	                                                                             const float Duration, const float EndValue, const float BlendExp,
	                                                                             const int32 Steps, const FString& OwnerProcessName, FString& ErrorMessage);

	/**
	 * Checks if the material parameter change on the specified retainer box has completed.
	 * @param RetainerBoxName The name of the retainer box to check
	 * @param ParameterName The name of the material parameter being animated
	 * @return True if the material parameter change has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	bool IsRetainerBoxMaterialParameterChangeCompleted(const FString& RetainerBoxName, const FString& ParameterName) const;

	/**
	 * Starts an animated movement and/or size change on a canvas panel slot.
	 * @param CanvasPanelSlotName The name of the canvas panel slot to animate
	 * @param Target The canvas panel slot to animate
	 * @param Function The easing function type for the animation
	 * @param Duration The duration of the animation in seconds
	 * @param bChangePosition True if position should be animated
	 * @param EndPosition The target position if position animation is enabled
	 * @param bChangeSize True if size should be animated
	 * @param EndSize The target size if size animation is enabled
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param Steps The number of animation steps for precision
	 * @param OwnerProcessName The name of the process that owns this animation
	 * @param ErrorMessage [out] Error details if the operation fails
	 * @return True if the canvas panel slot animation was successfully started
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Canvas Panel", meta = (OwnerProcessName = "Default", DisplayName = "Start Canvas Panel Slot Move"))
	UPARAM(DisplayName = "Success") bool TryStartCanvasPanelSlotMove(const FString& CanvasPanelSlotName, UPARAM(ref) UCanvasPanelSlot* Target,
	                                                                 EEasingFunc::Type Function, float Duration, bool bChangePosition,
	                                                                 FVector2D EndPosition, bool bChangeSize, FVector2D EndSize,
	                                                                 float BlendExp, int32 Steps, const FString& OwnerProcessName, FString& ErrorMessage);

	/**
	 * Checks if the canvas panel slot movement animation has completed.
	 * @param CanvasPanelName The name of the canvas panel slot to check
	 * @return True if the canvas panel slot animation has completed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	bool IsCanvasPanelSlotMoveCompleted(const FString& CanvasPanelName) const;

	/**
	 * Finds a canvas panel slot by name within the Shiden widget.
	 * @param CanvasPanelSlotName The name of the canvas panel slot to find
	 * @param CanvasPanelSlot [out] The found canvas panel slot
	 * @return True if the canvas panel slot was successfully found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel", meta = (DisplayName = "Find Canvas Panel Slot"))
	UPARAM(DisplayName = "Success") bool TryFindCanvasPanelSlot(const FString& CanvasPanelSlotName, UCanvasPanelSlot*& CanvasPanelSlot) const;

	/**
	 * Finds a widget animation by name within the Shiden widget.
	 * @param AnimationName The name of the animation to find
	 * @param TargetWidget [out] The widget that owns the animation
	 * @param WidgetAnimation [out] The found widget animation
	 * @return True if the animation was successfully found
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Animation", meta = (DisplayName = "Find Animation"))
	UPARAM(DisplayName = "Success") bool TryFindAnimation(const FString& AnimationName, UUserWidget*& TargetWidget, UWidgetAnimation*& WidgetAnimation) const;

	/**
	 * Resets all widget animations to their initial state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Animation")
	void ResetAllAnimations();

	/**
	 * Checks if text input has been submitted by the player.
	 * @return True if text has been submitted
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Text Input")
	bool IsTextSubmitted() const noexcept;

	/**
	 * Sanitizes input text according to the specified properties and constraints.
	 * @param Text The raw input text to sanitize
	 * @param Properties The text input properties defining constraints
	 * @param Result [out] The sanitized text
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void SanitizeInputText(const FString& Text, const FShidenTextInputProperties& Properties, FString& Result);

	/**
	 * Checks if the skip button is currently being pressed.
	 * @return True if the skip button is pressed
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Input")
	bool IsSkipPressed() const noexcept;

	/**
	 * Gets the visibility state of a widget by name.
	 * @param Name The name of the widget to check
	 * @param Result [out] The widget's visibility state
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	void GetVisibilityByName(const FString& Name, ESlateVisibility& Result) const;

	/**
	 * Sets the visibility state of a widget by name.
	 * @param Name The name of the widget to modify
	 * @param NewVisibility The new visibility state to set
	 * @param bShouldRegisterScenarioProperty True if this change should be saved to scenario properties
	 * @return True if the widget visibility was successfully set
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (DisplayName = "Set Visibility by Name"))
	UPARAM(DisplayName = "Success") bool TrySetVisibilityByName(const FString& Name, const ESlateVisibility NewVisibility,
	                                                            const bool bShouldRegisterScenarioProperty);

	/**
	 * Checks if any menu is currently open.
	 * @return True if a menu is open
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	bool IsMenuOpen() const;

	/**
	 * Updates the internal widget cache with current widget references.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void UpdateWidgetCache();

	/**
	 * Updates all widget animations for the specified target widget.
	 * @param Prefix The prefix to use when caching animation names
	 * @param TargetWidget The widget containing animations to update
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void UpdateAllWidgetAnimations(const FString& Prefix, const UUserWidget* TargetWidget);

	/**
	 * Checks if any widget in the text layer is currently visible.
	 * @return True if any widget in the text layer is visible
	 */
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	bool IsTextVisible() const;

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Widget")
	void GetAllWidgets(TArray<UWidget*>& Children) const;

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget", meta = (DisplayName = "Find Canvas Panel Move Params"))
	UPARAM(DisplayName = "Success") bool TryFindCanvasPanelMoveParams(const FString& CanvasPanelName, FShidenCanvasPanelSlotMoveParams& Value);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget", meta = (DisplayName = "Find Image Fade Params"))
	UPARAM(DisplayName = "Success") bool TryFindImageFadeParams(const FString& ImageName, FShidenImageFadeParams& Value);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget", meta = (DisplayName = "Find Image Material Scalar Params"))
	UPARAM(DisplayName = "Success") bool TryFindImageMaterialScalarParams(const FString& MaterialParamsKey, FShidenImageMaterialScalarParams& Value);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget", meta = (DisplayName = "Find Retainer Box Material Scalar Params"))
	UPARAM(DisplayName = "Success") bool TryFindRetainerBoxMaterialScalarParams(const FString& MaterialParamsKey, FShidenRetainerBoxMaterialScalarParams& Value);

	// Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> BaseLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> ImageLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> TextBaseLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> TextLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> OptionLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> MediaLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> MenuBaseLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> BacklogLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> MediaPauseMenuLayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shiden Visual Novel|Widget", meta = (BindWidget))
	TObjectPtr<UPanelWidget> TextInputLayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, TObjectPtr<UShidenTextWidget>> TextWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, TObjectPtr<UImage>> Images;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, FShidenAnimation> WidgetAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, TObjectPtr<URetainerBox>> RetainerBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, TObjectPtr<UCanvasPanelSlot>> CanvasPanelSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, TObjectPtr<UWidget>> AllWidgets;
};
