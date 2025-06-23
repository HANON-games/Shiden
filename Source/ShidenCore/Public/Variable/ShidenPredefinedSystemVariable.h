// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenPredefinedSystemVariableDefinition.h"
#include "ShidenVariableDescriptor.h"
#include "Audio/ShidenTextBlipTriggerMode.h"
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
	float BGMVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float SEVolume;

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
	EShidenTextBlipTriggerMode TextBlipTriggerMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float BlipIntervalSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	int32 BlipIntervalCharacters;
	
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
		if (this != &Other)
		{
			SkipSpeedRate = Other.SkipSpeedRate;
			VoiceVolume = Other.VoiceVolume;
			SEVolume = Other.SEVolume;
			MasterVolume = Other.MasterVolume;
			BGMVolume = Other.BGMVolume;
			LanguageIndex = Other.LanguageIndex;
			CharacterWaitTime = Other.CharacterWaitTime;
			AutoModeWaitTime = Other.AutoModeWaitTime;
			bShouldWaitForVoiceInAutoMode = Other.bShouldWaitForVoiceInAutoMode;
			VoiceStopCondition = Other.VoiceStopCondition;
			TextBlipTriggerMode = Other.TextBlipTriggerMode;
			BlipIntervalSeconds = Other.BlipIntervalSeconds;
			BlipIntervalCharacters = Other.BlipIntervalCharacters;
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
			EShidenAssetPathType::None,
			"30.0",
			false,
			[this] { return FString::SanitizeFloat(SkipSpeedRate); },
			[this](const FString&  Value) { SkipSpeedRate = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"MasterVolume",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"1.0",
			false,
			[this] { return FString::SanitizeFloat(MasterVolume); },
			[this](const FString&  Value) { MasterVolume = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"BGMVolume",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"1.0",
			false,
			[this] { return FString::SanitizeFloat(BGMVolume); },
			[this](const FString&  Value) { BGMVolume = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"SEVolume",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"1.0",
			false,
			[this] { return FString::SanitizeFloat(SEVolume); },
			[this](const FString&  Value) { SEVolume = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"VoiceVolume",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"1.0",
			false,
			[this] { return FString::SanitizeFloat(VoiceVolume); },
			[this](const FString&  Value) { VoiceVolume = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"LanguageIndex",
			EShidenVariableType::Integer,
			EShidenAssetPathType::None,
			"0",
			false,
			[this] { return FString::FromInt(LanguageIndex); },
			[this](const FString&  Value) { LanguageIndex = FCString::Atoi(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"CharacterWaitTime",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"0.05",
			false,
			[this] { return FString::SanitizeFloat(CharacterWaitTime); },
			[this](const FString&  Value) { CharacterWaitTime = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"AutoModeWaitTime",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"3.0",
			false,
			[this] { return FString::SanitizeFloat(AutoModeWaitTime); },
			[this](const FString&  Value) { AutoModeWaitTime = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"ShouldWaitForVoiceInAutoMode",
			EShidenVariableType::Boolean,
			EShidenAssetPathType::None,
			"true",
			false,
			[this] { return bShouldWaitForVoiceInAutoMode ? TEXT("true") : TEXT("false"); },
			[this](const FString&  Value) { bShouldWaitForVoiceInAutoMode =  Value.Compare("true", ESearchCase::IgnoreCase) == 0; }
		),
		FShidenPredefinedSystemVariableDefinition(
			"VoiceStopCondition",
			EShidenVariableType::String,
			EShidenAssetPathType::None,
			"Never",
			false,
			[this] { return StaticEnum<EShidenVoiceStopCondition>()->GetDisplayValueAsText(VoiceStopCondition).ToString(); },
			[this](const FString&  Value)
			{
				if ( Value.Compare("NextVoice", ESearchCase::IgnoreCase) == 0)
				{
					VoiceStopCondition = EShidenVoiceStopCondition::NextVoice;
					return;
				}
				if ( Value.Compare("NextTextOrVoice", ESearchCase::IgnoreCase) == 0)
				{
					VoiceStopCondition = EShidenVoiceStopCondition::NextTextOrVoice;
					return;
				}
				VoiceStopCondition = EShidenVoiceStopCondition::Never;
			}
		),
		FShidenPredefinedSystemVariableDefinition(
			"TextBlipTriggerMode",
			EShidenVariableType::String,
			EShidenAssetPathType::None,
			"TimeInterval",
			false,
			[this] { return StaticEnum<EShidenTextBlipTriggerMode>()->GetDisplayValueAsText(TextBlipTriggerMode).ToString(); },
			[this](const FString&  Value)
			{
				if ( Value.Compare("TimeInterval", ESearchCase::IgnoreCase) == 0)
				{
					TextBlipTriggerMode = EShidenTextBlipTriggerMode::TimeInterval;
					return;
				}

				TextBlipTriggerMode = EShidenTextBlipTriggerMode::CharacterInterval;
			}
		),
		FShidenPredefinedSystemVariableDefinition(
			"BlipIntervalSeconds",
			EShidenVariableType::Float,
			EShidenAssetPathType::None,
			"0.1",
			false,
			[this] { return FString::SanitizeFloat(BlipIntervalSeconds); },
			[this](const FString&  Value) { BlipIntervalSeconds = FCString::Atof(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"BlipIntervalCharacters",
			EShidenVariableType::Integer,
			EShidenAssetPathType::None,
			"2",
			false,
			[this] { return FString::FromInt(BlipIntervalCharacters); },
			[this](const FString&  Value) { BlipIntervalCharacters = FCString::Atoi(* Value); }
		),
		FShidenPredefinedSystemVariableDefinition(
			"SkipCondition",
			EShidenVariableType::String,
			EShidenAssetPathType::None,
			"ReadOnly",
			false,
			[this] { return StaticEnum<EShidenSkipCondition>()->GetDisplayValueAsText(SkipCondition).ToString(); },
			[this](const FString&  Value)
			{
				if ( Value.Compare("ReadOnly", ESearchCase::IgnoreCase) == 0)
				{
					SkipCondition = EShidenSkipCondition::ReadOnly;
					return;
				}
				if ( Value.Compare("All", ESearchCase::IgnoreCase) == 0)
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
			EShidenAssetPathType::None,
			"",
			false,
			[this] { return ClickWaitingGlyph; },
			[this](const FString&  Value) { ClickWaitingGlyph =  Value; }
		),
		FShidenPredefinedSystemVariableDefinition(
			"PlatformName",
			EShidenVariableType::String,
			EShidenAssetPathType::None,
			"Windows",
			true,
			[this] { return PlatformName; },
			[this](const FString&)
			{
			}
		)
	};
};
