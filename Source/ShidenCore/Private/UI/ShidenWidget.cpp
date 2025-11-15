// Copyright (c) 2025 HANON. All Rights Reserved.

#include "UI/ShidenWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Runtime/MediaAssets/Public/MediaSource.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/WidgetRenderer.h"
#include "TextureResource.h"
#include "Runtime/RenderCore/Public/RenderingThread.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Texture2D.h"

TArray<FColor> ResizeBitmap(const TArray<FColor>& OriginalBitmap, const int32 OriginalWidth, const int32 OriginalHeight, const int32 TargetWidth,
                            const int32 TargetHeight)
{
	TArray<FColor> ResizedBitmap;

	// Prevent division by zero
	if (TargetWidth <= 0 || TargetHeight <= 0)
	{
		return ResizedBitmap;
	}

	ResizedBitmap.SetNumZeroed(TargetWidth * TargetHeight);

	const float XRatio = static_cast<float>(OriginalWidth) / TargetWidth;
	const float YRatio = static_cast<float>(OriginalHeight) / TargetHeight;

	for (int32 y = 0; y < TargetHeight; ++y)
	{
		const int32 SrcY = FMath::Clamp(FMath::FloorToInt(y * YRatio), 0, OriginalHeight - 1);
		const int32 SrcYOffset = SrcY * OriginalWidth;
		const int32 DestOffset = y * TargetWidth;

		for (int32 x = 0; x < TargetWidth; ++x)
		{
			const int32 SrcX = FMath::Clamp(FMath::FloorToInt(x * XRatio), 0, OriginalWidth - 1);
			ResizedBitmap[DestOffset + x] = OriginalBitmap[SrcYOffset + SrcX];
		}
	}

	return ResizedBitmap;
}

bool CreateTexture2DFromBitmap(TArray<FColor>& Bitmap, const int32 Width, const int32 Height, UTexture2D*& OutTexture)
{
	// Set alpha to 255 for all pixels
	for (FColor& Color : Bitmap)
	{
		Color.A = 255;
	}

	// Create transient texture
	OutTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
	if (!OutTexture)
	{
		return false;
	}

	// Get platform data and validate
	FTexturePlatformData* PlatformData = OutTexture->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		return false;
	}

	// Configure texture properties
	OutTexture->SRGB = true;
	OutTexture->AddToRoot();

	// Copy bitmap data to texture
	FTexture2DMipMap& Mip = PlatformData->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, Bitmap.GetData(), Bitmap.Num() * sizeof(FColor));
	Mip.BulkData.Unlock();

	// Update texture resource
	OutTexture->UpdateResource();

	return true;
}

SHIDENCORE_API void UShidenWidget::CaptureWidget(const bool bShowTextBaseLayer, UTexture2D*& ResultTexture)
{
	const TSharedRef<SWidget> WidgetRef = TakeWidget();
	FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(true, false);

	const FVector2D OutSize = FVector2D(WidgetRef->GetDesiredSize().X, WidgetRef->GetDesiredSize().Y);
	constexpr TextureFilter Filter = TF_Bilinear;

	if (const TObjectPtr<UTextureRenderTarget2D> RenderTarget = FWidgetRenderer::CreateTargetFor(OutSize, Filter, true))
	{
		RenderTarget->bForceLinearGamma = true;
		RenderTarget->UpdateResourceImmediate();

		const ESlateVisibility TextBaseLayerVisibilityBeforeCapture = TextBaseLayer->GetVisibility();
		const ESlateVisibility MenuBaseLayerVisibilityBeforeCapture = MenuBaseLayer->GetVisibility();

		MenuBaseLayer->SetVisibility(ESlateVisibility::Hidden);
		if (!bShowTextBaseLayer)
		{
			TextBaseLayer->SetVisibility(ESlateVisibility::Hidden);
		}

		WidgetRenderer->DrawWidget(RenderTarget, WidgetRef, 1, OutSize, 0);

		FlushRenderingCommands();

		BeginCleanup(WidgetRenderer);

		MenuBaseLayer->SetVisibility(MenuBaseLayerVisibilityBeforeCapture);
		if (!bShowTextBaseLayer)
		{
			TextBaseLayer->SetVisibility(TextBaseLayerVisibilityBeforeCapture);
		}

		if (TArray<FColor> Bitmap; RenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(Bitmap, FReadSurfaceDataFlags()))
		{
			Bitmap = ResizeBitmap(Bitmap, OutSize.X, OutSize.Y, OutSize.X * CaptureScale, OutSize.Y * CaptureScale);
			CreateTexture2DFromBitmap(Bitmap, OutSize.X * CaptureScale, OutSize.Y * CaptureScale, ResultTexture);
		}
	}
	else
	{
		// In case of failure, clean up the widget renderer
		BeginCleanup(WidgetRenderer);
	}
}

