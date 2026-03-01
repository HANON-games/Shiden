// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenSearchableComboBoxString.h"
#include "Widgets/SNullWidget.h"
#include "UObject/EditorObjectVersion.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Styling/DefaultStyleCache.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

UShidenSearchableComboBoxString::UShidenSearchableComboBoxString(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetComboBoxStyle();
	ItemStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetTableRowStyle();

#if WITH_EDITOR
	if (IsEditorWidget())
	{
		WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetComboBoxStyle();
		ItemStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetTableRowStyle();

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif

	ForegroundColor = ItemStyle.TextColor;

	SetContentPadding(FMargin(4.0, 2.0));
	MaxListHeight = 450.0f;
	SetHasDownArrow(true);
	// We don't want to try and load fonts on the server.
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*GetDefaultFontName());
		Font = FSlateFontInfo(RobotoFontObj.Object, 16, FName("Bold"));
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UShidenSearchableComboBoxString::PostInitProperties()
{
	Super::PostInitProperties();

	// Populate Options from DefaultOptions for newly created objects.
	// For objects loaded from disk, PostLoad() is also called and will repopulate Options,
	// so we only add options here if Options is currently empty to avoid duplicates.
	if (Options.IsEmpty())
	{
		for (const FString& DefaultOption : DefaultOptions)
		{
			AddOption(DefaultOption);
		}
	}
}

void UShidenSearchableComboBoxString::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyComboBox.Reset();
	ComboBoxContent.Reset();
}

void UShidenSearchableComboBoxString::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);
}

void UShidenSearchableComboBoxString::PostLoad()
{
	Super::PostLoad();

	// Clear any options added in PostInitProperties and repopulate from the
	// deserialized DefaultOptions. This prevents duplicate entries that would
	// occur because PostInitProperties runs before deserialization and PostLoad
	// runs after, both attempting to add the same DefaultOptions.
	Options.Empty();
	for (const FString& DefaultOption : DefaultOptions)
	{
		AddOption(DefaultOption);
	}
}

TSharedRef<SWidget> UShidenSearchableComboBoxString::RebuildWidget()
{
	const int32 InitialIndex = FindOptionIndex(SelectedOption);
	if (InitialIndex != INDEX_NONE)
	{
		CurrentOptionPtr = Options[InitialIndex];
	}
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	MyComboBox =
		SNew(SSearchableComboBox)
		.ComboBoxStyle(&WidgetStyle)
		.ItemStyle(&ItemStyle)
		.ForegroundColor(ForegroundColor)
		.OptionsSource(&Options)
		.InitiallySelectedItem(CurrentOptionPtr)
		.ContentPadding(GetContentPadding())
		.MaxListHeight(MaxListHeight)
		.HasDownArrow(IsHasDownArrow())
		.OnGenerateWidget(BIND_UOBJECT_DELEGATE(SSearchableComboBox::FOnGenerateWidget, HandleGenerateWidget))
		.OnSelectionChanged(BIND_UOBJECT_DELEGATE(SSearchableComboBox::FOnSelectionChanged, HandleSelectionChanged))
		.OnComboBoxOpening(BIND_UOBJECT_DELEGATE(FOnComboBoxOpening, HandleOpening))
		[
			SAssignNew(ComboBoxContent, SBox)
		];
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	if (InitialIndex != INDEX_NONE)
	{
		// Generate the widget for the initially selected widget if needed
		UpdateOrGenerateWidget(CurrentOptionPtr);
	}

	return MyComboBox.ToSharedRef();
}

void UShidenSearchableComboBoxString::AddOption(const FString& Option)
{
	Options.Add(MakeShareable(new FString(Option)));

	RefreshOptions();
}

bool UShidenSearchableComboBoxString::TryRemoveOption(const FString& Option)
{
	const int32 OptionIndex = FindOptionIndex(Option);

	if (OptionIndex != INDEX_NONE)
	{
		if (Options[OptionIndex] == CurrentOptionPtr)
		{
			ClearSelection();
		}

		Options.RemoveAt(OptionIndex);

		RefreshOptions();

		return true;
	}

	return false;
}

int32 UShidenSearchableComboBoxString::FindOptionIndex(const FString& Option) const
{
	for (int32 OptionIndex = 0; OptionIndex < Options.Num(); OptionIndex++)
	{
		const TSharedPtr<FString>& OptionAtIndex = Options[OptionIndex];

		if (OptionAtIndex.IsValid() && *OptionAtIndex == Option)
		{
			return OptionIndex;
		}
	}

	return INDEX_NONE;
}

FString UShidenSearchableComboBoxString::GetOptionAtIndex(const int32 Index) const
{
	if (Index >= 0 && Index < Options.Num() && Options[Index].IsValid())
	{
		return *Options[Index];
	}
	return FString();
}

void UShidenSearchableComboBoxString::ClearOptions()
{
	ClearSelection();

	Options.Empty();

	if (MyComboBox.IsValid())
	{
		MyComboBox->RefreshOptions();
	}
}

void UShidenSearchableComboBoxString::ClearSelection()
{
	CurrentOptionPtr.Reset();
	SelectedOption.Reset();

	if (MyComboBox.IsValid())
	{
		MyComboBox->ClearSelection();
	}

	if (ComboBoxContent.IsValid())
	{
		ComboBoxContent->SetContent(SNullWidget::NullWidget);
	}
}

