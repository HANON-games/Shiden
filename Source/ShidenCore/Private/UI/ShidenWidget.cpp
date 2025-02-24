// Copyright (c) 2024 HANON. All Rights Reserved.

#include "UI/ShidenWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Utility/ShidenCoreFunctionLibrary.h"
#include "Runtime/MediaAssets/Public/MediaSource.h"
#include "Scenario/ShidenScenarioFunctionLibrary.h"
#include "Command/ShidenCommandFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/WidgetRenderer.h"
#include "TextureResource.h"
#include "Runtime/RenderCore/Public/RenderingThread.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Framework/Application/SlateApplication.h"

TArray<FColor> ResizeBitmap(const TArray<FColor>& OriginalBitmap, const int32 OriginalWidth, const int32 OriginalHeight, const int32 TargetWidth, const int32 TargetHeight)
{
	TArray<FColor> ResizedBitmap;
	ResizedBitmap.SetNumZeroed(TargetWidth * TargetHeight);

	const float XRatio = static_cast<float>(OriginalWidth) / TargetWidth;
	const float YRatio = static_cast<float>(OriginalHeight) / TargetHeight;

	for (int32 y = 0; y < TargetHeight; ++y)
	{
		for (int32 x = 0; x < TargetWidth; ++x)
		{
			int32 SrcX = FMath::FloorToInt(x * XRatio);
			int32 SrcY = FMath::FloorToInt(y * YRatio);

			SrcX = FMath::Clamp(SrcX, 0, OriginalWidth - 1);
			SrcY = FMath::Clamp(SrcY, 0, OriginalHeight - 1);

			ResizedBitmap[y * TargetWidth + x] = OriginalBitmap[SrcY * OriginalWidth + SrcX];
		}
	}

	return ResizedBitmap;
}

SHIDENCORE_API void UShidenWidget::CaptureWidget(const bool bShowTextBaseLayer, UTexture2D*& ResultTexture, bool& bResult)
{
	bResult = false;

	const TSharedRef<SWidget> WidgetRef = TakeWidget();
	FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(true, false);

	const FVector2D OutSize = FVector2D(WidgetRef->GetDesiredSize().X, WidgetRef->GetDesiredSize().Y);
	constexpr TextureFilter Filter = TF_Bilinear;

	if (UTextureRenderTarget2D* RenderTarget = FWidgetRenderer::CreateTargetFor(OutSize, Filter, true))
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

		TArray<FColor> Bitmap;

		if (RenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(Bitmap, FReadSurfaceDataFlags()))
		{
			Bitmap = ResizeBitmap(Bitmap, OutSize.X, OutSize.Y, OutSize.X * CaptureScale, OutSize.Y * CaptureScale);
			for (FColor& Color : Bitmap)
			{
				Color.A = 255;
			}

			ResultTexture =
				UTexture2D::CreateTransient(OutSize.X * CaptureScale, OutSize.Y * CaptureScale, PF_B8G8R8A8);
			ResultTexture->SRGB = true;
			ResultTexture->AddToRoot();
			FTexture2DMipMap& Mip = ResultTexture->GetPlatformData()->Mips[0];
			void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(Data, Bitmap.GetData(), Bitmap.Num() * sizeof(FColor));
			Mip.BulkData.Unlock();
			ResultTexture->UpdateResource();

			bResult = true;
		}
	}
}

SHIDENCORE_API void UShidenWidget::TakeScreenshot(const bool bShowTextBaseLayer, UTexture2D*& ResultTexture, bool& bResult)
{
	bResult = false;

	if (FSlateApplication::IsInitialized())
	{
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

		Bitmap = ResizeBitmap(Bitmap, OutSize.X, OutSize.Y, OutSize.X * CaptureScale, OutSize.Y * CaptureScale);

		if (bScreenshotSuccessful)
		{
			for (FColor& Color : Bitmap)
			{
				Color.A = 255;
			}

			ResultTexture =
				UTexture2D::CreateTransient(OutSize.X * CaptureScale, OutSize.Y * CaptureScale, PF_B8G8R8A8);
			ResultTexture->AddToRoot();
			FTexture2DMipMap& Mip = ResultTexture->GetPlatformData()->Mips[0];
			void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(Data, Bitmap.GetData(), Bitmap.Num() * sizeof(FColor));
			Mip.BulkData.Unlock();
			ResultTexture->SRGB = true;
			ResultTexture->UpdateResource();

			bResult = true;
		}
	}
}

