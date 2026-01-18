// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "ShidenTextWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FShidenOpenTextWindowDelegate);

DECLARE_DYNAMIC_DELEGATE(FShidenCloseTextWindowDelegate);

UCLASS(Abstract, Blueprintable)
class SHIDENCORE_API UShidenTextWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<FString, FString> OriginalTexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 CurrentLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<FString, TObjectPtr<URichTextBlock>> RichTextBlocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<FString, TObjectPtr<UTextBlock>> TextBlocks;

public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	/** 
	 * Get all full texts from all text types
	 * @param Texts [out] Map of text types and their full text content
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget")
	void GetAllFullTexts(TMap<FString, FString>& Texts);

	/** 
	 * Get the full text for a specific text type
	 * @param TextType The type of text to retrieve
	 * @param Text [out] The full text content
	 * @param bSuccess [out] Whether the text was successfully retrieved
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void GetFullText(const FString& TextType, FString& Text, bool& bSuccess);

	/** 
	 * Get the current text for a specific text type
	 * @param TextType The type of text to retrieve
	 * @param Text [out] The currently displayed text content
	 * @param bSuccess [out] Whether the text was successfully retrieved
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void GetCurrentText(const FString& TextType, FString& Text, bool& bSuccess);

	/** 
	 * Open the text window for a specific text type
	 * @param TextType The type of text window to open
	 * @param OnOpened Delegate called when the window is opened
	 * @param bSuccess [out] Whether the window was successfully opened
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget",
		meta = (TextType = "Default", AutoCreateRefTerm = "OnOpened"))
	void OpenWindow(const FString& TextType, const FShidenOpenTextWindowDelegate& OnOpened, bool& bSuccess);

	/** 
	 * Close the text window for a specific text type
	 * @param TextType The type of text window to close
	 * @param OnClosed Delegate called when the window is closed
	 * @param bSuccess [out] Whether the window was successfully closed
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget",
		meta = (TextType = "Default", AutoCreateRefTerm = "OnClosed"))
	void CloseWindow(const FString& TextType, const FShidenCloseTextWindowDelegate& OnClosed, bool& bSuccess);

	/**
	 * Sets the text for a specific text type, cutting it to the specified length
	 * @param TextType The type of text to set
	 * @param RawText The raw text content to set
	 * @param Length How many characters to display
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void SetText(const FString& TextType, const FString& RawText, int32 Length);

	/** 
	 * Clear text for a specific text type
	 * @param TextType The type of text to clear
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void ClearText(const FString& TextType);

	/** Clear all texts from all text types */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void ClearAllTexts();

	/** 
	 * Preview text for a specific text type
	 * @param TextType The type of text to preview
	 * @param Text The text content to preview
	 * @param bSuccess [out] Whether the preview operation was successful
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void PreviewText(const FString& TextType, const FString& Text, bool& bSuccess);
};
