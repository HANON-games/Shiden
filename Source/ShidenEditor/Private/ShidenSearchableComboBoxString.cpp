// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenSearchableComboBoxString.h"
#include "Widgets/SNullWidget.h"
#include "UObject/EditorObjectVersion.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Styling/DefaultStyleCache.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

SHIDENEDITOR_API UShidenSearchableComboBoxString::UShidenSearchableComboBoxString(const FObjectInitializer& ObjectInitializer)
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

SHIDENEDITOR_API void UShidenSearchableComboBoxString::PostInitProperties()
{
	Super::PostInitProperties();

	// Initialize the set of options from the default set only once.
	for (const FString& DefaultOption : DefaultOptions)
	{
		AddOption(DefaultOption);
	}
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyComboBox.Reset();
	ComboBoxContent.Reset();
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::PostLoad()
{
	Super::PostLoad();

	// Initialize the set of options from the default set only once.
	for (const FString& DefaultOption : DefaultOptions)
	{
		AddOption(DefaultOption);
	}
}

SHIDENEDITOR_API TSharedRef<SWidget> UShidenSearchableComboBoxString::RebuildWidget()
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

SHIDENEDITOR_API void UShidenSearchableComboBoxString::AddOption(const FString& Option)
{
	Options.Add(MakeShareable(new FString(Option)));

	RefreshOptions();
}

SHIDENEDITOR_API bool UShidenSearchableComboBoxString::RemoveOption(const FString& Option)
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

SHIDENEDITOR_API int32 UShidenSearchableComboBoxString::FindOptionIndex(const FString& Option) const
{
	for (int32 OptionIndex = 0; OptionIndex < Options.Num(); OptionIndex++)
	{
		const TSharedPtr<FString>& OptionAtIndex = Options[OptionIndex];

		if (*OptionAtIndex == Option)
		{
			return OptionIndex;
		}
	}

	return INDEX_NONE;
}

SHIDENEDITOR_API FString UShidenSearchableComboBoxString::GetOptionAtIndex(const int32 Index) const
{
	if (Index >= 0 && Index < Options.Num())
	{
		return *Options[Index];
	}
	return FString();
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::ClearOptions()
{
	ClearSelection();

	Options.Empty();

	if (MyComboBox.IsValid())
	{
		MyComboBox->RefreshOptions();
	}
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::ClearSelection()
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

SHIDENEDITOR_API void UShidenSearchableComboBoxString::RefreshOptions() const
{
	if (MyComboBox.IsValid())
	{
		MyComboBox->RefreshOptions();
	}
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::SetSelectedOption(const FString Option)
{
	const int32 InitialIndex = FindOptionIndex(Option);
	SetSelectedIndex(InitialIndex);
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::SetSelectedIndex(const int32 Index)
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

SHIDENEDITOR_API FString UShidenSearchableComboBoxString::GetSelectedOption() const
{
	if (CurrentOptionPtr.IsValid())
	{
		return *CurrentOptionPtr;
	}
	return FString();
}

SHIDENEDITOR_API int32 UShidenSearchableComboBoxString::GetSelectedIndex() const
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

SHIDENEDITOR_API int32 UShidenSearchableComboBoxString::OptionNum() const
{
	return Options.Num();
}

SHIDENEDITOR_API bool UShidenSearchableComboBoxString::IsOpen() const
{
	return MyComboBox.IsValid() && MyComboBox->IsOpen();
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS

SHIDENEDITOR_API void UShidenSearchableComboBoxString::SetContentPadding(const FMargin InPadding)
{
	// ReSharper disable once CppDeprecatedEntity
	ContentPadding = InPadding;
	if (MyComboBox.IsValid())
	{
		MyComboBox->SetButtonContentPadding(InPadding);
	}
}

SHIDENEDITOR_API FMargin UShidenSearchableComboBoxString::GetContentPadding() const
{
	// ReSharper disable once CppDeprecatedEntity
	return ContentPadding;
}

SHIDENEDITOR_API bool UShidenSearchableComboBoxString::IsHasDownArrow() const
{
	// ReSharper disable once CppDeprecatedEntity
	return bHasDownArrow;
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::SetHasDownArrow(const bool bInHasDownArrow)
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

SHIDENEDITOR_API void UShidenSearchableComboBoxString::UpdateOrGenerateWidget(const TSharedPtr<FString>& Item)
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

SHIDENEDITOR_API TSharedRef<SWidget> UShidenSearchableComboBoxString::HandleGenerateWidget(const TSharedPtr<FString> Item) const
{
	const FString StringItem = Item.IsValid() ? *Item : FString();

	// Call the user's delegate to see if they want to generate a custom widget bound to the data source.
	if (!IsDesignTime() && OnGenerateWidgetEvent.IsBound())
	{
		const TObjectPtr<UWidget> Widget = OnGenerateWidgetEvent.Execute(StringItem);
		if (Widget != NULL)
		{
			return Widget->TakeWidget();
		}
	}

	// If a row wasn't generated just create the default one, a simple text block of the item's name.
	return SNew(STextBlock)
		.Text(FText::FromString(StringItem))
		.Font(Font);
}

SHIDENEDITOR_API void UShidenSearchableComboBoxString::HandleSelectionChanged(const TSharedPtr<FString> Item, const ESelectInfo::Type SelectionType)
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

SHIDENEDITOR_API void UShidenSearchableComboBoxString::HandleOpening()
{
	OnOpening.Broadcast();
}

#if WITH_EDITOR

SHIDENEDITOR_API const FText UShidenSearchableComboBoxString::GetPaletteCategory()
{
	return LOCTEXT("Shiden Editor", "Shiden Editor");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
