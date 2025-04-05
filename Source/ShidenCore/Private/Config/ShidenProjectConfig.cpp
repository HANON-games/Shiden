// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Config/ShidenProjectConfig.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

SHIDENCORE_API UShidenProjectConfig::UShidenProjectConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
		, PredefinedSystemVariable(FShidenPredefinedSystemVariable())
		, ScenarioDirectoryPath("/Game/Shiden/Scenarios/")
		, MacroDirectoryPath("/Game/Shiden/Macros/")
		, TargetFrameRate(60)
		, bAutoSaveOnMobileAppWillDeactivate(true)
{
	ScenarioPaths = TMap<FGuid, FString>();
	WidgetClass = Cast<UClass>(FSoftObjectPath(TEXT("/Shiden/Samples/WBP_ShidenWidgetSample.WBP_ShidenWidgetSample_C")).TryLoad());
	MasterSoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenMaster.SC_ShidenMaster"));
	BgmSoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenBgm.SC_ShidenBgm"));
	SeSoundClass = FSoftObjectPath(TEXT("/Shiden/Misc/Audio/SC_ShidenSe.SC_ShidenSe"));
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
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths.Add(ScenarioId, ScenarioPath);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioPaths(const TMap<FGuid, FString>& Paths)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioPaths = Paths;
	ShidenProjectConfig->ScenarioPaths.KeySort([](const FGuid& A, const FGuid& B)
	{
		return A < B;
	});
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::SetScenarioDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->ScenarioDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::SetMacroDirectoryPath(const FString& Path)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->MacroDirectoryPath = Path;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::AddPreset(const FString& Name, const FShidenPreset& Preset)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Add(Name, Preset);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::GetPreset(const FString& Name, FShidenPreset& Preset, bool& bSuccess)
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->Presets.Contains(Name))
	{
		Preset = ShidenProjectConfig->Presets[Name];
		bSuccess = true;
		return;
	}
	bSuccess = false;
}

SHIDENCORE_API void UShidenProjectConfig::RemovePreset(const FString& Name)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->Presets.Remove(Name);
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::SetUserVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->UserVariableDefinitions = Definitions;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

SHIDENCORE_API void UShidenProjectConfig::SetSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
{
	const TObjectPtr<UShidenProjectConfig> ShidenProjectConfig = GetMutableDefault<UShidenProjectConfig>();
	ShidenProjectConfig->SystemVariableDefinitions = Definitions;
	ShidenProjectConfig->SaveConfig(CPF_Config, *ShidenProjectConfig->GetDefaultConfigFilename());
	ShidenProjectConfig->TryUpdateDefaultConfigFile();
}

USoundClass* UShidenProjectConfig::GetMasterSoundClass()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->MasterSoundClass.IsValid())
	{
		return ShidenProjectConfig->MasterSoundClass.Get();
	}
	return ShidenProjectConfig->MasterSoundClass.LoadSynchronous();
}

USoundClass* UShidenProjectConfig::GetBgmSoundClass()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->BgmSoundClass.IsValid())
	{
		return ShidenProjectConfig->BgmSoundClass.Get();
	}
	return ShidenProjectConfig->BgmSoundClass.LoadSynchronous();
}

USoundClass* UShidenProjectConfig::GetSeSoundClass()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->SeSoundClass.IsValid())
	{
		return ShidenProjectConfig->SeSoundClass.Get();
	}
	return ShidenProjectConfig->SeSoundClass.LoadSynchronous();
}

USoundClass* UShidenProjectConfig::GetVoiceSoundClass()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->VoiceSoundClass.IsValid())
	{
		return ShidenProjectConfig->VoiceSoundClass.Get();
	}
	return ShidenProjectConfig->VoiceSoundClass.LoadSynchronous();
}

USoundMix* UShidenProjectConfig::GetSoundClassMix()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	if (ShidenProjectConfig->SoundClassMix.IsValid())
	{
		return ShidenProjectConfig->SoundClassMix.Get();
	}
	return ShidenProjectConfig->SoundClassMix.LoadSynchronous();
}
