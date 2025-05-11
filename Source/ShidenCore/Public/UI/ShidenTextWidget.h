// Copyright (c) 2025 HANON. All Rights Reserved.

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
	int CurrentLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<FString, TObjectPtr<URichTextBlock>> RichTextBlocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<FString, TObjectPtr<UTextBlock>> TextBlocks;

public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget")
	void GetAllFullTexts(TMap<FString, FString>& Texts);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void GetFullText(const FString& TextType, FString& Text, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void GetCurrentText(const FString& TextType, FString& Text, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget",
		meta = (TextType = "Default", AutoCreateRefTerm = "OnOpened"))
	void OpenWindow(const FString& TextType, const FShidenOpenTextWindowDelegate& OnOpened, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget",
		meta = (TextType = "Default", AutoCreateRefTerm = "OnClosed"))
	void CloseWindow(const FString& TextType, const FShidenCloseTextWindowDelegate& OnClosed, bool& bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void SetText(const FString& TextType, const FString& RawText, int Length);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void ClearText(const FString& TextType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void ClearAllTexts();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void PreviewText(const FString& TextType, const FString& Text, bool& bSuccess);
};
