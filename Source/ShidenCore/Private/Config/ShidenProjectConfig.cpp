// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Config/ShidenProjectConfig.h"
#include "Save/ShidenUserSaveGame.h"
#include "Save/ShidenSystemSaveGame.h"
#include "Save/ShidenSaveSlotsSaveGame.h"
#include "Save/ShidenPredefinedSystemSaveGame.h"
#include "UObject/UnrealType.h"
#include "Misc/ConfigCacheIni.h"

#if WITH_EDITOR
#include "Misc/MessageDialog.h"
#endif

UShidenProjectConfig::UShidenProjectConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , PredefinedSystemVariable(FShidenPredefinedSystemVariable())
	  , ScenarioDirectoryPath("/Game/Shiden/Scenarios/")
	  , MacroDirectoryPath("/Game/Shiden/Macros/")
	  , bAutoSaveOnMobileAppWillDeactivate(true)
	  , UserSaveGameClass(UShidenUserSaveGame::StaticClass())
	  , SystemSaveGameClass(UShidenSystemSaveGame::StaticClass())
{
	ScenarioPaths = TMap<FGuid, FString>();
	DefaultWidgetClass = Cast<UClass>(FSoftObjectPath(TEXT("/Shiden/Samples/WBP_ShidenWidgetSample.WBP_ShidenWidgetSample_C")).TryLoad());
	MasterSoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenMaster.SC_ShidenMaster"));
	BGMSoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenBGM.SC_ShidenBGM"));
	SESoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenSE.SC_ShidenSE"));
	VoiceSoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenVoice.SC_ShidenVoice"));
	SoundClassMix = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SCM_ShidenSoundClassMix.SCM_ShidenSoundClassMix"));
	Presets = TMap<FString, FShidenPreset>();
	AdditionalTextTypes = TMap<FString, FShidenTextType>();
	CommandDefinitions = {
		FSoftObjectPath(TEXT("/Shiden/Core/DA_ShidenStandardCommandDefinitions.DA_ShidenStandardCommandDefinitions"))
	};
	UserVariableDefinitions = TArray<FShidenVariableDefinition>();
	SystemVariableDefinitions = TArray<FShidenVariableDefinition>();
}

FName UShidenProjectConfig::GetContainerName() const
{
	return TEXT("Project");
}

FName UShidenProjectConfig::GetCategoryName() const
{
	return TEXT("Plugins");
}

FName UShidenProjectConfig::GetSectionName() const
{
	return TEXT("ShidenVisualNovelEditor");
}

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "ShidenNamespace"

FText UShidenProjectConfig::GetSectionText() const
{
	return LOCTEXT("ShidenVisualNovelEditorName", "Shiden Visual Novel Editor");
}

FText UShidenProjectConfig::GetSectionDescription() const
{
	return LOCTEXT("ShidenVisualNovelEditorDescription", "Shiden Visual Novel Editor");
}

#undef LOCTEXT_NAMESPACE
#endif

void UShidenProjectConfig::AddScenarioPath(const FGuid& ScenarioId, const FString& ScenarioPath)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->ScenarioPaths.Add(ScenarioId, ScenarioPath);
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& Paths)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->ScenarioPaths = Paths;
	ProjectConfig->ScenarioPaths.KeySort([](const FGuid& A, const FGuid& B)
	{
		return A < B;
	});
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

void UShidenProjectConfig::SetScenarioDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->ScenarioDirectoryPath = Path;
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

void UShidenProjectConfig::SetMacroDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->MacroDirectoryPath = Path;
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

void UShidenProjectConfig::AddPreset(const FString& Name, const FShidenPreset& Preset)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->Presets.Add(Name, Preset);
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

bool UShidenProjectConfig::TryGetPreset(const FString& Name, FShidenPreset& Preset)
{
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	if (const FShidenPreset* FoundPreset = ProjectConfig->Presets.Find(Name))
	{
		Preset = *FoundPreset;
		return true;
	}
	return false;
}

void UShidenProjectConfig::RemovePreset(const FString& Name)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->Presets.Remove(Name);
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

void UShidenProjectConfig::SetUserVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->UserVariableDefinitions = Definitions;
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

void UShidenProjectConfig::SetSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->SystemVariableDefinitions = Definitions;
	ProjectConfig->SaveConfig();
	ProjectConfig->TryUpdateDefaultConfigFile();
}

