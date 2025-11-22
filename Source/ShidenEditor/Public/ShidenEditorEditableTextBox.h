// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/MultiLineEditableTextBox.h"
#include "ShidenEditorEditableTextBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSingleLineCursorMovedDelegate, int32, ColumnIndex);

/**
 * Shiden editor version of single-line editable text box with editor styling
 * Inherits from UMultiLineEditableTextBox but disables multi-line support
 */
UCLASS(MinimalAPI)
class UShidenEditorEditableTextBox : public UMultiLineEditableTextBox
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnSingleLineCursorMovedDelegate OnCursorMoved;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void InsertTextAtLocation(int32 ColumnIndex, const FString& StringToInsert);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void GoToCursorLocation(int32 ColumnIndex) const;

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface

	void HandleOnCursorMoved(const FTextLocation& NewCursorPosition) const;
};
