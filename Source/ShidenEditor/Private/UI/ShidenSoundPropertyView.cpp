// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenSoundPropertyView.h"
#include "ContentBrowserDelegates.h"
#include "PropertyCustomizationHelpers.h"
#include "Sound/SoundClass.h"
#include "ThumbnailRendering/ThumbnailManager.h"

UE_ENABLE_OPTIMIZATION

#define LOCTEXT_NAMESPACE "UMG"

UShidenSoundPropertyView::UShidenSoundPropertyView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

void UShidenSoundPropertyView::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	EntryBox.Reset();
}

TSharedRef<SWidget> UShidenSoundPropertyView::RebuildWidget()
{
	EntryBox = SNew(SObjectPropertyEntryBox)
		.AllowedClass(USoundBase::StaticClass())
		.OnShouldFilterAsset(FOnShouldFilterAsset::CreateLambda([this](const FAssetData& AssetData)
		{
			if (this->SoundClass == nullptr) { return false; }
			FString SoundClassPath;
			if (!AssetData.GetTagValue("SoundClassObject", SoundClassPath) || SoundClassPath.IsEmpty())
			{
				// Exclude if SoundClassObject does not exist
				return true;
			}
			const FSoftObjectPath SoundClassObjectPath(SoundClassPath);
			const USoundClass* TempSoundClass = Cast<USoundClass>(SoundClassObjectPath.TryLoad());
			if (!TempSoundClass) { return true; }
			// Allow only if SoundClass is this->SoundClass or its subclass
			return SoundClass != TempSoundClass && !SoundClass->ChildClasses.Contains(TempSoundClass);
		}))
		.AllowClear(true)
		.ThumbnailPool(UThumbnailManager::Get().GetSharedThumbnailPool())
		.ObjectPath_UObject(this, &UShidenSoundPropertyView::GetCurrentAssetPath)
		.OnObjectChanged_UObject(this, &UShidenSoundPropertyView::OnObjectChanged);

	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(4.0f, 0.0f)
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 1.0f, 0.0f, 1.0f))
			.FillWidth(1)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Sound")))
				.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
			]
		]
		+ SHorizontalBox::Slot()
		.Padding(4.0f, 0.0f)
		.FillWidth(1.0f)
		.VAlign(VAlign_Center)
		[
			EntryBox.ToSharedRef()
		]
		+ SHorizontalBox::Slot()
		.Padding(2.0f)
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.IsFocusable(false)
			.ButtonStyle(FAppStyle::Get(), "SimpleButton")
			.ContentPadding(0)
			.Visibility_UObject(this, &UShidenSoundPropertyView::GetResetVisibility)
			.OnClicked_UObject(this, &UShidenSoundPropertyView::OnResetToDefault)
			.Content()
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault"))
				.ColorAndOpacity(FSlateColor::UseForeground())
			]
		];

	return HorizontalBox;
}

FReply UShidenSoundPropertyView::OnResetToDefault()
{
	SelectedAsset = nullptr;
	OnAssetChanged.Broadcast(nullptr);
	return FReply::Handled();
}

EVisibility UShidenSoundPropertyView::GetResetVisibility() const
{
	return SelectedAsset.IsValid() ? EVisibility::Visible : EVisibility::Hidden;
}

FString UShidenSoundPropertyView::GetCurrentAssetPath() const
{
	return SelectedAsset.IsValid() ? SelectedAsset->GetPathName() : FString("");
}

void UShidenSoundPropertyView::OnObjectChanged(const FAssetData& AssetData)
{
	SelectedAsset = TSoftObjectPtr<USoundBase>(Cast<USoundBase>(AssetData.GetAsset()));
	OnAssetChanged.Broadcast(SelectedAsset.Get());
}

#if WITH_EDITOR

void UShidenSoundPropertyView::SetSoundClass(USoundClass* NewSoundClass)
{
	SoundClass = NewSoundClass;
	if (SelectedAsset.IsValid() && SoundClass
		&& SoundClass != SelectedAsset->SoundClassObject
		&& !SoundClass->ChildClasses.Contains(SelectedAsset->SoundClassObject))
	{
		SelectedAsset = nullptr;
		OnAssetChanged.Broadcast(nullptr);
	}
}

USoundClass* UShidenSoundPropertyView::GetSoundClass() const
{
	return SoundClass.Get();
}

bool UShidenSoundPropertyView::TrySetSelectedAsset(USoundBase* NewAsset)
{
	if (!NewAsset || !SoundClass
		|| SoundClass == NewAsset->SoundClassObject
		|| SoundClass->ChildClasses.Contains(NewAsset->SoundClassObject))
	{
		SelectedAsset = NewAsset;
		OnAssetChanged.Broadcast(NewAsset);
		return true;
	}
	return false;
}

USoundBase* UShidenSoundPropertyView::GetSelectedAsset() const
{
	return SelectedAsset.Get();
}

const FText UShidenSoundPropertyView::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
