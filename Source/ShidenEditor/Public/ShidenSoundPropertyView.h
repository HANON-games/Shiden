// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "Sound/SoundBase.h"
#include "ShidenSoundPropertyView.generated.h"

class SObjectPropertyEntryBox;

UCLASS()
class UShidenSoundPropertyView : public UContentWidget
{
	GENERATED_UCLASS_BODY()
	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetChanged, UObject*, Asset);

	UPROPERTY(BlueprintAssignable, Category = "View|Event")
	FOnAssetChanged OnAssetChanged;

	UFUNCTION(BlueprintCallable, Category = "View")
	void SetSoundClass(USoundClass* NewSoundClass);

	UFUNCTION(BlueprintPure, Category = "View")
	USoundClass* GetSoundClass() const;

	UFUNCTION(BlueprintCallable, Category = "View", meta = (DisplayName = "Set Selected Asset"))
	UPARAM(DisplayName = "Success") bool TrySetSelectedAsset(USoundBase* NewAsset);

	UFUNCTION(BlueprintPure, Category = "View")
	USoundBase* GetSelectedAsset() const;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	TSharedPtr<SObjectPropertyEntryBox> EntryBox;

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	FReply OnResetToDefault();

private:
	UPROPERTY()
	TObjectPtr<USoundClass> SoundClass = nullptr;

	UPROPERTY()
	TSoftObjectPtr<USoundBase> SelectedAsset = nullptr;

	EVisibility GetResetVisibility() const;

	FString GetCurrentAssetPath() const;

	void OnObjectChanged(const FAssetData& AssetData);
};