SHIDENCORE_API void UShidenWidget::CaptureScreenToTexture2D(UTexture2D*& ResultTexture, bool& bResult)
{
	switch (CaptureScreenMode)
	{
	case EShidenCaptureScreenMode::UIOnly:
		CaptureWidget(true, ResultTexture, bResult);
		break;
	case EShidenCaptureScreenMode::UIOnlyWithoutTextBaseLayer:
		CaptureWidget(false, ResultTexture, bResult);
		break;
	case EShidenCaptureScreenMode::FullScene:
		TakeScreenshot(true, ResultTexture, bResult);
		break;
	case EShidenCaptureScreenMode::FullSceneWithoutTextBaseLayer:
		TakeScreenshot(false, ResultTexture, bResult);
		break;
	}
}

SHIDENCORE_API void UShidenWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const bool bIsSkippedResult = bIsSkipped && UShidenScenarioFunctionLibrary::CanSkipCommand();

	const UShidenSubsystem* ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);
	
	const float DeltaTime = bIsSkippedResult
		? InDeltaTime * ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate
		: InDeltaTime;

	FadeInOut(DeltaTime);
	ImageFadeInOut(DeltaTime);
	MoveCanvasPanelSlot(DeltaTime);
	ChangeImageMaterial(DeltaTime);
	ChangeRetainerBoxMaterial(DeltaTime);
}

