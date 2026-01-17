// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Components/Widget.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/SWindow.h"
#include "ShidenColorPicker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChangedDelegate, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnColorPickerWindowClosedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractivePickBeginDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractivePickEndDelegate);

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

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnColorPickerWindowClosedDelegate OnColorPickerWindowClosed;

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnInteractivePickBeginDelegate OnInteractivePickBegin;

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnInteractivePickEndDelegate OnInteractivePickEnd;

	UFUNCTION(BlueprintCallable, Category = "Color Preview")
	void SetColor(const FLinearColor NewColor);

	UFUNCTION(BlueprintCallable, Category = "Color Preview")
	FLinearColor GetColor() const;

	UFUNCTION(BlueprintCallable, Category = "Color Preview")
	bool IsPicking() const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual const FText GetPaletteCategory() override;

private:
	bool bIsPicking = false;
	
	TSharedPtr<SColorBlock> ColorBlockWidget;

	FReply HandleColorBlockMouseButtonDown(const FPointerEvent& MouseEvent);
	
	void HandleColorPickerColorChanged(FLinearColor NewColor);

	void HandleColorPickerCancelled(FLinearColor OriginalColor);

	void HandleColorPickerWindowClosed(const TSharedRef<SWindow>& Window) const;

	void HandleInteractivePickBegin();

	void HandleInteractivePickEnd();
};