SHIDENCORE_API void UShidenWidget::TakeScreenshot(const bool bShowTextBaseLayer, UTexture2D*& ResultTexture)
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	TArray<FColor> Bitmap;
	FIntVector OutSize;

	const ESlateVisibility TextBaseLayerVisibilityBeforeCapture = TextBaseLayer->GetVisibility();
	const ESlateVisibility MenuBaseLayerVisibilityBeforeCapture = MenuBaseLayer->GetVisibility();

	MenuBaseLayer->SetVisibility(ESlateVisibility::Hidden);
	if (!bShowTextBaseLayer)
	{
		TextBaseLayer->SetVisibility(ESlateVisibility::Hidden);
	}

	FlushRenderingCommands();

	const bool bScreenshotSuccessful = FSlateApplication::Get().TakeScreenshot(TakeWidget(), Bitmap, OutSize);

	MenuBaseLayer->SetVisibility(MenuBaseLayerVisibilityBeforeCapture);
	if (!bShowTextBaseLayer)
	{
		TextBaseLayer->SetVisibility(TextBaseLayerVisibilityBeforeCapture);
	}

	if (bScreenshotSuccessful)
	{
		Bitmap = ResizeBitmap(Bitmap, OutSize.X, OutSize.Y, OutSize.X * CaptureScale, OutSize.Y * CaptureScale);
		CreateTexture2DFromBitmap(Bitmap, OutSize.X * CaptureScale, OutSize.Y * CaptureScale, ResultTexture);
	}
}

SHIDENCORE_API void UShidenWidget::CaptureScreenToTexture2D(UTexture2D*& ResultTexture)
{
	switch (CaptureScreenMode)
	{
	case EShidenCaptureScreenMode::UIOnly:
		CaptureWidget(true, ResultTexture);
		break;
	case EShidenCaptureScreenMode::UIOnlyWithoutTextBaseLayer:
		CaptureWidget(false, ResultTexture);
		break;
	case EShidenCaptureScreenMode::FullScene:
		TakeScreenshot(true, ResultTexture);
		break;
	case EShidenCaptureScreenMode::FullSceneWithoutTextBaseLayer:
		TakeScreenshot(false, ResultTexture);
		break;
	}
}

SHIDENCORE_API void UShidenWidget::NativeTick(const FGeometry& MyGeometry, const float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	const bool bIsSkippedResult = bIsSkipped && UShidenScenarioBlueprintLibrary::CanSkipCommand();

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const float DeltaTimeToUse = bIsSkippedResult
		                             ? DeltaTime * ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate
		                             : DeltaTime;

	FadeInOut(DeltaTimeToUse);
	ImageFadeInOut(DeltaTimeToUse);
	MoveCanvasPanelSlot(DeltaTimeToUse);
	ChangeImageMaterial(DeltaTimeToUse);
	ChangeRetainerBoxMaterial(DeltaTimeToUse);
}

