// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Fonts/SlateFontInfo.h"
#include "Layout/Margin.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Components/Widget.h"
#include "SSearchableComboBox.h"
#include "ShidenSearchableComboBoxString.generated.h"

UCLASS(meta = (DisplayName = "SearchableComboBox (String)"))
class SHIDENEDITOR_API UShidenSearchableComboBoxString : public UWidget
{
	GENERATED_UCLASS_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectionChangedEvent, FString, SelectedItem, ESelectInfo::Type, SelectionType);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpeningEvent);

private:
	UPROPERTY(EditAnywhere, Category = Content)
	TArray<FString> DefaultOptions;

	UPROPERTY(EditAnywhere, FieldNotify, Category = Content)
	FString SelectedOption;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Style, meta = (DisplayName = "Style"))
	FComboBoxStyle WidgetStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Style)
	FTableRowStyle ItemStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Style)
	FScrollBarStyle ScrollBarStyle;

	UE_DEPRECATED(5.2, "Direct access to ContentPadding is deprecated. Please use the getter or setter.")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, Category = Content)
	FMargin ContentPadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Content, AdvancedDisplay)
	float MaxListHeight;

	UE_DEPRECATED(5.2, "Direct access to HasDownArrow is deprecated. Please use the getter or setter.")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter = "IsHasDownArrow", Setter = "SetHasDownArrow", Category = Content, AdvancedDisplay)
	bool bHasDownArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Style)
	FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Style, meta = (DesignerRebuild))
	FSlateColor ForegroundColor;

public:
	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	FGenerateWidgetForString OnGenerateWidgetEvent;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSelectionChangedEvent OnSelectionChanged;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnOpeningEvent OnOpening;

public:
	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	void AddOption(const FString& Option);

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	bool RemoveOption(const FString& Option);

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	int32 FindOptionIndex(const FString& Option) const;

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	FString GetOptionAtIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	void ClearOptions();

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	void RefreshOptions();

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	void SetSelectedOption(FString Option);

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	void SetSelectedIndex(const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	FString GetSelectedOption() const;

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	int32 GetSelectedIndex() const;

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	int32 GetOptionCount() const;

	UFUNCTION(BlueprintCallable, Category = "ComboBox", Meta = (ReturnDisplayName = "bOpen"))
	bool IsOpen() const;

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	FMargin GetContentPadding() const;

	void SetContentPadding(FMargin InPadding);

	bool IsHasDownArrow() const;

	void SetHasDownArrow(bool bInHasDownArrow);

	//~ Begin UObject Interface
	virtual void PostInitProperties() override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostLoad() override;
	//~ End UObject Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	void UpdateOrGenerateWidget(const TSharedPtr<FString>& Item);

	virtual TSharedRef<SWidget> HandleGenerateWidget(TSharedPtr<FString> Item) const;

	virtual void HandleSelectionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectionType);

	virtual void HandleOpening();

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

protected:
	TArray<TSharedPtr<FString>> Options;

	TSharedPtr<SSearchableComboBox> MyComboBox;

	TSharedPtr<SBox> ComboBoxContent;

	TWeakPtr<STextBlock> DefaultComboBoxContent;

	TSharedPtr<FString> CurrentOptionPtr;
};