template <typename T>
T* UShidenProjectConfig::LoadSoftObjectIfNeeded(const TSoftObjectPtr<T>& SoftObject)
{
	if (SoftObject.IsValid())
	{
		return SoftObject.Get();
	}
	return SoftObject.LoadSynchronous();
}

USoundClass* UShidenProjectConfig::GetMasterSoundClass()
{
	return LoadSoftObjectIfNeeded(GetDefault<UShidenProjectConfig>()->MasterSoundClass);
}

USoundClass* UShidenProjectConfig::GetBGMSoundClass()
{
	return LoadSoftObjectIfNeeded(GetDefault<UShidenProjectConfig>()->BGMSoundClass);
}

USoundClass* UShidenProjectConfig::GetSESoundClass()
{
	return LoadSoftObjectIfNeeded(GetDefault<UShidenProjectConfig>()->SESoundClass);
}

USoundClass* UShidenProjectConfig::GetVoiceSoundClass()
{
	return LoadSoftObjectIfNeeded(GetDefault<UShidenProjectConfig>()->VoiceSoundClass);
}

USoundMix* UShidenProjectConfig::GetSoundClassMix()
{
	return LoadSoftObjectIfNeeded(GetDefault<UShidenProjectConfig>()->SoundClassMix);
}

#if WITH_EDITOR
void UShidenProjectConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	// Handle UserSaveGameClass changes
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenProjectConfig, UserSaveGameClass))
	{
		if (!UShidenSaveSlotsSaveGame::DoesExist())
		{
			return;
		}

		const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlots = UShidenSaveSlotsSaveGame::GetOrCreate();
		const int32 PreviousNumSlots = SaveSlots->SaveSlots.Num();
		if (PreviousNumSlots > 0)
		{
			const FText WarningTitle = NSLOCTEXT("ShidenNamespace", "UserSaveDataClassChangeTitle", "User Save Data Class Change");
			const FText WarningMessage = NSLOCTEXT("ShidenNamespace", "UserSaveDataClassChangeMessage",
			                                       "Changing the user save data class may prevent existing save data from being read or written properly.\n\n"
			                                       "Do you want to delete all existing user save data?");

			const EAppReturnType::Type Response = FMessageDialog::Open(EAppMsgType::YesNo, WarningMessage, WarningTitle);

			if (Response == EAppReturnType::Yes)
			{
				for (const TPair<FString, FShidenSaveSlot>& Pair : SaveSlots->SaveSlots)
				{
					UShidenUserSaveGame::TryDelete(Pair.Key);
					UShidenSaveSlotsSaveGame::TryDelete(Pair.Key);
				}

				// Display completion message
				const FText CompletionTitle = NSLOCTEXT("ShidenNamespace", "DeletionCompleteTitle", "Deletion Complete");
				const FText CompletionMessage = FText::Format(
					NSLOCTEXT("ShidenNamespace", "UserSaveDataDeletionMessage", "Deleted {0} user save data."),
					FText::AsNumber(PreviousNumSlots)
				);
				FMessageDialog::Open(EAppMsgType::Ok, CompletionMessage, CompletionTitle);
			}
		}
	}
	// Handle SystemSaveGameClass changes
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UShidenProjectConfig, SystemSaveGameClass))
	{
		if (!UShidenSystemSaveGame::DoesExist())
		{
			return;
		}

		const FText WarningTitle = NSLOCTEXT("ShidenNamespace", "SystemSaveDataClassChangeTitle", "System Save Data Class Change");
		const FText WarningMessage = NSLOCTEXT("ShidenNamespace", "SystemSaveDataClassChangeMessage",
		                                       "Changing the system save data class may prevent existing save data from being read or written properly.\n\n"
		                                       "Do you want to delete all existing system save data?");

		const EAppReturnType::Type Response = FMessageDialog::Open(EAppMsgType::YesNo, WarningMessage, WarningTitle);

		if (Response == EAppReturnType::Yes)
		{
			UShidenSystemSaveGame::TryDelete();

			// Display completion message
			const FText CompletionTitle = NSLOCTEXT("ShidenNamespace", "DeletionCompleteTitle", "Deletion Complete");
			const FText CompletionMessage = NSLOCTEXT("ShidenNamespace", "SystemSaveDataDeletionMessage", "System save data has been deleted.");
			FMessageDialog::Open(EAppMsgType::Ok, CompletionMessage, CompletionTitle);
		}
	}
}
#endif
