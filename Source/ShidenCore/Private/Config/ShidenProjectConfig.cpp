// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Config/ShidenProjectConfig.h"
#include "Save/ShidenUserSaveGame.h"
#include "Save/ShidenSystemSaveGame.h"
#include "Save/ShidenSaveSlotsSaveGame.h"
#include "Save/ShidenPredefinedSystemSaveGame.h"

#if WITH_EDITOR
#include "Misc/MessageDialog.h"
#endif

SHIDENCORE_API UShidenProjectConfig::UShidenProjectConfig(const FObjectInitializer& ObjectInitializer)
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

SHIDENCORE_API void UShidenProjectConfig::AddScenarioPath(const FGuid& ScenarioId, const FString& ScenarioPath)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->ScenarioPaths.Add(ScenarioId, ScenarioPath);
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& Paths)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->ScenarioPaths = Paths;
	ProjectConfig->ScenarioPaths.KeySort([](const FGuid& A, const FGuid& B)
	{
		return A < B;
	});
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->ScenarioDirectoryPath = Path;
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API void UShidenProjectConfig::SetMacroDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->MacroDirectoryPath = Path;
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API void UShidenProjectConfig::AddPreset(const FString& Name, const FShidenPreset& Preset)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->Presets.Add(Name, Preset);
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API bool UShidenProjectConfig::TryGetPreset(const FString& Name, FShidenPreset& Preset)
{
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	if (const FShidenPreset* FoundPreset = ProjectConfig->Presets.Find(Name))
	{
		Preset = *FoundPreset;
		return true;
	}
	return false;
}

SHIDENCORE_API void UShidenProjectConfig::RemovePreset(const FString& Name)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->Presets.Remove(Name);
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API void UShidenProjectConfig::SetUserVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->UserVariableDefinitions = Definitions;
	SaveProjectConfigChanges(ProjectConfig);
}

SHIDENCORE_API void UShidenProjectConfig::SetSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ProjectConfig->SystemVariableDefinitions = Definitions;
	SaveProjectConfigChanges(ProjectConfig);
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

void UShidenProjectConfig::SaveProjectConfigChanges(const TObjectPtr<UShidenProjectConfig> Config)
{
	if (Config)
	{
		Config->SaveConfig(CPF_Config, *Config->GetDefaultConfigFilename());
		Config->TryUpdateDefaultConfigFile();
	}
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
