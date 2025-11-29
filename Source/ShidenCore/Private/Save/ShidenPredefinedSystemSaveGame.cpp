// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Save/ShidenPredefinedSystemSaveGame.h"
#include "AudioDevice.h"
#include "Config/ShidenProjectConfig.h"
#include "System/ShidenSubsystem.h"
#include "Engine/Engine.h"

TObjectPtr<UShidenPredefinedSystemSaveGame> UShidenPredefinedSystemSaveGame::GetOrCreate()
{
	return DoesExist()
		       ? Cast<UShidenPredefinedSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(PredefinedSystemDataSlotName, 0))
		       : Cast<UShidenPredefinedSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
}

void UShidenPredefinedSystemSaveGame::Apply(const TObjectPtr<const UObject> WorldContextObject) const
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ShidenProjectConfig->SystemVariableDefinitions);
	ShidenSubsystem->PredefinedSystemVariable = FShidenPredefinedSystemVariable(PredefinedSystemVariable);
	ShidenSubsystem->ScenarioReadLines = ScenarioReadLines;

	// Apply volume rate
	if (!GEngine || !GEngine->UseSound())
	{
		return;
	}

	const TObjectPtr<UWorld> ThisWorld = WorldContextObject->GetWorld();
	if (!ThisWorld || !ThisWorld->bAllowAudioPlayback)
	{
		return;
	}

	if (FAudioDeviceHandle AudioDevice = ThisWorld->GetAudioDevice())
	{
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.MasterVolume, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.BGMVolume, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetSESoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.SEVolume, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.VoiceVolume, 1.0, 0.0, true);
		AudioDevice->PushSoundMixModifier(ShidenProjectConfig->GetSoundClassMix());
	}
}

void UShidenPredefinedSystemSaveGame::Prepare()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	PredefinedSystemVariable = ShidenSubsystem->PredefinedSystemVariable;
	ScenarioReadLines = ShidenSubsystem->ScenarioReadLines;
	if (CreatedAt == FDateTime::MinValue())
	{
		CreatedAt = FDateTime::UtcNow();
	}
	UpdatedAt = FDateTime::UtcNow();
}

bool UShidenPredefinedSystemSaveGame::TryCommit()
{
	return UGameplayStatics::SaveGameToSlot(this, PredefinedSystemDataSlotName, 0);
}

bool UShidenPredefinedSystemSaveGame::DoesExist()
{
	return UGameplayStatics::DoesSaveGameExist(PredefinedSystemDataSlotName, 0);
}

bool UShidenPredefinedSystemSaveGame::TryDelete()
{
	return UGameplayStatics::DeleteGameInSlot(PredefinedSystemDataSlotName, 0);
}
