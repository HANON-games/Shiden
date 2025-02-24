// Copyright (c) 2024 HANON. All Rights Reserved.

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
struct FShidenTextInputProperties
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
struct FShidenAnimation
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
	int32 SelectedOption = -1;

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
	void CaptureWidget(const bool bShowTextBaseLayer, UTexture2D*& ResultTexture, bool& bResult);

	UFUNCTION()
	void TakeScreenshot(const bool bShowTextBaseLayer, UTexture2D*& ResultTexture, bool& bResult);

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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void CaptureScreenToTexture2D(UTexture2D*& ResultTexture, bool& bResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SaveGameWithScreenCapture(const FString& SlotName);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void InitWidget(const TScriptInterface<IShidenManagerInterface>& InShidenManager, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void InitPreviewWidget(const TScriptInterface<IShidenManagerInterface>& InShidenManager, bool& bSuccess, FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaSuspended();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaResumed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaReachedEnd();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaClosed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OnMediaClicked();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade", meta = (OwnerProcessName = "Default"))
	void StartFade(const FString& LayerName, const EEasingFunc::Type Function, const float Duration,
	               const FLinearColor TargetColor, const bool bIsFadeOut, const float BlendExp, const int32 Steps,
	               const FString& OwnerProcessName, const int32 ZOrder, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Fade")
	void IsFadeCompleted(const FString& LayerName, bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void ClearAllFade();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Text")
	void FindTextWidget(const FString& TextWidgetName, UShidenTextWidget*& TextWidget, bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Text")
	void ClearAllTexts();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Image")
	void FindImage(const FString& ImageName, UImage*& Image, bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Image")
	void ClearAllImages();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Image", meta = (OwnerProcessName = "Default"))
	void StartImageFade(const FString& ImageName, UPARAM(ref) UImage* Target, const EEasingFunc::Type Function,
	                    const float Duration, const bool bIsWhiteFade, const bool bShouldBeTransparent,
	                    const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success,
	                    FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Image")
	void IsImageFadeCompleted(const FString& ImageName, bool& bResult) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Option")
	void SetOptions(const TArray<FString>& Options);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Option")
	void GetSelectedOption(int32& Result) const;

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Option")
	bool IsOptionSelected() const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void PlayMedia(const FString& MediaSourcePath, const bool bCanOpenPauseMenu, const int32 MediaZOrder, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Media")
	void IsMediaCompleted(bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void CollapseMedia() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Media")
	void OpenMediaPauseMenu();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	void FindRetainerBox(const FString& RetainerBoxName, URetainerBox*& RetainerBox, bool& bResult) const;

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	static FString MakeMaterialParamsKey(const FString& TargetName, const FString& ParameterName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Material", meta = (OwnerProcessName = "Default"))
	void StartImageMaterialScalarChange(const FString& ImageName, UPARAM(ref) UImage* Target,
	                                    const FName& ParameterName, const EEasingFunc::Type Function,
	                                    const float Duration, const float EndValue, const float BlendExp,
	                                    const int32 Steps, const FString& OwnerProcessName, bool& Success,
	                                    FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	void IsImageMaterialParameterChangeCompleted(const FString& ImageName, const FString& ParameterName, bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Material", meta = (OwnerProcessName = "Default"))
	void StartRetainerBoxMaterialScalarChange(const FString& RetainerBoxName, UPARAM(ref) URetainerBox* Target,
	                                          const FName& ParameterName, const EEasingFunc::Type Function,
	                                          const float Duration, const float EndValue, const float BlendExp,
	                                          const int32 Steps, const FString& OwnerProcessName, bool& Success,
	                                          FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	void IsRetainerBoxMaterialParameterChangeCompleted(const FString& RetainerBoxName, const FString& ParameterName, bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Canvas Panel", meta = (OwnerProcessName = "Default"))
	void StartCanvasPanelMove(const FString& CanvasPanelName, UPARAM(ref) UCanvasPanelSlot* Target,
	                          EEasingFunc::Type Function, float Duration, bool bChangePosition,
	                          FVector2D EndPosition, bool bChangeSize, FVector2D EndSize,
	                          float BlendExp, int32 Steps, const FString& OwnerProcessName, bool& Success,
	                          FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	void IsCanvasPanelMoveCompleted(const FString& CanvasPanelName, bool& bResult) const;

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	void FindCanvasPanelSlot(const FString& CanvasPanelName, UCanvasPanelSlot*& CanvasPanelSlot, bool& bResult) const;

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Animation")
	void FindAnimation(const FString& AnimationName, UUserWidget*& TargetWidget, UWidgetAnimation*& WidgetAnimation, bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Animation")
	void ResetAllAnimations();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeTextInput();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeOptionSelection();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeGameAndUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void InitTextInput(const FShidenTextInputProperties& Properties);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Shiden Visual Novel|Input")
	FText GetTextInput();
	
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static void SanitizeInputText(const FString& Text, const FShidenTextInputProperties& Properties, FString& Result);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Input")
	void IsSkipPressed(bool& bPressed) const;

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	void GetVisibilityByName(const FString& Name, ESlateVisibility& Result) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SetVisibilityByName(const FString& Name, const ESlateVisibility InVisibility,
	                         const bool bShouldRegisterScenarioProperty, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	bool IsMenuOpen() const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void UpdateWidgetCache();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void UpdateAllWidgetAnimations(const FString& Prefix, const UUserWidget* TargetWidget);

	// internal functions
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Widget")
	void GetAllWidgets(TArray<UWidget*>& Children) const;

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget")
	void FindCanvasPanelMoveParams(const FString& CanvasPanelName, FShidenCanvasPanelSlotMoveParams& Value, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget")
	void FindImageFadeParams(const FString& ImageName, FShidenImageFadeParams& Value, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget")
	void FindImageMaterialScalarParams(const FString& MaterialParamsKey, FShidenImageMaterialScalarParams& Value, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget")
	void FindRetainerBoxMaterialScalarParams(const FString& MaterialParamsKey, FShidenRetainerBoxMaterialScalarParams& Value, bool& bSuccess);

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
