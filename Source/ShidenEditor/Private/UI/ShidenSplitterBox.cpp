// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenSplitterBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Components/PanelSlot.h"
#include "Misc/ConfigCacheIni.h"

#define LOCTEXT_NAMESPACE "ShidenSplitterBox"

//////////////////////////////////////////////////////////////////////////
// UShidenSplitterBox

namespace ShidenSplitterBox
{
	const FString ConfigSection = TEXT("ShidenSplitterLayouts");
}

UShidenSplitterBox::UShidenSplitterBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Orientation(EShidenSplitterOrientation::Horizontal)
	, SplitterHandleSize(6.0f)
	, LayoutId(TEXT(""))
	, bPersistLayout(true)
{
	// Use Visible to ensure the splitter handles can receive hit tests
	SetVisibilityInternal(ESlateVisibility::Visible);
}

TSharedRef<SWidget> UShidenSplitterBox::RebuildWidget()
{
	MySplitter = SNew(SSplitter)
		.Orientation(Orientation == EShidenSplitterOrientation::Horizontal ? Orient_Horizontal : Orient_Vertical)
		.PhysicalSplitterHandleSize(SplitterHandleSize)
		.HitDetectionSplitterHandleSize(SplitterHandleSize)
		.OnSplitterFinishedResizing(FSimpleDelegate::CreateUObject(this, &UShidenSplitterBox::HandleSplitterResized));

	for (TObjectPtr PanelSlot : Slots)
	{
		if (UShidenSplitterBoxSlot* TypedSlot = Cast<UShidenSplitterBoxSlot>(PanelSlot))
		{
			TypedSlot->BuildSlot(MySplitter.ToSharedRef());
		}
	}

	// Restore layout after all slots have been built
	if (bPersistLayout && !LayoutId.IsEmpty())
	{
		RestoreLayout();
	}

	return MySplitter.ToSharedRef();
}

void UShidenSplitterBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MySplitter.IsValid())
	{
		MySplitter->SetOrientation(Orientation == EShidenSplitterOrientation::Horizontal ? Orient_Horizontal : Orient_Vertical);
	}
}

void UShidenSplitterBox::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MySplitter.Reset();
}

UClass* UShidenSplitterBox::GetSlotClass() const
{
	return UShidenSplitterBoxSlot::StaticClass();
}

void UShidenSplitterBox::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MySplitter.IsValid())
	{
		if (const TObjectPtr<UShidenSplitterBoxSlot> TypedSlot = Cast<UShidenSplitterBoxSlot>(InSlot))
		{
			TypedSlot->BuildSlot(MySplitter.ToSharedRef());
		}
	}
}

void UShidenSplitterBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (MySplitter.IsValid())
	{
		if (const TObjectPtr<UShidenSplitterBoxSlot>  TypedSlot = Cast<UShidenSplitterBoxSlot>(InSlot))
		{
			TypedSlot->ReleaseSlateSlot();
		}
	}
}

UShidenSplitterBoxSlot* UShidenSplitterBox::AddChildToSplitter(UWidget* Content)
{
	return Cast<UShidenSplitterBoxSlot>(AddChild(Content));
}

int32 UShidenSplitterBox::GetSlotCount() const
{
	return Slots.Num();
}

void UShidenSplitterBox::SetOrientation(const EShidenSplitterOrientation InOrientation)
{
	Orientation = InOrientation;
	if (MySplitter.IsValid())
	{
		MySplitter->SetOrientation(Orientation == EShidenSplitterOrientation::Horizontal ? Orient_Horizontal : Orient_Vertical);
	}
}

void UShidenSplitterBox::SetSplitterHandleSize(const float InSize)
{
	SplitterHandleSize = InSize;
}

void UShidenSplitterBox::SaveLayout() const
{
	if (LayoutId.IsEmpty() || !MySplitter.IsValid())
	{
		return;
	}

	const FString ConfigSectionName = GetLayoutConfigSection();
	const FChildren* Children = MySplitter->GetChildren();
	if (!Children)
	{
		return;
	}

	// Save the number of slots
	GConfig->SetInt(*ConfigSectionName, TEXT("SlotCount"), Children->Num(), GEditorPerProjectIni);

	// Save each slot's size value
	for (int32 SlotIndex = 0; SlotIndex < Children->Num(); ++SlotIndex)
	{
		const SSplitter::FSlot& SplitterSlot = MySplitter->SlotAt(SlotIndex);
		const FString ConfigKey = GetSlotConfigKey(SlotIndex);
		GConfig->SetFloat(*ConfigSectionName, *ConfigKey, SplitterSlot.GetSizeValue(), GEditorPerProjectIni);
	}

	GConfig->Flush(false, GEditorPerProjectIni);
}