SHIDENCORE_API void UShidenWidget::FadeInOut(const float DeltaTime)
{
	TArray<FString> KeysToRemove;
	for (TPair<FString, FShidenFadeParams>& Pair : FadeParams)
	{
		FShidenFadeParams& Param = Pair.Value;
		Param.EasingAlpha += DeltaTime / Param.FadeDuration;
		const float Alpha = FMath::Clamp(Param.EasingAlpha, 0.0f, 1.0f);
		const float ResultR = UKismetMathLibrary::Ease(Param.StartColor.R, Param.TargetColor.R, Alpha, Param.FadeFunction, Param.BlendExp,
		                                               Param.Steps);
		const float ResultG = UKismetMathLibrary::Ease(Param.StartColor.G, Param.TargetColor.G, Alpha, Param.FadeFunction, Param.BlendExp,
		                                               Param.Steps);
		const float ResultB = UKismetMathLibrary::Ease(Param.StartColor.B, Param.TargetColor.B, Alpha, Param.FadeFunction, Param.BlendExp,
		                                               Param.Steps);
		const float ResultA = UKismetMathLibrary::Ease(Param.StartColor.A, Param.TargetColor.A, Alpha, Param.FadeFunction, Param.BlendExp,
		                                               Param.Steps);

		const TObjectPtr<UBorder> FadeWidget = FadeWidgets.FindRef(Pair.Key);
		if (FadeWidget)
		{
			FadeWidget->SetBrushColor(FLinearColor(ResultR, ResultG, ResultB, ResultA));
		}

		if (Alpha >= 1.0f)
		{
			KeysToRemove.Add(Pair.Key);
			if (FMath::IsNearlyZero(ResultA) && FadeWidget)
			{
				FadeWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		FadeParams.Remove(Key);
	}
}

SHIDENCORE_API void UShidenWidget::ImageFadeInOut(const float DeltaTime)
{
	TArray<FString> KeysToRemove;
	for (TPair<FString, FShidenImageFadeParams>& Pair : ImageFadeParams)
	{
		FShidenImageFadeParams& Param = Pair.Value;
		Param.EasingAlpha += DeltaTime / Param.FadeDuration;
		const float Alpha = FMath::Clamp(Param.EasingAlpha, 0.0f, 1.0f);

		FLinearColor NewColor = Param.bIsWhiteFade ? FLinearColor::White : FLinearColor::Black;
		NewColor.A = UKismetMathLibrary::Ease(Param.bShouldBeTransparent ? 1 : 0, Param.bShouldBeTransparent ? 0 : 1, Alpha, Param.FadeFunction,
		                                      Param.BlendExp, Param.Steps);

		Param.Target->SetColorAndOpacity(NewColor);

		if (Alpha >= 1.0f)
		{
			if (Param.bShouldClearImageOnCompleted)
			{
				Param.Target->SetBrush(FSlateNoResource());
			}
			KeysToRemove.Add(Pair.Key);
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		ImageFadeParams.Remove(Key);
	}
}

SHIDENCORE_API void UShidenWidget::MoveCanvasPanelSlot(const float DeltaTime)
{
	TArray<FString> KeysToRemove;
	for (TPair<FString, FShidenCanvasPanelSlotMoveParams>& Pair : CanvasPanelMoveParams)
	{
		FShidenCanvasPanelSlotMoveParams& Param = Pair.Value;
		Param.EasingAlpha += DeltaTime / Param.MoveDuration;
		const float Alpha = FMath::Clamp(Param.EasingAlpha, 0.0f, 1.0f);
		const TObjectPtr<UCanvasPanelSlot> PanelSlot = Param.Target;

		if (Param.bChangePosition)
		{
			const FVector2D NewPosition = FVector2D(
				UKismetMathLibrary::Ease(Param.StartPosition.X, Param.EndPosition.X, Alpha, Param.MoveFunction, Param.BlendExp, Param.Steps),
				UKismetMathLibrary::Ease(Param.StartPosition.Y, Param.EndPosition.Y, Alpha, Param.MoveFunction, Param.BlendExp, Param.Steps)
			);
			PanelSlot->SetPosition(NewPosition);
		}

		if (Param.bChangeSize)
		{
			const FVector2D NewSize = FVector2D(
				UKismetMathLibrary::Ease(Param.StartSize.X, Param.EndSize.X, Alpha, Param.MoveFunction, Param.BlendExp, Param.Steps),
				UKismetMathLibrary::Ease(Param.StartSize.Y, Param.EndSize.Y, Alpha, Param.MoveFunction, Param.BlendExp, Param.Steps)
			);
			PanelSlot->SetSize(NewSize);
		}

		if (Alpha >= 1.0f)
		{
			KeysToRemove.Add(Pair.Key);
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		CanvasPanelMoveParams.Remove(Key);
	}
}

SHIDENCORE_API void UShidenWidget::ChangeImageMaterial(const float DeltaTime)
{
	TArray<FString> KeysToRemove;
	for (TPair<FString, FShidenImageMaterialScalarParams>& Pair : ImageMaterialParams)
	{
		FShidenImageMaterialScalarParams& Param = Pair.Value;
		Param.EasingAlpha += DeltaTime / Param.MoveDuration;
		const float Alpha = FMath::Clamp(Param.EasingAlpha, 0.0f, 1.0f);
		const float NewValue = UKismetMathLibrary::Ease(Param.StartValue, Param.EndValue, Alpha, Param.MoveFunction, Param.BlendExp,
		                                                Param.Steps);

		TObjectPtr<UMaterialInstanceDynamic> Material = Param.Target->GetDynamicMaterial();

		if (!Material)
		{
			KeysToRemove.Add(Pair.Key);
			continue;
		}

		Material->SetScalarParameterValue(Param.ParameterName, NewValue);

		if (Alpha >= 1.0f)
		{
			KeysToRemove.Add(Pair.Key);
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		ImageMaterialParams.Remove(Key);
	}
}

SHIDENCORE_API void UShidenWidget::ChangeRetainerBoxMaterial(const float DeltaTime)
{
	TArray<FString> KeysToRemove;
	for (TPair<FString, FShidenRetainerBoxMaterialScalarParams>& Pair : RetainerBoxMaterialParams)
	{
		FShidenRetainerBoxMaterialScalarParams& Param = Pair.Value;
		Param.EasingAlpha += DeltaTime / Param.MoveDuration;
		const float Alpha = FMath::Clamp(Param.EasingAlpha, 0.0f, 1.0f);
		const float NewValue = UKismetMathLibrary::Ease(Param.StartValue, Param.EndValue, Alpha, Param.MoveFunction, Param.BlendExp,
		                                                Param.Steps);

		TObjectPtr<UMaterialInstanceDynamic> Material = Param.Target->GetEffectMaterial();

		if (!Material)
		{
			KeysToRemove.Add(Pair.Key);
			continue;
		}

		Material->SetScalarParameterValue(Param.ParameterName, NewValue);

		if (Alpha >= 1.0f)
		{
			KeysToRemove.Add(Pair.Key);
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		RetainerBoxMaterialParams.Remove(Key);
	}
}

SHIDENCORE_API void UShidenWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (MediaPlayer)
	{
		MediaPlayer->OnMediaClosed.AddUniqueDynamic(this, &UShidenWidget::OnMediaClosed);
		MediaPlayer->OnEndReached.AddUniqueDynamic(this, &UShidenWidget::OnMediaReachedEnd);
		MediaPlayer->OnPlaybackSuspended.AddUniqueDynamic(this, &UShidenWidget::OnMediaSuspended);
		MediaPlayer->OnPlaybackResumed.AddUniqueDynamic(this, &UShidenWidget::OnMediaResumed);
	}
}

SHIDENCORE_API void UShidenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateAllWidgetAnimations(TEXT(""), this);
	UpdateWidgetCache();
}

SHIDENCORE_API void UShidenWidget::OnMediaClicked_Implementation()
{
}

SHIDENCORE_API void UShidenWidget::OnMediaSuspended_Implementation()
{
}

SHIDENCORE_API void UShidenWidget::OnMediaResumed_Implementation()
{
}

SHIDENCORE_API void UShidenWidget::OnMediaReachedEnd_Implementation()
{
	bIsEndOfMedia = true;
}

SHIDENCORE_API void UShidenWidget::OnMediaClosed_Implementation()
{
	bIsEndOfMedia = true;
}

SHIDENCORE_API bool UShidenWidget::IsFadeCompleted(const FString& LayerName) const
{
	return !FadeParams.Contains(LayerName);
}

SHIDENCORE_API void UShidenWidget::ClearAllFade()
{
	FadeParams.Empty();
	for (TTuple<FString, TObjectPtr<UBorder>>& Widget : FadeWidgets)
	{
		if (IsValid(Widget.Value))
		{
			Widget.Value->RemoveFromParent();
		}
	}
	FadeWidgets.Empty();
}

SHIDENCORE_API bool UShidenWidget::TryFindTextWidget(const FString& TextWidgetName, UShidenTextWidget*& TextWidget) const
{
	if (const TObjectPtr<UShidenTextWidget> Result = TextWidgets.FindRef(TextWidgetName))
	{
		TextWidget = Result;
		return true;
	}
	return false;
}

SHIDENCORE_API void UShidenWidget::ClearAllTexts()
{
	for (const TTuple<FString, TObjectPtr<UShidenTextWidget>>& TextWidget : TextWidgets)
	{
		TextWidget.Value->ClearAllTexts();
	}
}

SHIDENCORE_API bool UShidenWidget::TryFindImage(const FString& ImageName, UImage*& Image) const
{
	if (const TObjectPtr<UImage> Result = Images.FindRef(ImageName))
	{
		Image = Result;
		return true;
	}
	return false;
}

SHIDENCORE_API void UShidenWidget::ClearAllImages()
{
	for (const TTuple<FString, TObjectPtr<UImage>>& Image : Images)
	{
		if (const TObjectPtr<UImage> ImageValue = Image.Value)
		{
			FSlateBrush Brush = FSlateBrush();
			Brush.TintColor = FLinearColor::Transparent;
			ImageValue->SetBrush(Brush);
		}
	}
}

SHIDENCORE_API bool UShidenWidget::TryStartImageFade(const FString& ImageName, UPARAM(ref)
                                                     UImage* Target,
                                                     const EEasingFunc::Type Function, const float Duration,
                                                     const bool bIsWhiteFade, const bool bShouldBeTransparent,
                                                     const float BlendExp, const int32 Steps,
                                                     const FString& OwnerProcessName, const bool bClearImageOnCompleted,
                                                     FString& ErrorMessage)
{
	if (const FShidenImageFadeParams* FadeParam = ImageFadeParams.Find(ImageName); FadeParam && FadeParam->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(TEXT("Process \"%s\" is currently executing the image command."), *FadeParam->OwnerProcessName);
		return false;
	}

	const float InRgb = bIsWhiteFade ? 1 : 0;

	if (Duration == 0)
	{
		const float InEndAlpha = bShouldBeTransparent ? 0 : 1;
		Target->SetColorAndOpacity(FLinearColor(InRgb, InRgb, InRgb, InEndAlpha));
		ImageFadeParams.Remove(ImageName);
		return true;
	}

	const float InStartAlpha = bShouldBeTransparent ? 1 : 0;

	Target->SetColorAndOpacity(FLinearColor(InRgb, InRgb, InRgb, InStartAlpha));

	FShidenImageFadeParams Params = FShidenImageFadeParams();
	Params.Target = Target;
	Params.FadeDuration = Duration;
	Params.bIsWhiteFade = bIsWhiteFade;
	Params.bShouldBeTransparent = bShouldBeTransparent;
	Params.FadeFunction = Function;
	Params.EasingAlpha = 0;
	Params.BlendExp = BlendExp;
	Params.Steps = Steps;
	Params.OwnerProcessName = OwnerProcessName;
	Params.bShouldClearImageOnCompleted = bClearImageOnCompleted;

	ImageFadeParams.Add(ImageName, Params);
	return true;
}

SHIDENCORE_API bool UShidenWidget::TryStartFade(const FString& LayerName, const EEasingFunc::Type Function,
                                                const float Duration, const FLinearColor TargetColor,
                                                const bool bIsFadeOut, const float BlendExp, const int32 Steps,
                                                const FString& OwnerProcessName, const int32 ZOrder, FString& ErrorMessage)
{
	TObjectPtr<UBorder> FadeWidget = FadeWidgets.FindRef(LayerName);
	TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot;

	if (!FadeWidget)
	{
		FadeWidget = NewObject<UBorder>(this);
		FadeWidget->SetBrushColor(FLinearColor(0, 0, 0, 0));
		BaseLayer->AddChild(FadeWidget);
		CanvasPanelSlot = Cast<UCanvasPanelSlot>(FadeWidget->Slot);
		CanvasPanelSlot->SetAnchors(FAnchors(0, 0, 1, 1));
		CanvasPanelSlot->SetOffsets(FMargin(0, 0));
		FadeWidgets.Add(LayerName, FadeWidget);
	}
	else
	{
		if (const FShidenFadeParams* FadeParam = FadeParams.Find(LayerName); FadeParam && FadeParam->OwnerProcessName != OwnerProcessName)
		{
			ErrorMessage = FString::Printf(TEXT("Process \"%s\" is currently executing the fade command."), *FadeParam->OwnerProcessName);
			return false;
		}
		CanvasPanelSlot = Cast<UCanvasPanelSlot>(FadeWidget->Slot);
	}

	FadeWidget->SetVisibility(ESlateVisibility::Visible);
	CanvasPanelSlot->SetZOrder(ZOrder);

	if (Duration == 0)
	{
		const float InAlpha = bIsFadeOut ? 1 : 0;
		FadeWidget->SetBrushColor(FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, InAlpha));
		if (!bIsFadeOut)
		{
			FadeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		return true;
	}

	const FLinearColor StartColor = FadeWidget->GetBrushColor().A == 0
		                                ? FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, bIsFadeOut ? 0 : 1)
		                                : FadeWidget->GetBrushColor();
	const FLinearColor ActualTargetColor =
		FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, bIsFadeOut ? 1 : 0);

	FShidenFadeParams FadeParam = FShidenFadeParams();
	FadeParam.FadeDuration = Duration;
	FadeParam.StartColor = StartColor;
	FadeParam.TargetColor = ActualTargetColor;
	FadeParam.bIsFadeOut = bIsFadeOut;
	FadeParam.EasingAlpha = 0;
	FadeParam.FadeFunction = Function;
	FadeParam.BlendExp = BlendExp;
	FadeParam.Steps = Steps;
	FadeParam.OwnerProcessName = OwnerProcessName;

	FadeParams.Add(LayerName, FadeParam);

	return true;
}

SHIDENCORE_API bool UShidenWidget::IsImageFadeCompleted(const FString& ImageName) const
{
	return !ImageFadeParams.Contains(ImageName);
}

SHIDENCORE_API void UShidenWidget::SetOptions_Implementation(const TArray<FString>& Options)
{
	SelectedOption = INDEX_NONE;
}

SHIDENCORE_API int32 UShidenWidget::GetSelectedOption() const noexcept
{
	return SelectedOption;
}

SHIDENCORE_API bool UShidenWidget::IsOptionSelected() const noexcept
{
	return SelectedOption > -1;
}

SHIDENCORE_API bool UShidenWidget::TryPlayMedia(const FString& MediaSourcePath, const bool bCanOpenPauseMenu, const int32 MediaZOrder)
{
	Cast<UCanvasPanelSlot>(MediaLayer->Slot)->SetZOrder(MediaZOrder);
	MediaLayer->SetVisibility(ESlateVisibility::Visible);
	UObject* Asset = nullptr;
	if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(MediaSourcePath, Asset))
	{
		return false;
	}
	const TObjectPtr<UMediaSource> MediaSource = Cast<UMediaSource>(Asset);
	if (!MediaSource)
	{
		return false;
	}
	bCanOpenMediaPauseMenu = bCanOpenPauseMenu;
	bIsEndOfMedia = false;
	MediaPlayer->OpenSource(MediaSource);
	return true;
}

SHIDENCORE_API bool UShidenWidget::IsMediaCompleted() const noexcept
{
	return bIsEndOfMedia;
}

SHIDENCORE_API void UShidenWidget::CollapseMedia() const
{
	if (MediaLayer)
	{
		MediaLayer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

SHIDENCORE_API bool UShidenWidget::TryFindRetainerBox(const FString& RetainerBoxName, URetainerBox*& RetainerBox) const
{
	const TObjectPtr<URetainerBox> Result = RetainerBoxes.FindRef(RetainerBoxName);
	RetainerBox = Result;
	return Result ? true : false;
}

SHIDENCORE_API FString UShidenWidget::MakeMaterialParamsKey(const FString& TargetName, const FString& ParameterName)
{
	const FString EscapedTargetName = TargetName.Replace(TEXT(":"), TEXT("\\:"));
	const FString EscapedParameterName = ParameterName.Replace(TEXT(":"), TEXT("\\:"));
	return FString::Printf(TEXT("%s::%s"), *EscapedTargetName, *EscapedParameterName);
}

SHIDENCORE_API bool UShidenWidget::TryStartImageMaterialScalarChange(const FString& ImageName, UPARAM(ref)
                                                                     UImage* Target,
                                                                     const FName& ParameterName, const EEasingFunc::Type Function,
                                                                     const float Duration, const float EndValue,
                                                                     const float BlendExp, const int32 Steps,
                                                                     const FString& OwnerProcessName, FString& ErrorMessage)
{
	const FString Key = MakeMaterialParamsKey(ImageName, ParameterName.ToString());
	const FShidenImageMaterialScalarParams* Params = ImageMaterialParams.Find(Key);
	if (Params && Params->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(
			TEXT("Process \"%s\" is currently executing the change material scalar parameter command."),
			*Params->OwnerProcessName);
		return false;
	}

	const TObjectPtr<UMaterialInstanceDynamic> Material = Target->GetDynamicMaterial();
	if (!Material)
	{
		ErrorMessage = FString::Printf(TEXT("Material of image \"%s\" is not found."), *ImageName);
		return false;
	}

	if (Duration == 0)
	{
		Material->SetScalarParameterValue(ParameterName, EndValue);
		ImageMaterialParams.Remove(Key);
		return true;
	}

	if (Params)
	{
		Material->SetScalarParameterValue(ParameterName, Params->EndValue);
	}

	float StartValue = 0;

	Material->GetScalarParameterValue(ParameterName, StartValue);

	FShidenImageMaterialScalarParams MaterialParams = FShidenImageMaterialScalarParams();
	MaterialParams.Target = Target;
	MaterialParams.ParameterName = ParameterName;
	MaterialParams.MoveDuration = Duration;
	MaterialParams.MoveFunction = Function;
	MaterialParams.EasingAlpha = 0;
	MaterialParams.StartValue = StartValue;
	MaterialParams.EndValue = EndValue;
	MaterialParams.BlendExp = BlendExp;
	MaterialParams.Steps = Steps;
	MaterialParams.OwnerProcessName = OwnerProcessName;

	ImageMaterialParams.Add(Key, MaterialParams);
	return true;
}

SHIDENCORE_API bool UShidenWidget::IsImageMaterialParameterChangeCompleted(const FString& ImageName, const FString& ParameterName) const
{
	const FString Key = MakeMaterialParamsKey(ImageName, ParameterName);
	return !ImageMaterialParams.Contains(Key);
}

SHIDENCORE_API bool UShidenWidget::TryStartRetainerBoxMaterialScalarChange(const FString& RetainerBoxName, UPARAM(ref)
                                                                           URetainerBox* Target,
                                                                           const FName& ParameterName, const EEasingFunc::Type Function,
                                                                           const float Duration, const float EndValue, const float BlendExp,
                                                                           const int32 Steps, const FString& OwnerProcessName, FString& ErrorMessage)
{
	const FString Key = MakeMaterialParamsKey(RetainerBoxName, ParameterName.ToString());
	const FShidenRetainerBoxMaterialScalarParams* Params = RetainerBoxMaterialParams.Find(Key);
	if (Params && Params->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(
			TEXT("Process \"%s\" is currently executing the change material scalar parameter command."),
			*Params->OwnerProcessName);
		return false;
	}

	const TObjectPtr<UMaterialInstanceDynamic> Material = Target->GetEffectMaterial();
	if (!Material)
	{
		ErrorMessage = FString::Printf(TEXT("Material of retainer box \"%s\" is not found."), *RetainerBoxName);
		return false;
	}

	if (Duration == 0)
	{
		Material->SetScalarParameterValue(ParameterName, EndValue);
		RetainerBoxMaterialParams.Remove(Key);
		return true;
	}

	if (Params)
	{
		Material->SetScalarParameterValue(ParameterName, Params->EndValue);
	}

	float StartValue = 0;
	Material->GetScalarParameterValue(ParameterName, StartValue);

	FShidenRetainerBoxMaterialScalarParams MaterialParams = FShidenRetainerBoxMaterialScalarParams();
	MaterialParams.Target = Target;
	MaterialParams.ParameterName = ParameterName;
	MaterialParams.MoveDuration = Duration;
	MaterialParams.MoveFunction = Function;
	MaterialParams.EasingAlpha = 0;
	MaterialParams.StartValue = StartValue;
	MaterialParams.EndValue = EndValue;
	MaterialParams.BlendExp = BlendExp;
	MaterialParams.Steps = Steps;
	MaterialParams.OwnerProcessName = OwnerProcessName;

	RetainerBoxMaterialParams.Add(Key, MaterialParams);
	return true;
}

SHIDENCORE_API bool UShidenWidget::IsRetainerBoxMaterialParameterChangeCompleted(const FString& RetainerBoxName, const FString& ParameterName) const
{
	const FString Key = MakeMaterialParamsKey(RetainerBoxName, ParameterName);
	return !RetainerBoxMaterialParams.Contains(Key);
}

SHIDENCORE_API bool UShidenWidget::TryStartCanvasPanelSlotMove(const FString& CanvasPanelSlotName, UPARAM(ref)
                                                               UCanvasPanelSlot* Target,
                                                               const EEasingFunc::Type Function, const float Duration,
                                                               const bool bChangePosition, const FVector2D EndPosition,
                                                               const bool bChangeSize, const FVector2D EndSize, const float BlendExp,
                                                               const int32 Steps, const FString& OwnerProcessName, FString& ErrorMessage)
{
	const FShidenCanvasPanelSlotMoveParams* Params = CanvasPanelMoveParams.Find(CanvasPanelSlotName);
	if (Params && Params->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(TEXT("Process \"%s\" is currently executing the move canvas panel command."), *Params->OwnerProcessName);
		return false;
	}

	if (!Target)
	{
		ErrorMessage = TEXT("Canvas panel slot is null.");
		return false;
	}

	if (Duration == 0)
	{
		if (bChangePosition)
		{
			Target->SetPosition(EndPosition);
		}
		if (bChangeSize)
		{
			Target->SetSize(EndSize);
		}
		CanvasPanelMoveParams.Remove(CanvasPanelSlotName);
		return true;
	}

	if (Params)
	{
		if (Params->bChangePosition)
		{
			Target->SetPosition(Params->EndPosition);
		}
		if (Params->bChangeSize)
		{
			Target->SetSize(Params->EndSize);
		}
	}

	FShidenCanvasPanelSlotMoveParams MoveParams = FShidenCanvasPanelSlotMoveParams();
	MoveParams.Target = Target;
	MoveParams.MoveDuration = Duration;
	MoveParams.MoveFunction = Function;
	MoveParams.EasingAlpha = 0;
	MoveParams.bChangePosition = bChangePosition;
	MoveParams.StartPosition = Target->GetPosition();
	MoveParams.EndPosition = EndPosition;
	MoveParams.bChangeSize = bChangeSize;
	MoveParams.StartSize = Target->GetSize();
	MoveParams.EndSize = EndSize;
	MoveParams.BlendExp = BlendExp;
	MoveParams.Steps = Steps;
	MoveParams.OwnerProcessName = OwnerProcessName;

	CanvasPanelMoveParams.Add(CanvasPanelSlotName, MoveParams);
	return true;
}

SHIDENCORE_API bool UShidenWidget::IsCanvasPanelSlotMoveCompleted(const FString& CanvasPanelName) const
{
	return !CanvasPanelMoveParams.Contains(CanvasPanelName);
}

SHIDENCORE_API bool UShidenWidget::TryFindCanvasPanelSlot(const FString& CanvasPanelSlotName, UCanvasPanelSlot*& CanvasPanelSlot) const
{
	if (const TObjectPtr<UCanvasPanelSlot> Result = CanvasPanelSlots.FindRef(CanvasPanelSlotName))
	{
		CanvasPanelSlot = Result;
		return true;
	}
	return false;
}

SHIDENCORE_API bool UShidenWidget::TryFindAnimation(const FString& AnimationName, UUserWidget*& TargetWidget, UWidgetAnimation*& WidgetAnimation) const
{
	if (const FShidenAnimation* Result = WidgetAnimations.Find(AnimationName))
	{
		WidgetAnimation = Result->Animation;
		TargetWidget = Result->TargetWidget;
		return true;
	}
	return false;
}

SHIDENCORE_API void UShidenWidget::ResetAllAnimations()
{
	for (const TTuple<FString, FShidenAnimation>& WidgetAnimation : WidgetAnimations)
	{
		if (WidgetAnimation.Value.TargetWidget && WidgetAnimation.Value.Animation)
		{
			const float EndTime = WidgetAnimation.Value.Animation->GetEndTime();
			WidgetAnimation.Value.TargetWidget->PlayAnimation(WidgetAnimation.Value.Animation, EndTime, 1, EUMGSequencePlayMode::Reverse, 1.0, false);
		}
	}
}

SHIDENCORE_API void UShidenWidget::SetInputModeTextInput_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Please override SetInputModeTextInput function of Shiden Widget!"));
}

SHIDENCORE_API bool UShidenWidget::IsTextSubmitted() const noexcept
{
	return bIsTextSubmitted;
}

SHIDENCORE_API void UShidenWidget::SetInputModeOptionSelection_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Please override SetInputModeOptionSelection function of Shiden Widget!"));
}

SHIDENCORE_API void UShidenWidget::InitTextInput_Implementation(const FShidenTextInputProperties& Properties)
{
	UE_LOG(LogTemp, Warning, TEXT("Please override InitTextInput function of Shiden Widget!"));
}

SHIDENCORE_API void UShidenWidget::SetInputModeGameAndUI_Implementation()
{
	FInputModeGameAndUI InputMode = FInputModeGameAndUI();
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(true);
	UGameplayStatics::GetPlayerController(this->GetWorld(), 0)->SetInputMode(InputMode);
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

SHIDENCORE_API void UShidenWidget::SanitizeInputText(const FString& Text, const FShidenTextInputProperties& Properties, FString& Result)
{
	TArray<FString> Lines;
	Text.ParseIntoArrayLines(Lines, false);
	Result = TEXT("");
	if (Properties.MaxLines > 0 && Properties.MaxLines < Lines.Num())
	{
		for (int32 Index = 0; Index < Properties.MaxLines; Index++)
		{
			Result += Lines[Index];
			if (Index < Properties.MaxLines - 1)
			{
				Result += TEXT("\n");
			}
		}
	}
	else
	{
		Result = Text;
	}

	int32 NewLineCount = 0;
	for (const TCHAR& Char : Result)
	{
		if (Char == '\n' || Char == '\r')
		{
			NewLineCount++;
		}
	}

	if (Properties.MaxLength > 0)
	{
		Result = Result.Left(Properties.MaxLength + NewLineCount);
	}

	if (!Properties.AllowedCharacterRegex.IsEmpty())
	{
		FString SanitizedText;
		const FRegexPattern Pattern(Properties.AllowedCharacterRegex);
		FRegexMatcher Matcher(Pattern, Result);
		while (Matcher.FindNext())
		{
			SanitizedText += Matcher.GetCaptureGroup(0);
		}
		Result = SanitizedText;
	}
}

SHIDENCORE_API bool UShidenWidget::IsSkipPressed() const noexcept
{
	return bPressSkipButton;
}

SHIDENCORE_API void UShidenWidget::GetVisibilityByName(const FString& Name, ESlateVisibility& Result) const
{
	if (const TObjectPtr<UWidget> Widget = AllWidgets[Name])
	{
		Result = Widget->GetVisibility();
	}
}

SHIDENCORE_API bool UShidenWidget::TrySetVisibilityByName(const FString& Name, const ESlateVisibility NewVisibility,
                                                          const bool bShouldRegisterScenarioProperty)
{
	if (!IsValid(this) || !AllWidgets.Contains(Name))
	{
		return false;
	}

	const TObjectPtr<UWidget> Widget = AllWidgets[Name];
	if (!Widget)
	{
		return false;
	}

	Widget->SetVisibility(NewVisibility);
	if (bShouldRegisterScenarioProperty)
	{
		FString VisibilityStr;

		switch (NewVisibility)
		{
		case ESlateVisibility::Visible:
			VisibilityStr = TEXT("Visible");
			break;
		case ESlateVisibility::Collapsed:
			VisibilityStr = TEXT("Collapsed");
			break;
		case ESlateVisibility::Hidden:
			VisibilityStr = TEXT("Hidden");
			break;
		case ESlateVisibility::HitTestInvisible:
			VisibilityStr = TEXT("Not Hit-Testable (Self & All Children)");
			break;
		case ESlateVisibility::SelfHitTestInvisible:
			VisibilityStr = TEXT("Not Hit-Testable (Self Only)");
			break;
		}

		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(TEXT("ChangeVisibility"), Name, VisibilityStr);
	}
	return true;
}

SHIDENCORE_API bool UShidenWidget::IsMenuOpen() const
{
	if (!MenuBaseLayer->IsVisible())
	{
		return false;
	}

	for (TArray<UWidget*> Children = MenuBaseLayer->GetAllChildren(); const TObjectPtr<UWidget> Child : Children)
	{
		if (Child->IsVisible())
		{
			return true;
		}
	}

	return false;
}

SHIDENCORE_API void UShidenWidget::UpdateWidgetCacheCore(const FString& Prefix, const UWidgetTree* Tree)
{
	if (!Tree)
	{
		return;
	}

	TArray<UWidget*> Children;
	Tree->GetAllWidgets(Children);

	for (TObjectPtr<UWidget> Child : Children)
	{
		const FString ChildName = Child->GetName();
		if (ChildName.Contains(TEXT(".")))
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("Widget name \"%s\" contains a period. Please avoid using periods in widget names."),
			       *ChildName);
		}

		const FString WidgetPath = Prefix.IsEmpty() ? ChildName : Prefix + TEXT(".") + ChildName;

		if (const TObjectPtr<UUserWidget> UserWidget = Cast<UUserWidget>(Child))
		{
			UpdateWidgetCacheCore(WidgetPath, UserWidget->WidgetTree);
		}

		AllWidgets.Add(WidgetPath, Child);

		if (TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot = Cast<UCanvasPanelSlot>(Child->Slot))
		{
			CanvasPanelSlots.Add(WidgetPath, CanvasPanelSlot);
		}

		if (TObjectPtr<UShidenTextWidget> TextWidget = Cast<UShidenTextWidget>(Child))
		{
			TextWidgets.Add(WidgetPath, TextWidget);
		}
		else if (TObjectPtr<UImage> Image = Cast<UImage>(Child))
		{
			Images.Add(WidgetPath, Image);
		}
		else if (TObjectPtr<URetainerBox> RetainerBox = Cast<URetainerBox>(Child))
		{
			RetainerBoxes.Add(WidgetPath, RetainerBox);
		}
	}
}

