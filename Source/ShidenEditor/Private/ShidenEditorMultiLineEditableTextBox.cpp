// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenEditorMultiLineEditableTextBox.h"
#include "Styling/SlateStyle.h"
#include "Styling/AppStyle.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

UShidenEditorMultiLineEditableTextBox::UShidenEditorMultiLineEditableTextBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WidgetStyle = FAppStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox");
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	// ReSharper disable once CppDeprecatedEntity
	TextStyle_DEPRECATED = WidgetStyle.TextStyle;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

TSharedRef<SWidget> UShidenEditorMultiLineEditableTextBox::RebuildWidget()
{
	MyEditableTextBlock = SNew(SMultiLineEditableTextBox)
		.Style(&WidgetStyle)
		.AllowContextMenu(AllowContextMenu)
		.IsReadOnly(GetIsReadOnly())
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		.OnCursorMoved_Lambda([this](const FTextLocation& Location)
		{
			HandleOnCursorMoved(Location);
		});

	return MyEditableTextBlock.ToSharedRef();
}

const FText UShidenEditorMultiLineEditableTextBox::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}

void UShidenEditorMultiLineEditableTextBox::HandleOnCursorMoved(const FTextLocation& NewCursorPosition) const
{
	OnCursorMoved.Broadcast(NewCursorPosition.GetLineIndex(), NewCursorPosition.GetOffset());
}

void UShidenEditorMultiLineEditableTextBox::InsertTextAtLocation(const int32 LineIndex, const int32 ColumnIndex, const FString& StringToInsert)
{
	if (!MyEditableTextBlock.IsValid())
	{
		return;
	}

	const FString CurrentString = GetText().ToString();

	TArray<FString> Lines;
	CurrentString.ParseIntoArrayLines(Lines, false);

	if (!Lines.IsValidIndex(LineIndex))
	{
		return;
	}

	FString& TargetLine = Lines[LineIndex];

	if (ColumnIndex < 0 || ColumnIndex > TargetLine.Len())
	{
		return;
	}

	TargetLine.InsertAt(ColumnIndex, StringToInsert);

	SetText(FText::FromString(FString::Join(Lines, TEXT("\n"))));
}

void UShidenEditorMultiLineEditableTextBox::GoToCursorLocation(const int32 LineIndex, const int32 ColumnIndex) const
{
	if (!MyEditableTextBlock.IsValid())
	{
		return;
	}

	MyEditableTextBlock->GoTo(FTextLocation(LineIndex, ColumnIndex));
}
