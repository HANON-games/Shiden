// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenColorPicker.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"

TSharedRef<SWidget> UShidenColorPicker::RebuildWidget()
{
	ColorBlockWidget = SNew(SColorBlock)
		.Color_Lambda([this]() { return Color; })
		.Size(Size)
		.OnMouseButtonDown_Lambda([this](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
		{
			return HandleColorBlockMouseButtonDown(MyGeometry, MouseEvent);
		});

	return ColorBlockWidget.ToSharedRef();
}

FReply UShidenColorPicker::HandleColorBlockMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FColorPickerArgs PickerArgs;
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnMouseUp = false;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.InitialColor = Color;
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateUObject(this, &UShidenColorPicker::OnColorPickerColorChanged);
		PickerArgs.OnColorPickerCancelled = FOnColorPickerCancelled::CreateUObject(this, &UShidenColorPicker::OnColorPickerCancelled);

		OpenColorPicker(PickerArgs);

		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

void UShidenColorPicker::OnColorPickerColorChanged(const FLinearColor NewColor)
{
	SetColor(NewColor);
}

void UShidenColorPicker::OnColorPickerCancelled(const FLinearColor OriginalColor)
{
	SetColor(OriginalColor);
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

const FText UShidenColorPicker::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}
