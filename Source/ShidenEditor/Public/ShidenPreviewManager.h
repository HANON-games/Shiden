// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Scenario/ShidenManagerInterface.h"
#include "InputMappingContext.h"
#include "Audio/ShidenSoundInfo.h"
#include "UI/ShidenWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"
#include "ShidenPreviewManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SHIDENEDITOR_API UShidenPreviewManager : public UEditorUtilityWidget, public IShidenManagerInterface
{
	GENERATED_BODY()

public:
	explicit UShidenPreviewManager(const FObjectInitializer& ObjectInitializer);

	virtual void FindShidenDigitalInput_Implementation(const UInputAction* InputAction, bool& bValue, bool& bSuccess) override;

	virtual void FindShidenAxis1DInput_Implementation(const UInputAction* InputAction, float& X, bool& bSuccess) override;

	virtual void FindShidenAxis2DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, bool& bSuccess) override;

	virtual void FindShidenAxis3DInput_Implementation(const UInputAction* InputAction, float& X, float& Y, float& Z, bool& bSuccess) override;

	virtual void PlaySound_Implementation(const FShidenSoundInfo& SoundInfo, const bool bRegisterSound, float& Duration, bool& bSuccess) override;

	virtual void StopSound_Implementation(const int32& TrackId, const EShidenSoundType Type) override;

	virtual void StopVoices_Implementation() override;

	virtual void AdjustBGMVolume_Implementation(const int32& TrackId, const float& VolumeDuration, const float& VolumeLevel, const EAudioFaderCurve FaderCurve) override;

	virtual void PauseAllSounds_Implementation(const bool bPause) override;

	virtual void PlayForceFeedback_Implementation(const FString& ForceFeedbackEffectPath, bool& bSuccess) override;

	virtual void CallMacroAsParallel_Implementation(const FString& NewProcessName, UObject* CallerObject) override;

	virtual void Initialize_Implementation(const UShidenWidget* ShidenWidget) override;

	virtual void Destroy_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "SVNInternal")
	void GetSoundInfo(TMap<int32, FShidenSoundInfo>& BGMInfo, TArray<FShidenSoundInfo>& SEInfo, TMap<int32, FShidenSoundInfo>& VoiceInfo) const;

	UFUNCTION(BlueprintCallable, Category = "SVNInternal")
	void ClearSoundInfo();

protected:
	virtual const FText GetPaletteCategory() override;

private:
	UPROPERTY()
	TMap<int32, FShidenSoundInfo> CachedBGMInfo;

	UPROPERTY()
	TArray<FShidenSoundInfo> CachedSEInfo;

	UPROPERTY()
	TMap<int32, FShidenSoundInfo> CachedVoiceInfo;
};
