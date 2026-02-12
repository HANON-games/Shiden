// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Config/ShidenEditorUserConfig.h"

UShidenEditorUserConfig::UShidenEditorUserConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , ScenarioFilterPath("")
	  , EditScenarioPath("")
	  , bPreviewSound(false)
	  , PreviewSize(960.0f, 540.0f)
	  , bCombineEditorWindows(true)
	  , PreviewPosition(EShidenPreviewPosition::Right)
	  , ShidenDebuggerRefreshInterval(0.1f)
	  , bAutoSaveScenario(false)
	  , bUseAdvancedMode(true)
	  , bForceShowAllCommandArguments(false)
{
}

template <typename T, typename TMember>
void UShidenEditorUserConfig::UpdateConfig(TMember UShidenEditorUserConfig::* MemberPtr, const T& Value)
{
	if (const TObjectPtr<UShidenEditorUserConfig> Config = GetMutableDefault<UShidenEditorUserConfig>())
	{
		Config->*MemberPtr = Value;
		Config->SaveConfig();
		Config->TryUpdateDefaultConfigFile();
	}
}

FName UShidenEditorUserConfig::GetContainerName() const
{
	return TEXT("Editor");
}

FName UShidenEditorUserConfig::GetCategoryName() const
{
	return TEXT("Plugins");
}

FName UShidenEditorUserConfig::GetSectionName() const
{
	return TEXT("ShidenVisualNovelEditorUser");
}

#define LOCTEXT_NAMESPACE "FShidenEditorModule"

FText UShidenEditorUserConfig::GetSectionText() const
{
	return LOCTEXT("ShidenVisualNovelEditorUserName", "Shiden Visual Novel Editor (User)");
}

FText UShidenEditorUserConfig::GetSectionDescription() const
{
	return LOCTEXT("ShidenVisualNovelEditorUserDescription", "Shiden Visual Novel Editor Per-User Settings");
}

#undef LOCTEXT_NAMESPACE

void UShidenEditorUserConfig::SetScenarioFilterPath(const FString& Path)
{
	UpdateConfig(&UShidenEditorUserConfig::ScenarioFilterPath, Path);
}

void UShidenEditorUserConfig::SetEditScenarioPath(const FString& PackageName)
{
	UpdateConfig(&UShidenEditorUserConfig::EditScenarioPath, PackageName);
}

void UShidenEditorUserConfig::SetPreviewSound(const bool bEnabled)
{
	UpdateConfig(&UShidenEditorUserConfig::bPreviewSound, bEnabled);
}

void UShidenEditorUserConfig::SetPreviewSize(const FVector2D& Size)
{
	UpdateConfig(&UShidenEditorUserConfig::PreviewSize, Size);
}

void UShidenEditorUserConfig::SetShidenDebuggerRefreshInterval(const float Interval)
{
	UpdateConfig(&UShidenEditorUserConfig::ShidenDebuggerRefreshInterval, Interval);
}

void UShidenEditorUserConfig::SetAutoSaveScenario(const bool bEnabled)
{
	UpdateConfig(&UShidenEditorUserConfig::bAutoSaveScenario, bEnabled);
}
