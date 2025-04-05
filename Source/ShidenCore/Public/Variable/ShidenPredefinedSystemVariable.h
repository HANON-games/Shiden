// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenPredefinedSystemVariableDefinition.h"
#include "ShidenVariableDescriptor.h"
#include "Audio/ShidenVoiceStopCondition.h"
#include "Kismet/GameplayStatics.h"
#include "System/ShidenSkipCondition.h"
#include "ShidenPredefinedSystemVariable.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenPredefinedSystemVariable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float SkipSpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float BgmVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float SeVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float VoiceVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	int32 LanguageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float CharacterWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float AutoModeWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bShouldWaitForVoiceInAutoMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	EShidenVoiceStopCondition VoiceStopCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	EShidenSkipCondition SkipCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	FString ClickWaitingGlyph;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	FString PlatformName = UGameplayStatics::GetPlatformName();

	bool TryGetDefinition(const FString& Name, FShidenPredefinedSystemVariableDefinition& Definition) const;
	
	bool TryUpdateByString(const FString& Name, const FString& Value, bool bForceUpdateReadOnly = false) const;
	
	bool Contains(const FString& Name) const;

	bool TryGetAsString(const FString& Name, EShidenVariableType& Type, FString& Value) const;

	void ResetAll() const;

	bool TryReset(const FString& Name) const;

	void GetNames(TArray<FString>& OutNames) const;

	int32 Num() const;

	void ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors) const;
	
	explicit FShidenPredefinedSystemVariable()
	{
		ResetAll();
	}
	
	FShidenPredefinedSystemVariable& operator=(const FShidenPredefinedSystemVariable& Other)
	{
		if (this != &Other) {
			SkipSpeedRate = Other.SkipSpeedRate;
			VoiceVolume = Other.VoiceVolume;
			SeVolume = Other.SeVolume;
			MasterVolume = Other.MasterVolume;
			BgmVolume = Other.BgmVolume;
			LanguageIndex = Other.LanguageIndex;
			CharacterWaitTime = Other.CharacterWaitTime;
			AutoModeWaitTime = Other.AutoModeWaitTime;
			bShouldWaitForVoiceInAutoMode = Other.bShouldWaitForVoiceInAutoMode;
			VoiceStopCondition = Other.VoiceStopCondition;
			SkipCondition = Other.SkipCondition;
			ClickWaitingGlyph = Other.ClickWaitingGlyph;
		}
		return *this;
	}
	
	const TArray<FShidenPredefinedSystemVariableDefinition> Definitions
	{
			FShidenPredefinedSystemVariableDefinition(
				"SkipSpeedRate",
				EShidenVariableType::Float,
				"30.0",
				false,
				[this] { return FString::SanitizeFloat(SkipSpeedRate); },
				[this](const FString& InValue) { SkipSpeedRate = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"MasterVolume",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(MasterVolume); },
				[this](const FString& InValue) { MasterVolume = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"BgmVolume",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(BgmVolume); },
				[this](const FString& InValue) { BgmVolume = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"SeVolume",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(SeVolume); },
				[this](const FString& InValue) { SeVolume = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"VoiceVolume",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(VoiceVolume); },
				[this](const FString& InValue) { VoiceVolume = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"LanguageIndex",
				EShidenVariableType::Integer,
				"0",
				false,
				[this] { return FString::FromInt(LanguageIndex); },
				[this](const FString& InValue) { LanguageIndex = FCString::Atoi(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"CharacterWaitTime",
				EShidenVariableType::Float,
				"0.08",
				false,
				[this] { return FString::SanitizeFloat(CharacterWaitTime); },
				[this](const FString& InValue) { CharacterWaitTime = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"AutoModeWaitTime",
				EShidenVariableType::Float,
				"3.0",
				false,
				[this] { return FString::SanitizeFloat(AutoModeWaitTime); },
				[this](const FString& InValue) { AutoModeWaitTime = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"ShouldWaitForVoiceInAutoMode",
				EShidenVariableType::Boolean,
				"true",
				false,
				[this] { return bShouldWaitForVoiceInAutoMode ? TEXT("true") : TEXT("false"); },
				[this](const FString& InValue) { bShouldWaitForVoiceInAutoMode = InValue.Compare("true", ESearchCase::IgnoreCase) == 0; }
			),
			FShidenPredefinedSystemVariableDefinition(
				"VoiceStopCondition",
				EShidenVariableType::String,
				"Never",
				false,
				[this] { return StaticEnum<EShidenVoiceStopCondition>()->GetDisplayValueAsText(VoiceStopCondition).ToString(); },
				[this](const FString& InValue)
				{
					if (InValue.Compare("NextVoice", ESearchCase::IgnoreCase) == 0)
					{
						VoiceStopCondition = EShidenVoiceStopCondition::NextVoice;
						return;
					}
					if (InValue.Compare("NextTextOrVoice", ESearchCase::IgnoreCase) == 0)
					{
						VoiceStopCondition = EShidenVoiceStopCondition::NextTextOrVoice;
						return;
					}
					VoiceStopCondition = EShidenVoiceStopCondition::Never;
				}
			),
			FShidenPredefinedSystemVariableDefinition(
				"SkipCondition",
				EShidenVariableType::String,
				"ReadOnly",
				false,
				[this] { return StaticEnum<EShidenSkipCondition>()->GetDisplayValueAsText(SkipCondition).ToString(); },
				[this](const FString& InValue)
				{
					if (InValue.Compare("ReadOnly", ESearchCase::IgnoreCase) == 0)
					{
						SkipCondition = EShidenSkipCondition::ReadOnly;
						return;
					}
					if (InValue.Compare("All", ESearchCase::IgnoreCase) == 0)
					{
						SkipCondition = EShidenSkipCondition::All;
						return;
					}
					SkipCondition = EShidenSkipCondition::None;
				}
			),
			FShidenPredefinedSystemVariableDefinition(
				"ClickWaitingGlyph",
				EShidenVariableType::String,
				"",
				false,
				[this] { return ClickWaitingGlyph; },
				[this](const FString& InValue) { ClickWaitingGlyph = InValue; }
			),
			FShidenPredefinedSystemVariableDefinition(
				"PlatformName",
				EShidenVariableType::String,
				"Windows",
				true,
				[this] { return PlatformName; },
				[this](const FString&) { }
			)
	};
};