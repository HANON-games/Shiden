// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenColorPicker.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"

TSharedRef<SWidget> UShidenColorPicker::RebuildWidget()
{
	ColorBlockWidget = SNew(SColorBlock)
		.Color_Lambda([this] { return Color; })
		.Size(Size)
		.OnMouseButtonDown_Lambda([this](const FGeometry&, const FPointerEvent& MouseEvent)
		{
			return HandleColorBlockMouseButtonDown(MouseEvent);
		});

	return ColorBlockWidget.ToSharedRef();
}

FReply UShidenColorPicker::HandleColorBlockMouseButtonDown(const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FColorPickerArgs PickerArgs;
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnMouseUp = false;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.InitialColor = Color;
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateUObject(this, &UShidenColorPicker::HandleColorPickerColorChanged);
		PickerArgs.OnColorPickerCancelled = FOnColorPickerCancelled::CreateUObject(this, &UShidenColorPicker::HandleColorPickerCancelled);
		PickerArgs.OnColorPickerWindowClosed = FOnWindowClosed::CreateUObject(this, &UShidenColorPicker::HandleColorPickerWindowClosed);
		PickerArgs.OnInteractivePickBegin = FSimpleDelegate::CreateUObject(this, &UShidenColorPicker::HandleInteractivePickBegin);
		PickerArgs.OnInteractivePickEnd = FSimpleDelegate::CreateUObject(this, &UShidenColorPicker::HandleInteractivePickEnd);

		OpenColorPicker(PickerArgs);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UShidenColorPicker::HandleColorPickerColorChanged(const FLinearColor NewColor)
{
	SetColor(NewColor);
}

void UShidenColorPicker::HandleColorPickerCancelled(const FLinearColor OriginalColor)
{
	SetColor(OriginalColor);
}

void UShidenColorPicker::HandleColorPickerWindowClosed(const TSharedRef<SWindow>&) const
{
	OnColorPickerWindowClosed.Broadcast();
}

void UShidenColorPicker::HandleInteractivePickBegin()
{
	bIsPicking = true;
	OnInteractivePickBegin.Broadcast();
}

void UShidenColorPicker::HandleInteractivePickEnd()
{
	bIsPicking = false;
	OnInteractivePickEnd.Broadcast();
}

void UShidenColorPicker::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ColorBlockWidget.Reset();
}

void UShidenColorPicker::SetColor(const FLinearColor NewColor)
{
	Color = NewColor;
	OnColorChanged.Broadcast(NewColor);
}

FLinearColor UShidenColorPicker::GetColor() const
{
	return Color;
}

bool UShidenColorPicker::IsPicking() const
{
	return bIsPicking;
}

const FText UShidenColorPicker::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}
