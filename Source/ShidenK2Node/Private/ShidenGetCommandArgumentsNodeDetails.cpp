// Copyright (c) 2026 HANON. All Rights Reserved.

#include "ShidenGetCommandArgumentsNodeDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "K2Node_GetCommandArguments.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ShidenGetCommandArgumentsNodeDetails"

TSharedRef<IDetailCustomization> FShidenGetCommandArgumentsNodeDetails::MakeInstance()
{
	return MakeShared<FShidenGetCommandArgumentsNodeDetails>();
}

void FShidenGetCommandArgumentsNodeDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	const TArray<TWeakObjectPtr<UObject>>& SelectedObjects = DetailBuilder.GetSelectedObjects();
	if (SelectedObjects.Num() != 1)
	{
		return;
	}

	TargetNode = Cast<UK2Node_GetCommandArguments>(SelectedObjects[0].Get());
	if (!TargetNode.IsValid())
	{
		return;
	}

	IDetailCategoryBuilder& DetailCategory = DetailBuilder.EditCategory("PinOptions");
	TSharedRef<IPropertyHandle> AvailablePins = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UK2Node_GetCommandArguments, ShowPinForProperties));

	constexpr bool bGenerateHeader = true;
	constexpr bool bDisplayResetToDefault = false;
	constexpr bool bDisplayElementNum = false;
	constexpr bool bForAdvanced = false;
	TSharedRef<FDetailArrayBuilder> AvailablePinsBuilder = MakeShared<FDetailArrayBuilder>(
		AvailablePins,
		bGenerateHeader,
		bDisplayResetToDefault,
		bDisplayElementNum);
	AvailablePinsBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(
		this,
		&FShidenGetCommandArgumentsNodeDetails::OnGenerateElementForPropertyPin));
	AvailablePinsBuilder->SetDisplayName(LOCTEXT("Pins", "Pins"));
	DetailCategory.AddCustomBuilder(AvailablePinsBuilder, bForAdvanced);

	DetailCategory.AddCustomRow(LOCTEXT("PinActionsSearchText", "Pin Actions"))
	.ValueContent()
	.HAlign(HAlign_Left)
	.MaxDesiredWidth(500.0f)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.OnClicked(this, &FShidenGetCommandArgumentsNodeDetails::HideUnconnectedPins)
			.IsEnabled_Lambda([this]()
			{
				return TargetNode.IsValid() && TargetNode->CanHideUnconnectedOutputPins();
			})
			.ToolTipText(LOCTEXT("HideUnconnectedPinsTooltip", "All unconnected output pins get hidden."))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("HideUnconnectedPins", "Hide Unconnected Pins"))
				.Font(DetailBuilder.GetDetailFont())
			]
		]
	];
}

void FShidenGetCommandArgumentsNodeDetails::OnGenerateElementForPropertyPin(TSharedRef<IPropertyHandle> ElementProperty, int32,
                                                                            IDetailChildrenBuilder& ChildrenBuilder)
{
	FText PropertyFriendlyName = LOCTEXT("Invalid", "Invalid");
	FString FilterString;
	if (const TSharedPtr<IPropertyHandle> PropertyFriendlyNameHandle = ElementProperty->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, PropertyFriendlyName)))
	{
		FString DisplayFriendlyName;
		if (PropertyFriendlyNameHandle->GetValue(DisplayFriendlyName) == FPropertyAccess::Success)
		{
			FilterString = DisplayFriendlyName;
			PropertyFriendlyName = FText::FromString(DisplayFriendlyName);
		}
	}

	if (const TSharedPtr<IPropertyHandle> PropertyNameHandle = ElementProperty->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, PropertyName)))
	{
		FString RawName;
		if (PropertyNameHandle->GetValue(RawName) == FPropertyAccess::Success)
		{
			FilterString += TEXT(" ") + RawName;
		}
	}

	FText PinTooltip;
	if (const TSharedPtr<IPropertyHandle> PropertyTooltipHandle = ElementProperty->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, PropertyTooltip)))
	{
		if (PropertyTooltipHandle->GetValue(PinTooltip) == FPropertyAccess::Success)
		{
			FilterString += TEXT(" ") + PinTooltip.ToString();
		}
	}

	ChildrenBuilder.AddCustomRow(PropertyFriendlyName)
	.FilterString(FText::AsCultureInvariant(FilterString))
	.NameContent()
	[
		ElementProperty->CreatePropertyNameWidget(PropertyFriendlyName, PinTooltip)
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		.ToolTipText(LOCTEXT("AsPinTooltip", "Show this output pin on the node."))
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SCheckBox)
			.IsChecked(this, &FShidenGetCommandArgumentsNodeDetails::GetShowPinValueForProperty, ElementProperty)
			.OnCheckStateChanged(this, &FShidenGetCommandArgumentsNodeDetails::OnShowPinChanged, ElementProperty)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("AsPin", " (As pin)"))
			.Font(ChildrenBuilder.GetParentCategory().GetParentLayout().GetDetailFont())
		]
	];
}

ECheckBoxState FShidenGetCommandArgumentsNodeDetails::GetShowPinValueForProperty(TSharedRef<IPropertyHandle> InElementProperty) const
{
	bool bIsShown = false;
	const TSharedPtr<IPropertyHandle> ShowPinHandle = InElementProperty->GetChildHandle(GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, bShowPin));
	if (ShowPinHandle.IsValid() && ShowPinHandle->GetValue(bIsShown) == FPropertyAccess::Success)
	{
		return bIsShown ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}
	return ECheckBoxState::Undetermined;
}

void FShidenGetCommandArgumentsNodeDetails::OnShowPinChanged(ECheckBoxState InNewState, TSharedRef<IPropertyHandle> InElementProperty)
{
	if (const TSharedPtr<IPropertyHandle> ShowPinHandle = InElementProperty->GetChildHandle(GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, bShowPin)))
	{
		ShowPinHandle->SetValue(InNewState == ECheckBoxState::Checked);
	}
}

FReply FShidenGetCommandArgumentsNodeDetails::HideUnconnectedPins() const
{
	if (TargetNode.IsValid())
	{
		TargetNode->HideUnconnectedOutputPins();
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
