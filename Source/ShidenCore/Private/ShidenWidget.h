// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/RichTextBlock.h"
#include "Components/Image.h"
#include "Components/RetainerBox.h"
#include "ShidenWindowMode.h"
#include "ShidenCanvasPanelSlotMoveParams.h"
#include "ShidenImageFadeParams.h"
#include "ShidenImageMaterialScalarParams.h"
#include "ShidenRetainerBoxMaterialScalarParams.h"
#include "ShidenWidget.generated.h"

class IShidenScenarioManagerInterface;

UCLASS(Abstract, Blueprintable)
class SHIDENCORE_API UShidenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void StartFade(const FString& LayerName, const EEasingFunc::Type Function, const float Duration, const FLinearColor TargetColor, const bool bIsFadeOut, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, const int32 ZOrder, bool& Success, FString& ErrorMessage);

	virtual void StartFade_Implementation(const FString LayerName, const EEasingFunc::Type Function, const float Duration, const FLinearColor TargetColor, const bool bIsFadeOut, const float BlendExp, const int32 Steps, const FString OwnerProcessName, const int32 ZOrder, bool& Success, FString& ErrorMessage) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Fade")
	void IsFadeCompleted(const FString& LayerName, bool& bResult);

	virtual void IsFadeCompleted_Implementation(const FString LayerName, bool& bResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void ClearAllFade();

	virtual void ClearAllFade_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Fade")
	void FadeInOut(const float DeltaTime);

	virtual void FadeInOut_Implementation(const float DeltaTime) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Text")
	void GetTexts(TMap <FString, URichTextBlock*>& TextObjects);

	virtual void GetTexts_Implementation(TMap <FString, URichTextBlock*>& TextObjects) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Image")
	void GetImages(TMap <FString, UImage*>& ImageObjects);

	virtual void GetImages_Implementation(TMap <FString, UImage*>& ImageObjects) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Image")
	void StartImageFade(const FString& ImageName, const UImage* Target, const EEasingFunc::Type Function, const float Duration, const bool bIsWhiteFade, const bool bShouldBeTransparent, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	virtual void StartImageFade_Implementation(const FString ImageName, const UImage* Target, const EEasingFunc::Type Function, const float Duration, const bool bIsWhiteFade, const bool bShouldBeTransparent, const float BlendExp, const int32 Steps, const FString OwnerProcessName, bool& Success, FString& ErrorMessage) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Image")
	void IsImageFadeCompleted(const FString& ImageSlot, bool& bResult);

	virtual void IsImageFadeCompleted_Implementation(const FString ImageSlot, bool& bResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Option")
	void SetOptions(const TArray<FString>& Options);

	virtual void SetOptions_Implementation(const TArray<FString>& Options) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Option")
	void GetSelectedOption(int32& Result);

	virtual void GetSelectedOption_Implementation(int32& Result) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void PlayMovie(const FString& MediaSourcePath, const bool CanOpenPauseMenu, const int32 MovieZOrder, const int32 PauseMenuZOrder, bool& bSuccess);

	virtual void PlayMovie_Implementation(const FString MediaSourcePath, const bool CanOpenPauseMenu, const int32 MovieZOrder, const int32 PauseMenuZOrder, bool& bSuccess) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Movie")
	void IsMovieCompleted(bool& bResult);

	virtual void IsMovieCompleted_Implementation(bool& bResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void CollapseMovie();

	virtual void CollapseMovie_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Movie")
	void OpenMoviePauseMenu();

	virtual void OpenMoviePauseMenu_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Material")
	void GetRetainerBoxes(TMap<FString, URetainerBox*>& RetainerBoxes);

	virtual void GetRetainerBoxes_Implementation(TMap<FString, URetainerBox*>& RetainerBoxes) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Material")
	void MakeMaterialParamsKey(const FString& TargetName, const FString& ParameterName, FString& Key);

	virtual void MakeMaterialParamsKey_Implementation(const FString TargetName, const FString ParameterName, FString& Key) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Material")
	void StartImageMaterialScalarChange(const FString& ImageName, const UImage* Target, const FName& ParameterName, const EEasingFunc::Type Function, const float Duration, const float EndValue, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	virtual void StartImageMaterialScalarChange_Implementation(const FString ImageName, const UImage* Target, const FName ParameterName, const EEasingFunc::Type Function, const float Duration, const float EndValue, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Material")
	void IsImageMaterialParameterChangeCompleted(const FString& ImageName, const FString& ParameterName, bool& bResult);

	virtual void IsImageMaterialParameterChangeCompleted_Implementation(const FString ImageName, const FString ParameterName, bool& bResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Material")
	void StartRetainerBoxMaterialScalarChange(const FString& RetainerBoxName, const URetainerBox* Target, const FName& ParameterName, const EEasingFunc::Type Function, const float Duration, const float EndValue, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	virtual void StartRetainerBoxMaterialScalarChange_Implementation(const FString RetainerBoxName, const URetainerBox* Target, const FName ParameterName, const EEasingFunc::Type Function, const float Duration, const float EndValue, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Material")
	void IsRetainerBoxMaterialParameterChangeCompleted(const FString& RetainerBoxName, const FString& ParameterName, bool& bResult);

	virtual void IsRetainerBoxMaterialParameterChangeCompleted_Implementation(const FString RetainerBoxName, const FString ParameterName, bool& bResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Canvas Panel")
	void StartCanvasPanelMove(const FString& PanelName, const UWidget* Target, const EEasingFunc::Type Function, const float Duration, const bool bChangePosition, const FVector2D EndPosition, const bool bChangeSize, const FVector2D EndSize, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage);

	virtual void StartCanvasPanelMove_Implementation(const FString PanelName, const UWidget* Target, const EEasingFunc::Type Function, const float Duration, const bool bChangePosition, const FVector2D EndPosition, const bool bChangeSize, const FVector2D EndSize, const float BlendExp, const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	void IsPanelMoveCompleted(const FString& PanelName, bool& bResult);

	virtual void IsPanelMoveCompleted_Implementation(const FString PanelName, bool& bResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Canvas Panel")
	void GetCanvasPanelSlots(TMap <FString, UWidget*>& CanvasPanelSlots);

	virtual void GetCanvasPanelSlots_Implementation(TMap <FString, UWidget*>& CanvasPanelSlots) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Animation")
	void GetAnimations(TMap <FString, UWidgetAnimation*>& WidgetAnimations);

	virtual void GetAnimations_Implementation(TMap <FString, UWidgetAnimation*>& WidgetAnimations) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Animation")
	void ResetAllAnimations();

	virtual void ResetAllAnimations_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeSelectOption();

	virtual void SetInputModeSelectOption_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Input")
	void SetInputModeGameAndUI();

	virtual void SetInputModeGameAndUI_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Window")
	void IsSkipPressed(bool& bPressed);

	virtual void IsSkipPressed_Implementation(bool& bPressed) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Window")
	void GetWindowMode(EShidenWindowMode& WindowMode);

	virtual void GetWindowMode_Implementation(EShidenWindowMode& WindowMode) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Window")
	void TogglePreviewMode();

	virtual void TogglePreviewMode_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Window")
	void InitializeWidget(const TScriptInterface<IShidenScenarioManagerInterface>& ShidenScenarioManager, bool& Success, FString& ErrorMessage);

	virtual void InitializeWidget_Implementation(const TScriptInterface<IShidenScenarioManagerInterface>& ShidenScenarioManager, bool& Success, FString& ErrorMessage) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Window")
	void ChangeWindowMode(EShidenWindowMode WindowMode, bool ShouldRegisterCurrentProperty);

	virtual void ChangeWindowMode_Implementation(EShidenWindowMode WindowMode, bool ShouldRegisterCurrentProperty) {}

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, FShidenCanvasPanelSlotMoveParams> CanvasPanelSlotMoveParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, FShidenImageFadeParams> ImageFadeParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, FShidenImageMaterialScalarParams> ImageMaterialScalarParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Widget")
	TMap <FString, FShidenRetainerBoxMaterialScalarParams> RetainerBoxMaterialScalarParams;
};