bool UShidenSplitterBox::RestoreLayout()
{
	if (LayoutId.IsEmpty() || !MySplitter.IsValid())
	{
		return false;
	}

	const FString ConfigSectionName = GetLayoutConfigSection();
	const FChildren* Children = MySplitter->GetChildren();
	if (!Children)
	{
		return false;
	}

	// Check if we have saved data
	int32 SavedSlotCount = 0;
	if (!GConfig->GetInt(*ConfigSectionName, TEXT("SlotCount"), SavedSlotCount, GEditorPerProjectIni))
	{
		return false;
	}

	// Only restore if the slot count matches
	if (SavedSlotCount != Children->Num())
	{
		return false;
	}

	// Restore each slot's size value
	for (int32 SlotIndex = 0; SlotIndex < Children->Num(); ++SlotIndex)
	{
		const FString ConfigKey = GetSlotConfigKey(SlotIndex);
		float SizeValue;
		if (GConfig->GetFloat(*ConfigSectionName, *ConfigKey, SizeValue, GEditorPerProjectIni))
		{
			SSplitter::FSlot& SplitterSlot = MySplitter->SlotAt(SlotIndex);
			SplitterSlot.SetSizeValue(SizeValue);

			// Also update the UMG slot if available
			if (SlotIndex < Slots.Num())
			{
				if (const TObjectPtr<UShidenSplitterBoxSlot> TypedSlot = Cast<UShidenSplitterBoxSlot>(Slots[SlotIndex]))
				{
					TypedSlot->SetSizeValue(SizeValue);
				}
			}
		}
	}

	return true;
}

TArray<float> UShidenSplitterBox::GetSlotSizeValues() const
{
	TArray<float> SizeValues;

	if (!MySplitter.IsValid())
	{
		return SizeValues;
	}

	const FChildren* Children = MySplitter->GetChildren();
	if (!Children)
	{
		return SizeValues;
	}

	SizeValues.Reserve(Children->Num());
	for (int32 SlotIndex = 0; SlotIndex < Children->Num(); ++SlotIndex)
	{
		const SSplitter::FSlot& SplitterSlot = MySplitter->SlotAt(SlotIndex);
		SizeValues.Add(SplitterSlot.GetSizeValue());
	}

	return SizeValues;
}

void UShidenSplitterBox::SetSlotSizeValues(const TArray<float>& SizeValues)
{
	if (!MySplitter.IsValid())
	{
		return;
	}

	const FChildren* Children = MySplitter->GetChildren();
	if (!Children)
	{
		return;
	}

	const int32 NumToSet = FMath::Min(SizeValues.Num(), Children->Num());
	for (int32 SlotIndex = 0; SlotIndex < NumToSet; ++SlotIndex)
	{
		SSplitter::FSlot& SplitterSlot = MySplitter->SlotAt(SlotIndex);
		SplitterSlot.SetSizeValue(SizeValues[SlotIndex]);

		// Also update the UMG slot if available
		if (SlotIndex < Slots.Num())
		{
			if (UShidenSplitterBoxSlot* TypedSlot = Cast<UShidenSplitterBoxSlot>(Slots[SlotIndex]))
			{
				TypedSlot->SetSizeValue(SizeValues[SlotIndex]);
			}
		}
	}
}

void UShidenSplitterBox::HandleSplitterResized() const
{
	if (bPersistLayout && !LayoutId.IsEmpty())
	{
		SaveLayout();
	}
}

FString UShidenSplitterBox::GetLayoutConfigSection() const
{
	return FString::Printf(TEXT("%s.%s"), *ShidenSplitterBox::ConfigSection, *LayoutId);
}

FString UShidenSplitterBox::GetSlotConfigKey(const int32 SlotIndex)
{
	return FString::Printf(TEXT("Slot%d"), SlotIndex);
}

#if WITH_EDITOR

const FText UShidenSplitterBox::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}

