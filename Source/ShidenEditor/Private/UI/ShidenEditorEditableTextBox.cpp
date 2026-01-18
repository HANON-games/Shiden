// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenEditorEditableTextBox.h"
#include "Styling/SlateStyle.h"
#include "Styling/AppStyle.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

UShidenEditorEditableTextBox::UShidenEditorEditableTextBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WidgetStyle = FAppStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox");
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	// ReSharper disable once CppDeprecatedEntity
	TextStyle_DEPRECATED = WidgetStyle.TextStyle;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

TSharedRef<SWidget> UShidenEditorEditableTextBox::RebuildWidget()
{
	MyEditableTextBlock = SNew(SMultiLineEditableTextBox)
		.Style(&WidgetStyle)
		.AllowContextMenu(AllowContextMenu)
		.IsReadOnly(GetIsReadOnly())
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		.AllowMultiLine(false)
		.OnCursorMoved_Lambda([this](const FTextLocation& Location)
		{
			HandleOnCursorMoved(Location);
		});

	return MyEditableTextBlock.ToSharedRef();
}

const FText UShidenEditorEditableTextBox::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}

void UShidenEditorEditableTextBox::HandleOnCursorMoved(const FTextLocation& NewCursorPosition) const
{
	OnCursorMoved.Broadcast(NewCursorPosition.GetOffset());
}

void UShidenEditorEditableTextBox::InsertTextAtLocation(const int32 ColumnIndex, const FString& StringToInsert)
{
	if (!MyEditableTextBlock.IsValid())
	{
		return;
	}

	FString CurrentString = GetText().ToString();

	if (ColumnIndex < 0 || ColumnIndex > CurrentString.Len())
	{
		return;
	}

	CurrentString.InsertAt(ColumnIndex, StringToInsert);

	SetText(FText::FromString(CurrentString));
}

void UShidenEditorEditableTextBox::GoToCursorLocation(const int32 ColumnIndex) const
{
	if (!MyEditableTextBlock.IsValid())
	{
		return;
	}

	MyEditableTextBlock->GoTo(FTextLocation(0, ColumnIndex));
}
