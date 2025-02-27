// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "ShidenTextWidget.generated.h"

UENUM(BlueprintType)
enum class EShidenTextWidgetInteractionState : uint8
{
	Busy = 0,
	Interactive
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EShidenTextWidgetInteractionState InteractionState = EShidenTextWidgetInteractionState::Busy;

public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	EShidenTextWidgetInteractionState GetWidgetInteractionState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget")
	void GetAllFullTexts(TMap<FString, FString>& Texts);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void GetFullText(const FString& TextType, FString& Text, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void GetCurrentText(const FString& TextType, FString& Text, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void OpenWindow(const FString& TextType, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void CloseWindow(const FString& TextType, bool& bResult);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void SetText(const FString& TextType, const FString& RawText, const int Length, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void ClearText(const FString& TextType, bool& bResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void ClearAllTexts();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (TextType = "Default"))
	void PreviewText(const FString& TextType, const FString& Text, bool& bResult);
};
