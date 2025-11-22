// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Components/Widget.h"
#include "Widgets/Colors/SColorBlock.h"
#include "ShidenColorPicker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChangedDelegate, FLinearColor, NewColor);

UCLASS(BlueprintType)
class SHIDENEDITOR_API UShidenColorPicker : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color Preview")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color Preview")
	FVector2D Size = FVector2D(20.0f, 20.0f);

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnColorChangedDelegate OnColorChanged;

	UFUNCTION(BlueprintCallable, Category = "Color Preview")
	void SetColor(const FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "Color Preview")
	FLinearColor GetColor() const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual const FText GetPaletteCategory() override;

private:
	TSharedPtr<SColorBlock> ColorBlockWidget;

	FReply HandleColorBlockMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	
	void OnColorPickerColorChanged(FLinearColor NewColor);

	void OnColorPickerCancelled(FLinearColor OriginalColor);
};
