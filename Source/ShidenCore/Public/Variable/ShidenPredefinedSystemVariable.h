// Copyright (c) 2024 HANON. All Rights Reserved.

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
	float MasterVolumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float BgmVolumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float SeVolumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shiden Visual Novel|Predefined System Variables")
	float VoiceVolumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	int32 LanguageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float SecondsToWaitForEachLetter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	float WaitTimeInAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Predefined System Variables")
	bool bWaitForVoiceInAutoMode;
	
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
			VoiceVolumeRate = Other.VoiceVolumeRate;
			SeVolumeRate = Other.SeVolumeRate;
			MasterVolumeRate = Other.MasterVolumeRate;
			BgmVolumeRate = Other.BgmVolumeRate;
			LanguageIndex = Other.LanguageIndex;
			SecondsToWaitForEachLetter = Other.SecondsToWaitForEachLetter;
			WaitTimeInAutoMode = Other.WaitTimeInAutoMode;
			bWaitForVoiceInAutoMode = Other.bWaitForVoiceInAutoMode;
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
				"10.0",
				false,
				[this] { return FString::SanitizeFloat(SkipSpeedRate); },
				[this](const FString& InValue) { SkipSpeedRate = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"MasterVolumeRate",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(MasterVolumeRate); },
				[this](const FString& InValue) { MasterVolumeRate = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"BgmVolumeRate",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(BgmVolumeRate); },
				[this](const FString& InValue) { BgmVolumeRate = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"SeVolumeRate",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(SeVolumeRate); },
				[this](const FString& InValue) { SeVolumeRate = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"VoiceVolumeRate",
				EShidenVariableType::Float,
				"1.0",
				false,
				[this] { return FString::SanitizeFloat(VoiceVolumeRate); },
				[this](const FString& InValue) { VoiceVolumeRate = FCString::Atof(*InValue); }
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
				"SecondsToWaitForEachLetter",
				EShidenVariableType::Float,
				"0.08",
				false,
				[this] { return FString::SanitizeFloat(SecondsToWaitForEachLetter); },
				[this](const FString& InValue) { SecondsToWaitForEachLetter = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"WaitTimeInAutoMode",
				EShidenVariableType::Float,
				"3.0",
				false,
				[this] { return FString::SanitizeFloat(WaitTimeInAutoMode); },
				[this](const FString& InValue) { WaitTimeInAutoMode = FCString::Atof(*InValue); }
			),
			FShidenPredefinedSystemVariableDefinition(
				"WaitForVoiceInAutoMode",
				EShidenVariableType::Boolean,
				"true",
				false,
				[this] { return bWaitForVoiceInAutoMode ? TEXT("true") : TEXT("false"); },
				[this](const FString& InValue) { bWaitForVoiceInAutoMode = InValue.Compare("true", ESearchCase::IgnoreCase) == 0; }
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
					if (InValue.Compare("Always", ESearchCase::IgnoreCase) == 0)
					{
						SkipCondition = EShidenSkipCondition::Always;
						return;
					}
					SkipCondition = EShidenSkipCondition::Never;
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