#endif

//////////////////////////////////////////////////////////////////////////
// UShidenSplitterBoxSlot

UShidenSplitterBoxSlot::UShidenSplitterBoxSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, DefaultSizeValue(1.0f)
	, bIsCollapsed(false)
	, SlotIndex(INDEX_NONE)
{
}

void UShidenSplitterBoxSlot::BuildSlot(const TSharedRef<SSplitter> InSplitter)
{
	Splitter = InSplitter;

	// Store the index before adding the slot
	SlotIndex = InSplitter->GetChildren()->Num();

	// Add the slot (we don't store a pointer, we use the index)
	// Wrap content in SBox with clipping to prevent rendering outside the slot bounds
	// If collapsed, set size to 0 and visibility to Collapsed
	SizeValue = DefaultSizeValue;
	const float ActualSize = bIsCollapsed ? 0.0f : SizeValue;
	const EVisibility SlotVisibility = bIsCollapsed ? EVisibility::Collapsed : EVisibility::SelfHitTestInvisible;

	ContentBox = SNew(SBox)
		.Clipping(EWidgetClipping::ClipToBounds)
		.Visibility(SlotVisibility)
		[
			Content ? Content->TakeWidget() : SNullWidget::NullWidget
		];

	InSplitter->AddSlot()
		.Value(ActualSize)
		.SizeRule(SSplitter::FractionOfParent)
		[
			ContentBox.ToSharedRef()
		];
}

SSplitter::FSlot* UShidenSplitterBoxSlot::GetSlot() const
{
	if (SlotIndex == INDEX_NONE || !Splitter.IsValid())
	{
		return nullptr;
	}

	const TSharedPtr<SSplitter> SplitterPtr = Splitter.Pin();
	if (!SplitterPtr.IsValid())
	{
		return nullptr;
	}

	const FChildren* Children = SplitterPtr->GetChildren();
	if (!Children || SlotIndex >= Children->Num())
	{
		return nullptr;
	}

	// Cast from const FSlotBase to SSplitter::FSlot
	// We need const_cast because GetSlotAt returns const, but we need to modify the slot
	FSlotBase& SlotBase = const_cast<FSlotBase&>(Children->GetSlotAt(SlotIndex));
	return &static_cast<SSplitter::FSlot&>(SlotBase);
}

void UShidenSplitterBoxSlot::ReleaseSlateSlot()
{
	SlotIndex = INDEX_NONE;
	Splitter.Reset();
	ContentBox.Reset();
}

void UShidenSplitterBoxSlot::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ContentBox.Reset();
}

void UShidenSplitterBoxSlot::SynchronizeProperties()
{
	if (SSplitter::FSlot* Slot = GetSlot())
	{
		Slot->SetSizingRule(SSplitter::FractionOfParent);
		// If collapsed, set size to 0
		const float ActualSize = bIsCollapsed ? 0.0f : SizeValue;
		Slot->SetSizeValue(ActualSize);
	}
}

void UShidenSplitterBoxSlot::SetDefaultSizeValue(const float InDefaultSizeValue)
{
	DefaultSizeValue = InDefaultSizeValue;
}

void UShidenSplitterBoxSlot::SetSizeValue(const float InSizeValue)
{
	SizeValue = InSizeValue;
	if (SSplitter::FSlot* Slot = GetSlot())
	{
		// If collapsed, keep size at 0
		const float ActualSize = bIsCollapsed ? 0.0f : SizeValue;
		Slot->SetSizeValue(ActualSize);
	}
}

void UShidenSplitterBoxSlot::SetIsCollapsed(const bool bInIsCollapsed)
{
	bIsCollapsed = bInIsCollapsed;

	// Update slot size
	if (SSplitter::FSlot* Slot = GetSlot())
	{
		// If collapsed, set size to 0, otherwise use SizeValue
		const float ActualSize = bIsCollapsed ? 0.0f : SizeValue;
		Slot->SetSizeValue(ActualSize);
	}

	// Update content box visibility
	if (ContentBox.IsValid())
	{
		const EVisibility SlotVisibility = bIsCollapsed ? EVisibility::Collapsed : EVisibility::SelfHitTestInvisible;
		ContentBox->SetVisibility(SlotVisibility);
	}
}

#undef LOCTEXT_NAMESPACE