void UShidenSearchableComboBoxString::RefreshOptions() const
{
	if (MyComboBox.IsValid())
	{
		MyComboBox->RefreshOptions();
	}
}

void UShidenSearchableComboBoxString::SetSelectedOption(const FString Option)
{
	const int32 InitialIndex = FindOptionIndex(Option);
	SetSelectedIndex(InitialIndex);
}

void UShidenSearchableComboBoxString::SetSelectedIndex(const int32 Index)
{
	if (Options.IsValidIndex(Index))
	{
		CurrentOptionPtr = Options[Index];
		// Don't select item if its already selected
		if (SelectedOption != *CurrentOptionPtr)
		{
			BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::SelectedOption);
			SelectedOption = *CurrentOptionPtr;

			if (ComboBoxContent.IsValid())
			{
				MyComboBox->SetSelectedItem(CurrentOptionPtr);
				UpdateOrGenerateWidget(CurrentOptionPtr);
			}
			else
			{
				HandleSelectionChanged(CurrentOptionPtr, ESelectInfo::Direct);
			}
		}
	}
}

FString UShidenSearchableComboBoxString::GetSelectedOption() const
{
	if (CurrentOptionPtr.IsValid())
	{
		return *CurrentOptionPtr;
	}
	return FString();
}

int32 UShidenSearchableComboBoxString::GetSelectedIndex() const
{
	if (CurrentOptionPtr.IsValid())
	{
		for (int32 OptionIndex = 0; OptionIndex < Options.Num(); ++OptionIndex)
		{
			if (Options[OptionIndex] == CurrentOptionPtr)
			{
				return OptionIndex;
			}
		}
	}

	return INDEX_NONE;
}

int32 UShidenSearchableComboBoxString::OptionNum() const
{
	return Options.Num();
}

bool UShidenSearchableComboBoxString::IsOpen() const
{
	return MyComboBox.IsValid() && MyComboBox->IsOpen();
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void UShidenSearchableComboBoxString::SetContentPadding(const FMargin InPadding)
{
	// ReSharper disable once CppDeprecatedEntity
	ContentPadding = InPadding;
	if (MyComboBox.IsValid())
	{
		MyComboBox->SetButtonContentPadding(InPadding);
	}
}

FMargin UShidenSearchableComboBoxString::GetContentPadding() const
{
	// ReSharper disable once CppDeprecatedEntity
	return ContentPadding;
}

bool UShidenSearchableComboBoxString::IsHasDownArrow() const
{
	// ReSharper disable once CppDeprecatedEntity
	return bHasDownArrow;
}

void UShidenSearchableComboBoxString::SetHasDownArrow(const bool bInHasDownArrow)
{
	// ReSharper disable once CppDeprecatedEntity
	bHasDownArrow = bInHasDownArrow;
	if (MyComboBox.IsValid())
	{
		// ReSharper disable once CppDeprecatedEntity
		MyComboBox->SetHasDownArrow(bHasDownArrow);
	}
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS

void UShidenSearchableComboBoxString::UpdateOrGenerateWidget(const TSharedPtr<FString>& Item)
{
	// If no custom widget was supplied and the default STextBlock already exists,
	// just update its text instead of rebuilding the widget.
	if (DefaultComboBoxContent.IsValid() && (IsDesignTime() || OnGenerateWidgetEvent.IsBound()))
	{
		const FString StringItem = Item.IsValid() ? *Item : FString();
		DefaultComboBoxContent.Pin()->SetText(FText::FromString(StringItem));
	}
	else
	{
		DefaultComboBoxContent.Reset();
		if (ComboBoxContent.IsValid())
		{
			ComboBoxContent->SetContent(HandleGenerateWidget(Item));
		}
	}
}

TSharedRef<SWidget> UShidenSearchableComboBoxString::HandleGenerateWidget(const TSharedPtr<FString> Item) const
{
	const FString StringItem = Item.IsValid() ? *Item : FString();

	// Call the user's delegate to see if they want to generate a custom widget bound to the data source.
	if (!IsDesignTime() && OnGenerateWidgetEvent.IsBound())
	{
		const TObjectPtr<UWidget> Widget = OnGenerateWidgetEvent.Execute(StringItem);
		if (Widget != nullptr)
		{
			return Widget->TakeWidget();
		}
	}

	// If a row wasn't generated just create the default one, a simple text block of the item's name.
	return SNew(STextBlock)
		.Text(FText::FromString(StringItem))
		.Font(Font);
}

void UShidenSearchableComboBoxString::HandleSelectionChanged(const TSharedPtr<FString> Item, const ESelectInfo::Type SelectionType)
{
	CurrentOptionPtr = Item;
	BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::SelectedOption);
	SelectedOption = CurrentOptionPtr.IsValid() ? CurrentOptionPtr.ToSharedRef().Get() : FString();

	// When the selection changes we always generate another widget to represent the content area of the combobox.
	if (ComboBoxContent.IsValid())
	{
		UpdateOrGenerateWidget(CurrentOptionPtr);
	}

	if (!IsDesignTime())
	{
		OnSelectionChanged.Broadcast(Item.IsValid() ? *Item : FString(), SelectionType);
	}
}

void UShidenSearchableComboBoxString::HandleOpening()
{
	OnOpening.Broadcast();
}

#if WITH_EDITOR

const FText UShidenSearchableComboBoxString::GetPaletteCategory()
{
	return LOCTEXT("Shiden Editor", "Shiden Editor");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
