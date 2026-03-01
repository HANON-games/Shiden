// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Components/PanelWidget.h"
#include "Widgets/Layout/SSplitter.h"
#include "ShidenSplitterBox.generated.h"

class UShidenSplitterBoxSlot;

/**
 * Orientation of the splitter
 */
UENUM(BlueprintType)
enum class EShidenSplitterOrientation : uint8
{
	Horizontal UMETA(DisplayName = "Horizontal"),
	Vertical UMETA(DisplayName = "Vertical")
};

/**
 * A splitter widget that divides its area into resizable sections.
 * Users can drag the splitter handles to resize sections at runtime.
 */
UCLASS()
class SHIDENEDITOR_API UShidenSplitterBox : public UPanelWidget
{
	GENERATED_UCLASS_BODY()

	/** The orientation of the splitter (Horizontal or Vertical) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	EShidenSplitterOrientation Orientation;

	/** The thickness of the splitter handle in slate units */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	float SplitterHandleSize;

	/**
	 * Unique identifier for saving/restoring the splitter layout.
	 * If set, the splitter sizes will be saved to config when resized
	 * and restored when the widget is constructed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout Persistence")
	FString LayoutId;

	/**
	 * Whether to persist the layout (auto-save on resize and restore on construct).
	 * Requires LayoutId to be set.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout Persistence")
	bool bPersistLayout;

	/**
	 * Adds a new child widget to the splitter.
	 * @param Content The widget to add
	 * @return The slot for the added widget
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	UShidenSplitterBoxSlot* AddChildToSplitter(UWidget* Content);

	/**
	 * Gets the number of slots in the splitter
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	int32 GetSlotCount() const;

	/**
	 * Sets the orientation of the splitter
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetOrientation(EShidenSplitterOrientation InOrientation);

	/**
	 * Sets the splitter handle size
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetSplitterHandleSize(float InSize);

	/**
	 * Saves the current splitter layout to config.
	 * Requires LayoutId to be set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SaveLayout() const;

	/**
	 * Restores the splitter layout from config.
	 * Requires LayoutId to be set.
	 * @return True if the layout was restored successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool RestoreLayout();

	/**
	 * Gets the current size values of all slots.
	 * @return Array of size values for each slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	TArray<float> GetSlotSizeValues() const;

	/**
	 * Sets the size values for all slots.
	 * @param SizeValues Array of size values to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetSlotSizeValues(const TArray<float>& SizeValues);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	//~ Begin UPanelWidget Interface
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	//~ End UPanelWidget Interface

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	TSharedPtr<SSplitter> MySplitter;

private:
	/** Called when the splitter handle finishes resizing */
	void HandleSplitterResized() const;

	/** Gets the config section name for layout persistence */
	FString GetLayoutConfigSection() const;

	/** Gets the config key for a specific slot */
	static FString GetSlotConfigKey(int32 SlotIndex);
};

/**
 * Slot for the Splitter Box widget
 */
UCLASS()
class SHIDENEDITOR_API UShidenSplitterBoxSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

	/**
	 * The default size value for this slot.
	 * - For Fixed mode: absolute size in slate units
	 * - For Fill mode: weight relative to other Fill slots
	 * - For Fraction mode: fraction of total size (0.0 to 1.0)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float DefaultSizeValue;

	/**
	 * Whether this slot is collapsed. When collapsed, the slot size is set to 0.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	bool bIsCollapsed;
	
	/**
	 * Sets the default size value for this slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Layout")
	void SetDefaultSizeValue(float InDefaultSizeValue);
	
	/**
	 * Sets the size value for this slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Layout")
	void SetSizeValue(float InSizeValue);

	/**
	 * Sets whether this slot is collapsed
	 */
	UFUNCTION(BlueprintCallable, Category = "Layout")
	void SetIsCollapsed(bool bInIsCollapsed);

	//~ Begin UPanelSlot Interface
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UPanelSlot Interface

	virtual void BuildSlot(TSharedRef<SSplitter> InSplitter);
	virtual void ReleaseSlateSlot();

private:
	TWeakPtr<SSplitter> Splitter;
	TSharedPtr<SBox> ContentBox;
	int32 SlotIndex;
	float SizeValue;

	SSplitter::FSlot* GetSlot() const;
};
