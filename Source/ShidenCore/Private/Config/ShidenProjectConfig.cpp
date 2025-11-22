// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Config/ShidenProjectConfig.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

SHIDENCORE_API UShidenProjectConfig::UShidenProjectConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , PredefinedSystemVariable(FShidenPredefinedSystemVariable())
	  , ScenarioDirectoryPath("/Game/Shiden/Scenarios/")
	  , MacroDirectoryPath("/Game/Shiden/Macros/")
	  , bAutoSaveOnMobileAppWillDeactivate(true)
{
	ScenarioPaths = TMap<FGuid, FString>();
	WidgetClass = Cast<UClass>(FSoftObjectPath(TEXT("/Shiden/Samples/WBP_ShidenWidgetSample.WBP_ShidenWidgetSample_C")).TryLoad());
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

template<typename T>
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