SHIDENCORE_API void UShidenWidget::FadeInOut(const float DeltaTime)
{
	TArray<FString> Keys;
	FadeParams.GetKeys(Keys);

	for (FString& Key : Keys)
	{
		FShidenFadeParams* Param = FadeParams.Find(Key);
		Param->EasingAlpha += DeltaTime / Param->FadeDuration;
		const float Alpha = FMath::Clamp(Param->EasingAlpha, 0.0f, 1.0f);
		const float ResultR = UKismetMathLibrary::Ease(Param->StartColor.R, Param->TargetColor.R, Alpha, Param->FadeFunction, Param->BlendExp, Param->Steps);
		const float ResultG = UKismetMathLibrary::Ease(Param->StartColor.G, Param->TargetColor.G, Alpha, Param->FadeFunction, Param->BlendExp, Param->Steps);
		const float ResultB = UKismetMathLibrary::Ease(Param->StartColor.B, Param->TargetColor.B, Alpha, Param->FadeFunction, Param->BlendExp, Param->Steps);
		const float ResultA = UKismetMathLibrary::Ease(Param->StartColor.A, Param->TargetColor.A, Alpha, Param->FadeFunction, Param->BlendExp, Param->Steps);

		const TObjectPtr<UBorder> FadeWidget = FadeWidgets.FindRef(Key);

		if (FadeWidget)
		{
			FadeWidget->SetBrushColor(FLinearColor(ResultR, ResultG, ResultB, ResultA));
		}

		if (Alpha >= 1.0f)
		{
			FadeParams.Remove(Key);
			if (ResultA == 0.0f && FadeWidget)
			{
				FadeWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

SHIDENCORE_API void UShidenWidget::ImageFadeInOut(const float DeltaTime)
{
	TArray<FString> Keys;
	ImageFadeParams.GetKeys(Keys);

	for (FString Key : Keys)
	{
		FShidenImageFadeParams* Param = ImageFadeParams.Find(Key);
		Param->EasingAlpha += DeltaTime / Param->FadeDuration;
		const float Alpha = FMath::Clamp(Param->EasingAlpha, 0.0f, 1.0f);

		FLinearColor NewColor = Param->bIsWhiteFade ? FLinearColor::White : FLinearColor::Black;
		NewColor.A = UKismetMathLibrary::Ease(Param->bToBeTransparent ? 1 : 0, Param->bToBeTransparent ? 0 : 1, Alpha, Param->FadeFunction, Param->BlendExp, Param->Steps);

		Param->Target->SetColorAndOpacity(NewColor);

		if (Alpha >= 1.0f)
		{
			ImageFadeParams.Remove(Key);
		}
	}
}

SHIDENCORE_API void UShidenWidget::MoveCanvasPanelSlot(const float DeltaTime)
{
	TArray<FString> Keys;
	CanvasPanelMoveParams.GetKeys(Keys);

	for (FString& Key : Keys)
	{
		FShidenCanvasPanelSlotMoveParams* Param = CanvasPanelMoveParams.Find(Key);
		Param->EasingAlpha += DeltaTime / Param->MoveDuration;
		const float Alpha = FMath::Clamp(Param->EasingAlpha, 0.0f, 1.0f);
		const TObjectPtr<UCanvasPanelSlot> PanelSlot = Param->Target;

		if (Param->bChangePosition)
		{
			const FVector2D NewPosition = FVector2D(
				UKismetMathLibrary::Ease(Param->StartPosition.X, Param->EndPosition.X, Alpha, Param->MoveFunction, Param->BlendExp, Param->Steps),
				UKismetMathLibrary::Ease(Param->StartPosition.Y, Param->EndPosition.Y, Alpha, Param->MoveFunction, Param->BlendExp, Param->Steps)
			);
			PanelSlot->SetPosition(NewPosition);
		}

		if (Param->bChangeSize)
		{
			const FVector2D NewSize = FVector2D(
				UKismetMathLibrary::Ease(Param->StartSize.X, Param->EndSize.X, Alpha, Param->MoveFunction, Param->BlendExp, Param->Steps),
				UKismetMathLibrary::Ease(Param->StartSize.Y, Param->EndSize.Y, Alpha, Param->MoveFunction, Param->BlendExp, Param->Steps)
			);
			PanelSlot->SetSize(NewSize);
		}

		if (Alpha >= 1.0f)
		{
			CanvasPanelMoveParams.Remove(Key);
		}
	}
}

SHIDENCORE_API void UShidenWidget::ChangeImageMaterial(const float DeltaTime)
{
	TArray<FString> Keys;
	ImageMaterialParams.GetKeys(Keys);

	for (FString Key : Keys)
	{
		FShidenImageMaterialScalarParams* Param = ImageMaterialParams.Find(Key);
		Param->EasingAlpha += DeltaTime / Param->MoveDuration;
		const float Alpha = FMath::Clamp(Param->EasingAlpha, 0.0f, 1.0f);
		const float NewValue = UKismetMathLibrary::Ease(Param->StartValue, Param->EndValue, Alpha, Param->MoveFunction, Param->BlendExp, Param->Steps);

		TObjectPtr<UMaterialInstanceDynamic> Material = Param->Target->GetDynamicMaterial();

		if (!Material)
		{
			ImageMaterialParams.Remove(Key);
			continue;
		}

		Material->SetScalarParameterValue(Param->ParameterName, NewValue);

		if (Alpha >= 1.0f)
		{
			ImageMaterialParams.Remove(Key);
		}
	}
}

SHIDENCORE_API void UShidenWidget::ChangeRetainerBoxMaterial(const float DeltaTime)
{
	TArray<FString> Keys;
	RetainerBoxMaterialParams.GetKeys(Keys);

	for (FString Key : Keys)
	{
		FShidenRetainerBoxMaterialScalarParams* Param = RetainerBoxMaterialParams.Find(Key);
		Param->EasingAlpha += DeltaTime / Param->MoveDuration;
		const float Alpha = FMath::Clamp(Param->EasingAlpha, 0.0f, 1.0f);
		const float NewValue = UKismetMathLibrary::Ease(Param->StartValue, Param->EndValue, Alpha, Param->MoveFunction, Param->BlendExp, Param->Steps);

		TObjectPtr<UMaterialInstanceDynamic> Material = Param->Target->GetEffectMaterial();

		if (!Material)
		{
			RetainerBoxMaterialParams.Remove(Key);
			continue;
		}

		Material->SetScalarParameterValue(Param->ParameterName, NewValue);

		if (Alpha >= 1.0f)
		{
			RetainerBoxMaterialParams.Remove(Key);
		}
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

SHIDENCORE_API void UShidenWidget::IsFadeCompleted(const FString& LayerName, bool& bResult) const
{
	bResult = !FadeParams.Contains(LayerName);
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

SHIDENCORE_API void UShidenWidget::FindTextWidget(const FString& TextWidgetName, UShidenTextWidget*& TextWidget, bool& bResult) const
{
	const TObjectPtr<UShidenTextWidget> Result = TextWidgets.FindRef(TextWidgetName);
	if (!Result)
	{
		bResult = false;
		return;
	}
	TextWidget = Result;
	bResult = true;
}

SHIDENCORE_API void UShidenWidget::ClearAllTexts()
{
	for (const TTuple<FString, TObjectPtr<UShidenTextWidget>>& TextWidget : TextWidgets)
	{
		TextWidget.Value->ClearAllTexts();
	}
}

SHIDENCORE_API void UShidenWidget::FindImage(const FString& ImageName, UImage*& Image, bool& bResult) const
{
	const TObjectPtr<UImage> Result = Images.FindRef(ImageName);
	if (!Result)
	{
		bResult = false;
		return;
	}
	Image = Result;
	bResult = true;
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

SHIDENCORE_API void UShidenWidget::StartImageFade(const FString& ImageName, UPARAM(ref) UImage* Target,
                                                  const EEasingFunc::Type Function, const float Duration,
                                                  const bool bIsWhiteFade, const bool bShouldBeTransparent,
                                                  const float BlendExp, const int32 Steps,
                                                  const FString& OwnerProcessName, bool& Success, FString& ErrorMessage)
{
	if (const FShidenImageFadeParams* FadeParam = ImageFadeParams.Find(ImageName); FadeParam && FadeParam->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(TEXT("Process \"%s\" is currently executing the image command."), *FadeParam->OwnerProcessName);
		Success = false;
		return;
	}

	const float InRgb = bIsWhiteFade ? 1 : 0;

	if (Duration == 0)
	{
		const float InEndAlpha = bShouldBeTransparent ? 0 : 1;
		Target->SetColorAndOpacity(FLinearColor(InRgb, InRgb, InRgb, InEndAlpha));
		ImageFadeParams.Remove(ImageName);
		Success = true;
		return;
	}

	const float InStartAlpha = bShouldBeTransparent ? 1 : 0;

	Target->SetColorAndOpacity(FLinearColor(InRgb, InRgb, InRgb, InStartAlpha));

	FShidenImageFadeParams Params = FShidenImageFadeParams();
	Params.Target = Target;
	Params.FadeDuration = Duration;
	Params.bIsWhiteFade = bIsWhiteFade;
	Params.bToBeTransparent = bShouldBeTransparent;
	Params.FadeFunction = Function;
	Params.EasingAlpha = 0;
	Params.BlendExp = BlendExp;
	Params.Steps = Steps;
	Params.OwnerProcessName = OwnerProcessName;

	ImageFadeParams.Add(ImageName, Params);
	Success = true;
}

SHIDENCORE_API void UShidenWidget::StartFade(const FString& LayerName, const EEasingFunc::Type Function,
                                             const float Duration, const FLinearColor TargetColor,
                                             const bool bIsFadeOut, const float BlendExp, const int32 Steps,
                                             const FString& OwnerProcessName, const int32 ZOrder, bool& Success,
                                             FString& ErrorMessage)
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
			Success = false;
			return;
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
		Success = true;
		return;
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

	Success = true;
}

SHIDENCORE_API void UShidenWidget::IsImageFadeCompleted(const FString& ImageName, bool& bResult) const
{
	bResult = !ImageFadeParams.Contains(ImageName);
}

SHIDENCORE_API void UShidenWidget::SetOptions_Implementation(const TArray<FString>& Options)
{
	SelectedOption = -1;
}

SHIDENCORE_API void UShidenWidget::GetSelectedOption(int32& Result) const
{
	Result = SelectedOption;
}

SHIDENCORE_API bool UShidenWidget::IsOptionSelected() const
{
	return SelectedOption > -1;
}

SHIDENCORE_API void UShidenWidget::PlayMedia(const FString& MediaSourcePath, const bool bCanOpenPauseMenu, const int32 MediaZOrder, bool& bSuccess)
{
	Cast<UCanvasPanelSlot>(MediaLayer->Slot)->SetZOrder(MediaZOrder);
	MediaLayer->SetVisibility(ESlateVisibility::Visible);
	UObject* Asset = nullptr;
	bool bTempSuccess = false;
	UShidenCoreFunctionLibrary::GetOrLoadAsset(MediaSourcePath, Asset, bTempSuccess);
	if (!bTempSuccess)
	{
		bSuccess = false;
		return;
	}
	const TObjectPtr<UMediaSource> MediaSource = Cast<UMediaSource>(Asset);
	if (!MediaSource)
	{
		bSuccess = false;
		return;
	}
	bCanOpenMediaPauseMenu = bCanOpenPauseMenu;
	bIsEndOfMedia = false;
	MediaPlayer->OpenSource(MediaSource);
	bSuccess = true;
}

SHIDENCORE_API void UShidenWidget::IsMediaCompleted(bool& bResult) const
{
	bResult = bIsEndOfMedia;
}

SHIDENCORE_API void UShidenWidget::CollapseMedia() const
{
	if (MediaLayer)
	{
		MediaLayer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

SHIDENCORE_API void UShidenWidget::FindRetainerBox(const FString& RetainerBoxName, URetainerBox*& RetainerBox, bool& bResult) const
{
	const TObjectPtr<URetainerBox> Result = RetainerBoxes.FindRef(RetainerBoxName);
	RetainerBox = Result;
	bResult = Result ? true : false;
}

SHIDENCORE_API FString UShidenWidget::MakeMaterialParamsKey(const FString& TargetName, const FString& ParameterName)
{
	const FString EscapedTargetName = TargetName.Replace(TEXT(":"), TEXT("\\:"));
	const FString EscapedParameterName = ParameterName.Replace(TEXT(":"), TEXT("\\:"));
	return FString::Printf(TEXT("%s::%s"), *EscapedTargetName, *EscapedParameterName);
}

SHIDENCORE_API void UShidenWidget::StartImageMaterialScalarChange(const FString& ImageName, UPARAM(ref) UImage* Target,
                                                                  const FName& ParameterName, const EEasingFunc::Type Function,
                                                                  const float Duration, const float EndValue,
                                                                  const float BlendExp, const int32 Steps,
                                                                  const FString& OwnerProcessName, bool& Success,
                                                                  FString& ErrorMessage)
{
	const FString Key = MakeMaterialParamsKey(ImageName, ParameterName.ToString());
	const FShidenImageMaterialScalarParams* Params = ImageMaterialParams.Find(Key);
	if (Params && Params->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(
			TEXT("Process \"%s\" is currently executing the change material scalar parameter command."),
			*Params->OwnerProcessName);
		Success = false;
		return;
	}

	const TObjectPtr<UMaterialInstanceDynamic> Material = Target->GetDynamicMaterial();

	if (!Material)
	{
		ErrorMessage = FString::Printf(TEXT("Material of image \"%s\" is not found."), *ImageName);
		Success = false;
		return;
	}

	if (Duration == 0)
	{
		Material->SetScalarParameterValue(ParameterName, EndValue);
		ImageMaterialParams.Remove(Key);
		Success = true;
		return;
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
	Success = true;
}

SHIDENCORE_API void UShidenWidget::IsImageMaterialParameterChangeCompleted(const FString& ImageName, const FString& ParameterName, bool& bResult) const
{
	const FString Key = MakeMaterialParamsKey(ImageName, ParameterName);
	bResult = !ImageMaterialParams.Contains(Key);
}

SHIDENCORE_API void UShidenWidget::StartRetainerBoxMaterialScalarChange(const FString& RetainerBoxName, UPARAM(ref) URetainerBox* Target,
                                                                        const FName& ParameterName, const EEasingFunc::Type Function,
                                                                        const float Duration, const float EndValue, const float BlendExp, const int32 Steps,
                                                                        const FString& OwnerProcessName, bool& Success, FString& ErrorMessage)
{
	const FString Key = MakeMaterialParamsKey(RetainerBoxName, ParameterName.ToString());
	const FShidenRetainerBoxMaterialScalarParams* Params = RetainerBoxMaterialParams.Find(Key);
	if (Params && Params->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(
			TEXT("Process \"%s\" is currently executing the change material scalar parameter command."),
			*Params->OwnerProcessName);
		Success = false;
		return;
	}

	const TObjectPtr<UMaterialInstanceDynamic> Material = Target->GetEffectMaterial();

	if (!Material)
	{
		ErrorMessage = FString::Printf(TEXT("Material of retainer box \"%s\" is not found."), *RetainerBoxName);
		Success = false;
		return;
	}

	if (Duration == 0)
	{
		Material->SetScalarParameterValue(ParameterName, EndValue);
		RetainerBoxMaterialParams.Remove(Key);
		Success = true;
		return;
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
	Success = true;
}

SHIDENCORE_API void UShidenWidget::IsRetainerBoxMaterialParameterChangeCompleted(const FString& RetainerBoxName, const FString& ParameterName, bool& bResult) const
{
	const FString Key = MakeMaterialParamsKey(RetainerBoxName, ParameterName);
	bResult = !RetainerBoxMaterialParams.Contains(Key);
}

SHIDENCORE_API void UShidenWidget::StartCanvasPanelMove(const FString& CanvasPanelName, UPARAM(ref) UCanvasPanelSlot* Target,
                                                        const EEasingFunc::Type Function, const float Duration,
                                                        const bool bChangePosition, const FVector2D EndPosition,
                                                        const bool bChangeSize, const FVector2D EndSize, const float BlendExp,
                                                        const int32 Steps, const FString& OwnerProcessName, bool& Success, FString& ErrorMessage)
{
	const FShidenCanvasPanelSlotMoveParams* Params = CanvasPanelMoveParams.Find(CanvasPanelName);
	if (Params && Params->OwnerProcessName != OwnerProcessName)
	{
		ErrorMessage = FString::Printf(TEXT("Process \"%s\" is currently executing the move canvas panel command."), *Params->OwnerProcessName);
		Success = false;
		return;
	}

	if (!Target)
	{
		ErrorMessage = TEXT("Canvas panel slot is null.");
		Success = false;
		return;
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
		CanvasPanelMoveParams.Remove(CanvasPanelName);
		Success = true;
		return;
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

	CanvasPanelMoveParams.Add(CanvasPanelName, MoveParams);
	Success = true;
}

SHIDENCORE_API void UShidenWidget::IsCanvasPanelMoveCompleted(const FString& CanvasPanelName, bool& bResult) const
{
	bResult = !CanvasPanelMoveParams.Contains(CanvasPanelName);
}

SHIDENCORE_API void UShidenWidget::FindCanvasPanelSlot(const FString& CanvasPanelName, UCanvasPanelSlot*& CanvasPanelSlot, bool& bResult) const
{
	const TObjectPtr<UCanvasPanelSlot> Result = CanvasPanelSlots.FindRef(CanvasPanelName);
	if (!Result)
	{
		bResult = false;
		return;
	}
	CanvasPanelSlot = Result;
	bResult = true;
}

SHIDENCORE_API void UShidenWidget::FindAnimation(const FString& AnimationName, UUserWidget*& TargetWidget, UWidgetAnimation*& WidgetAnimation, bool& bResult) const
{
	const FShidenAnimation* Result = WidgetAnimations.Find(AnimationName);
	if (!Result)
	{
		bResult = false;
		return;
	}
	WidgetAnimation = Result->Animation;
	TargetWidget = Result->TargetWidget;
	bResult = true;
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
	if (Properties.MaxLength > 0)
	{
		Result = Result.Left(Properties.MaxLength);
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

SHIDENCORE_API void UShidenWidget::IsSkipPressed(bool& bPressed) const
{
	bPressed = bPressSkipButton;
}

SHIDENCORE_API void UShidenWidget::GetVisibilityByName(const FString& Name, ESlateVisibility& Result) const
{
	if (const TObjectPtr<UWidget> Widget = AllWidgets[Name])
	{
		Result = Widget->GetVisibility();
	}
}

SHIDENCORE_API void UShidenWidget::SetVisibilityByName(const FString& Name, const ESlateVisibility InVisibility, const bool bShouldRegisterScenarioProperty, bool& bSuccess)
{
	if (!AllWidgets.Contains(Name))
	{
		bSuccess = false;
		return;
	}
	const TObjectPtr<UWidget> Widget = AllWidgets[Name];
	if (!Widget)
	{
		bSuccess = false;
		return;
	}
	bSuccess = true;
	Widget->SetVisibility(InVisibility);
	if (bShouldRegisterScenarioProperty)
	{
		FString VisibilityStr;

		switch (InVisibility)
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

		UShidenCommandFunctionLibrary::RegisterScenarioProperty(TEXT("ChangeVisibility"), Name, VisibilityStr);
	}
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
				if (const TObjectPtr<UWidgetAnimation> Animation = Cast<UWidgetAnimation>(ObjectProperty->GetObjectPropertyValue_InContainer(TargetWidget)))
				{
					WidgetAnimations.Add(Prefix + Property->GetName(), FShidenAnimation{ const_cast<UUserWidget*>(TargetWidget), Animation });
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

SHIDENCORE_API void UShidenWidget::FindCanvasPanelMoveParams(const FString& CanvasPanelName, FShidenCanvasPanelSlotMoveParams& Value, bool& bSuccess)
{
	const FShidenCanvasPanelSlotMoveParams* Params = CanvasPanelMoveParams.Find(CanvasPanelName);
	if (!Params)
	{
		bSuccess = false;
		return;
	}
	Value = *Params;
	bSuccess = true;
}

SHIDENCORE_API void UShidenWidget::FindImageFadeParams(const FString& ImageName, FShidenImageFadeParams& Value, bool& bSuccess)
{
	const FShidenImageFadeParams* Params = ImageFadeParams.Find(ImageName);
	if (!Params)
	{
		bSuccess = false;
		return;
	}
	Value = *Params;
	bSuccess = true;
}

SHIDENCORE_API void UShidenWidget::FindImageMaterialScalarParams(const FString& MaterialParamsKey, FShidenImageMaterialScalarParams& Value, bool& bSuccess)
{
	const FShidenImageMaterialScalarParams* Params = ImageMaterialParams.Find(MaterialParamsKey);
	if (!Params)
	{
		bSuccess = false;
		return;
	}
	Value = *Params;
	bSuccess = true;
}

SHIDENCORE_API void UShidenWidget::FindRetainerBoxMaterialScalarParams(const FString& MaterialParamsKey, FShidenRetainerBoxMaterialScalarParams& Value, bool& bSuccess)
{
	const FShidenRetainerBoxMaterialScalarParams* Params = RetainerBoxMaterialParams.Find(MaterialParamsKey);
	if (!Params)
	{
		bSuccess = false;
		return;
	}
	Value = *Params;
	bSuccess = true;
}
