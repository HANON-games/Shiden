// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/RichTextBlock.h"
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
#include "Materials/MaterialInstanceDynamic.h"
#include "Framework/Application/SlateApplication.h"
#include "ShidenWidget.generated.h"

class IShidenScenarioManagerInterface;

UCLASS(Abstract, Blueprintable)
class SHIDENCORE_API UShidenWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	bool bIsEndOfMovie = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	int32 SelectedOption = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TMap <FString, FShidenCanvasPanelMoveParams> CanvasPanelMoveParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TMap <FString, FShidenImageFadeParams> ImageFadeParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TMap <FString, FShidenImageMaterialScalarParams> ImageMaterialParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TMap <FString, FShidenRetainerBoxMaterialScalarParams> RetainerBoxMaterialParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TMap <FString, FShidenFadeParams> FadeParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TMap <FString, TObjectPtr<UBorder>> FadeWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IShidenScenarioManagerInterface> ScenarioManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	bool bIsSkipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	bool bCanOpenMoviePauseMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget", meta = (AllowPrivateAccess = "true"))
	bool bPressSkipButton = false;

public:
	UFUNCTION()
	void FadeInOut(float DeltaTime);

	UFUNCTION()
	void ImageFadeInOut(float DeltaTime);

	UFUNCTION()
	void MoveCanvasPanel(float DeltaTime);

	UFUNCTION()
	void ChangeRetainerBoxMaterial(float DeltaTime);

	UFUNCTION()
	void ChangeImageMaterial(float DeltaTime);

	UFUNCTION()
	void NativePreConstruct() override;

	UFUNCTION()
	void NativeConstruct() override;

	UFUNCTION()
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OnMediaSuspended();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OnMediaResumed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OnMediaReachedEnd();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OnMediaClosed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OnMediaClicked();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void StartFade(const FString& LayerName, const EEasingFunc::Type Function, const float Duration, const FLinearColor TargetColor, const bool bIsFadeOut, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, const int32 ZOrder, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Fade")
	void IsFadeCompleted(const FString& LayerName, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void ClearAllFade();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Text")
	void FindTextBlock(const FString TextBlockName, URichTextBlock*& TextBlock, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Text")
	void ClearAllTexts();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Image")
	void FindImage(const FString ImageName, UImage*& Image, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Image")
	void ClearAllImages();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Image")
	void StartImageFade(const FString& ImageName, UPARAM(ref) UImage* Target, const EEasingFunc::Type Function, const float Duration, const bool bIsWhiteFade, const bool bShouldBeTransparent, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Image")
	void IsImageFadeCompleted(const FString& ImageName, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Option")
	void SetOptions(const TArray<FString>& Options);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Option")
	void GetSelectedOption(int32& Result) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void PlayMovie(const FString& MediaSourcePath, const bool CanOpenPauseMenu, const int32 MovieZOrder, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Movie")
	void IsMovieCompleted(bool& bResult) const;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void CollapseMovie();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OpenMoviePauseMenu();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	void FindRetainerBox(const FString RetainerBoxName, URetainerBox*& RetainerBox, bool& bResult);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	FString MakeMaterialParamsKey(const FString& TargetName, const FString& ParameterName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Material")
	void StartImageMaterialScalarChange(const FString& ImageName, UPARAM(ref) UImage* Target, const FName& ParameterName, const EEasingFunc::Type Function, const float Duration, const float EndValue, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	void IsImageMaterialParameterChangeCompleted(const FString& ImageName, const FString& ParameterName, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Material")
	void StartRetainerBoxMaterialScalarChange(const FString& RetainerBoxName, UPARAM(ref) URetainerBox* Target, const FName& ParameterName, const EEasingFunc::Type Function, const float Duration, const float EndValue, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Material")
	void IsRetainerBoxMaterialParameterChangeCompleted(const FString& RetainerBoxName, const FString& ParameterName, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Canvas Panel")
	void StartCanvasPanelMove(const FString& CanvasPanelName, UPARAM(ref) UCanvasPanel* Target, const EEasingFunc::Type Function, const float Duration, const bool bChangePosition, const FVector2D EndPosition, const bool bChangeSize, const FVector2D EndSize, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	void IsCanvasPanelMoveCompleted(const FString& CanvasPanelName, bool& bResult);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	void FindCanvasPanel(const FString CanvalPanelName, UCanvasPanel*& CanvasPanel, bool& bResult);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Animation")
	void FindAnimation(const FString AnimationName, UWidgetAnimation*& WidgetAnimation, bool& bResult);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Animation")
	void ResetAllAnimations();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeSelectOption();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeGameAndUI();

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Input")
	void IsSkipPressed(bool& bPressed) const;

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	void GetVisibilityByName(const FName Name, ESlateVisibility& Result);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SetVisibilityByName(const FName Name, const ESlateVisibility InVisibility, const bool bShouldRegisterScenarioProperty, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|Widget")
	bool IsMenuOpen() const;

	// internal functions
	UFUNCTION()
	void InitWidgetCache();
	
	UFUNCTION()
	void InitAllWidgetAnimations();
		
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Widget")
	void GetAllWidgets(TArray<UWidget*>& Children);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|Widget")
	void InitializeWidget(const TScriptInterface<IShidenScenarioManagerInterface>& ShidenScenarioManager, bool& Success, FString& ErrorMessage);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|Widget")
	void FindCanvasPanelMoveParams(const FString& CanvasPanelName, FShidenCanvasPanelMoveParams& Value, bool& bSuccess);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, URichTextBlock*> TextBlocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, UImage*> Images;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, UWidgetAnimation*> WidgetAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap<FString, URetainerBox*> RetainerBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, UCanvasPanel*> CanvasPanels;
};
