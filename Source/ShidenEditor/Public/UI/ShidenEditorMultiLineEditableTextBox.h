// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/MultiLineEditableTextBox.h"
#include "ShidenEditorMultiLineEditableTextBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCursorMovedDelegate, int32, LineIndex, int32, ColumnIndex);

/**
 * Shiden editor version of multi-line editable text box with editor styling
 */
UCLASS(MinimalAPI)
class UShidenEditorMultiLineEditableTextBox : public UMultiLineEditableTextBox
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnCursorMovedDelegate OnCursorMoved;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void InsertTextAtLocation(int32 LineIndex, int32 ColumnIndex, const FString& StringToInsert);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void GoToCursorLocation(int32 LineIndex, int32 ColumnIndex) const;

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface

	void HandleOnCursorMoved(const FTextLocation& NewCursorPosition) const;
};