SHIDENCORE_API void UShidenWidget::UpdateWidgetCache()
{
	UpdateWidgetCacheCore(TEXT(""), WidgetTree);
}

SHIDENCORE_API void UShidenWidget::UpdateAllWidgetAnimations(const FString& Prefix, const UUserWidget* TargetWidget)
{
	if (!TargetWidget)
	{
		return;
	}

	for (TFieldIterator<FProperty> PropIt(TargetWidget->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		if (const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			FString ClassName = ObjectProperty->PropertyClass->GetName();

			if (ObjectProperty->PropertyClass->IsChildOf(UWidgetAnimation::StaticClass()))
			{
				if (const TObjectPtr<UWidgetAnimation> Animation = Cast<UWidgetAnimation>(
					ObjectProperty->GetObjectPropertyValue_InContainer(TargetWidget)))
				{
					WidgetAnimations.Add(Prefix + Property->GetName(), FShidenAnimation{const_cast<UUserWidget*>(TargetWidget), Animation});
				}
			}
			else if (ObjectProperty->PropertyClass->IsChildOf(UUserWidget::StaticClass()))
			{
				if (const TObjectPtr<UUserWidget> UserWidget = Cast<UUserWidget>(ObjectProperty->GetObjectPropertyValue_InContainer(TargetWidget)))
				{
					UpdateAllWidgetAnimations(Prefix + Property->GetName() + TEXT("."), UserWidget);
				}
			}
		}
	}
}

SHIDENCORE_API void UShidenWidget::GetAllWidgets(TArray<UWidget*>& Children) const
{
	WidgetTree->GetAllWidgets(Children);
}

SHIDENCORE_API bool UShidenWidget::TryFindCanvasPanelMoveParams(const FString& CanvasPanelName, FShidenCanvasPanelSlotMoveParams& Value)
{
	if (const FShidenCanvasPanelSlotMoveParams* Params = CanvasPanelMoveParams.Find(CanvasPanelName))
	{
		Value = *Params;
		return true;
	}
	return false;
}

SHIDENCORE_API bool UShidenWidget::TryFindImageFadeParams(const FString& ImageName, FShidenImageFadeParams& Value)
{
	if (const FShidenImageFadeParams* Params = ImageFadeParams.Find(ImageName))
	{
		Value = *Params;
		return true;
	}
	return false;
}

SHIDENCORE_API bool UShidenWidget::TryFindImageMaterialScalarParams(const FString& MaterialParamsKey, FShidenImageMaterialScalarParams& Value)
{
	if (const FShidenImageMaterialScalarParams* Params = ImageMaterialParams.Find(MaterialParamsKey))
	{
		Value = *Params;
		return true;
	}
	return false;
}

SHIDENCORE_API bool UShidenWidget::TryFindRetainerBoxMaterialScalarParams(const FString& MaterialParamsKey, FShidenRetainerBoxMaterialScalarParams& Value)
{
	if (const FShidenRetainerBoxMaterialScalarParams* Params = RetainerBoxMaterialParams.Find(MaterialParamsKey))
	{
		Value = *Params;
		return true;
	}
	return false;
}

SHIDENCORE_API bool UShidenWidget::IsTextVisible() const
{
	if (!TextLayer->IsVisible() && !OptionLayer->IsVisible() && !TextInputLayer->IsVisible())
	{
		return false;
	}

	if (OptionLayer->IsVisible() || TextInputLayer->IsVisible())
	{
		return true;
	}

	const TArray<UWidget*> Children = TextLayer->GetAllChildren();
	for (const UWidget* Child : Children)
	{
		if (Child->IsVisible())
		{
			return true;
		}
	}

	return false;
}
