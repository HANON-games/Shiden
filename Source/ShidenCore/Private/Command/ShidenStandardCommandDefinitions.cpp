// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/ShidenStandardCommandDefinitions.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

const FShidenCommandStyle UShidenStandardCommandDefinitions::TextStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.15f, 0.15f, 0.15f),
	FLinearColor(0.15f, 0.15f, 0.15f),
	FLinearColor(0.15f, 0.15f, 0.15f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::MediaStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.2f, 0.1f, 0.0f),
	FLinearColor(0.25f, 0.125f, 0.0f),
	FLinearColor(0.25f, 0.125f, 0.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::FlowControlStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.0f, 0.0f, 0.15f),
	FLinearColor(0.0f, 0.0f, 0.2f),
	FLinearColor(0.0f, 0.0f, 0.2f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::UIControlStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.0f, 0.2f, 0.0f),
	FLinearColor(0.0f, 0.25f, 0.0f),
	FLinearColor(0.0f, 0.25f, 0.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::DataStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.0f, 0.15f, 0.20f),
	FLinearColor(0.0f, 0.175f, 0.25f),
	FLinearColor(0.0f, 0.175f, 0.25f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::MacroStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.05f, 0.05f, 0.05f),
	FLinearColor(0.07f, 0.07f, 0.07f),
	FLinearColor(0.07f, 0.07f, 0.07f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::CommentStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.25f, 0.25f, 0.0f),
	FLinearColor(0.3f, 0.3f, 0.0f),
	FLinearColor(0.3f, 0.3f, 0.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::TagStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.2f, 0.0f, 0.0f),
	FLinearColor(0.25f, 0.0f, 0.0f),
	FLinearColor(0.25f, 0.0f, 0.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::InteractionStyle = FShidenCommandStyle{
	FLinearColor::White,
	FLinearColor(0.2f, 0.0f, 0.20f),
	FLinearColor(0.25f, 0.0f, 0.25f),
	FLinearColor(0.25f, 0.0f, 0.25f)
};

const FSoftObjectPath UShidenStandardCommandDefinitions::TextInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenTextInput.EUW_ShidenTextInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::TextTypeInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenTextTypeInput.EUW_ShidenTextTypeInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::MultilineTextInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenMultilineTextInput.EUW_ShidenMultilineTextInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::BooleanInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenBooleanInput.EUW_ShidenBooleanInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::IntegerInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenIntegerInput.EUW_ShidenIntegerInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::FloatInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenFloatInput.EUW_ShidenFloatInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::Vector2InputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenVector2Input.EUW_ShidenVector2Input"));

const FSoftObjectPath UShidenStandardCommandDefinitions::ColorInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenColorInput.EUW_ShidenColorInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::VariableKindInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenVariableKindInput.EUW_ShidenVariableKindInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::VariableNameInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenVariableNameInput.EUW_ShidenVariableNameInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::FlexInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenFlexInput.EUW_ShidenFlexInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::ComboBoxInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenComboBoxInput.EUW_ShidenComboBoxInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::AssetInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenAssetInput.EUW_ShidenAssetInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::SoundInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenSoundInput.EUW_ShidenSoundInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::SoundTypeInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenSoundTypeInput.EUW_ShidenSoundTypeInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::BGMFadeFuncInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenBgmFadeFuncInput.EUW_ShidenBgmFadeFuncInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::EasingFuncInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenEasingFuncInput.EUW_ShidenEasingFuncInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::FlexCalculationOperatorInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenFlexCalculationOperatorInput.EUW_ShidenFlexCalculationOperatorInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::FadeTypeInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenFadeTypeInput.EUW_ShidenFadeTypeInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::FlexOperatorInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenFlexOperatorInput.EUW_ShidenFlexOperatorInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::FlexNumberInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenFlexNumberInput.EUW_ShidenFlexNumberInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::ScenarioInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenScenarioInput.EUW_ShidenScenarioInput"));

const FSoftObjectPath UShidenStandardCommandDefinitions::SlateVisibilityInputTemplate =
	FSoftObjectPath(TEXT("/Shiden/Editor/Inputs/EUW_ShidenSlateVisibilityInput.EUW_ShidenSlateVisibilityInput"));

UShidenStandardCommandDefinitions::UShidenStandardCommandDefinitions()
{
	CommandDefinitions =
	{
		{
			TEXT("Text"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("TextNoteKey",
				                "You can insert variable values using { variable_name }.\r\nIf \":\" is included in the variable name, escape it as \"\\:\".\r\nThe prefixes for variables are as follows:\r\n* \"System::\": System variables\r\n* \"Local::\": Local variables\r\n* \"Predefined::\": Predefined system variables\r\nThe track ID for Voice will be 0.\r\nIf both Voice and DialogueBlip are specified, Voice takes precedence."),
				.Style = TextStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenTextCommand")),
				.Args =
				{
					{
						TEXT("TextWidgetName"),
						LOCTEXT("TextWidgetNameKey", "TextWidgetName"),
						TEXT("Talk"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("TextType"),
						LOCTEXT("TextTypeKey", "TextType"),
						TEXT("Default"),
						TextTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("Text"),
						LOCTEXT("TextKey", "Text"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("WaitForInput"),
						LOCTEXT("WaitForInputKey", "WaitForInput"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("InstantTextDisplay"),
						LOCTEXT("InstantTextDisplayKey", "InstantTextDisplay"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("Voice"),
						LOCTEXT("VoiceKey", "Voice"),
						TEXT("None"),
						SoundInputTemplate,
						{{TEXT("Target"), TEXT("Voice")}},
						true
					},
					{
						TEXT("VoiceTrackId"),
						LOCTEXT("VoiceTrackIdKey", "VoiceTrackId"),
						TEXT("0"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("TextBlip"),
						LOCTEXT("DialogueBlipKey", "TextBlip"),
						TEXT("None"),
						SoundInputTemplate,
						{{TEXT("Target"), TEXT("Voice")}},
						true
					},
					{
						TEXT("DisableAutoStopPreviousVoices"),
						LOCTEXT("DisableAutoStopPreviousVoicesKey", "DisableAutoStop\r\nPreviousVoices"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("ContinueFromThePreviousText"),
						LOCTEXT("ContinueFromThePreviousTextKey", "ContinueFrom\r\nThePreviousText"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2"),
						LOCTEXT("Language2Key", "Language 2"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 3"),
						LOCTEXT("Language3Key", "Language 3"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 4"),
						LOCTEXT("Language4Key", "Language 4"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 5"),
						LOCTEXT("Language5Key", "Language 5"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 6"),
						LOCTEXT("Language6Key", "Language 6"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 7"),
						LOCTEXT("Language7Key", "Language 7"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 8"),
						LOCTEXT("Language8Key", "Language 8"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 9"),
						LOCTEXT("Language9Key", "Language 9"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 10"),
						LOCTEXT("Language10Key", "Language 10"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		},
		{
			TEXT("Image"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("ImageNoteKey",
				                "If no image is specified, the values of Position, Size,\r\nand SizeToContent are ignored, and the current image fades out.\r\nSteps is used only when the FadeFunction is Step.\r\nBlend Exp is used only when the FadeFunction is Ease.\r\nFor SlateBrush created from Material, fading does not work,\r\nso please change the opacity separately."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenImageCommand")),
				.Args =
				{
					{
						TEXT("SlotName"),
						LOCTEXT("SlotNameKey", "SlotName"),
						TEXT("Background"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Image"),
						LOCTEXT("ImageKey", "Image"),
						TEXT("None"),
						AssetInputTemplate,
						{{TEXT("Target"), TEXT("SlateBrush")}},
						true
					},
					{
						TEXT("OverwritePosition"),
						LOCTEXT("OverwritePositionKey", "OverwritePosition"),
						TEXT(""),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteSize"),
						LOCTEXT("OverwriteSizeKey", "OverwriteSize"),
						TEXT(""),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteSizeToContent"),
						LOCTEXT("OverwriteSizeToContentKey", "OverwriteSizeToContent"),
						TEXT(""),
						BooleanInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("FadeFunction"),
						LOCTEXT("FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						LOCTEXT("FadeDurationKey", "FadeDuration"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("Steps"),
						LOCTEXT("StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						LOCTEXT("BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Sound"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("SoundNoteKey",
				                "Track ID is not effective for SE.\r\nYou cannot stop a playing SE midway.\r\nYou can stop it by leaving the sound source field empty.\r\nThe value for DisableAutoStopPreviousVoices is only effective for Voice.\r\nWhen fading out the currently playing BGM or voice, the values of Volume and Pitch are ignored.\r\nFor MetaSound Source, it seems that the Start Time value is not reflected."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenSoundCommand")),
				.Args =
				{
					{
						TEXT("SoundType"),
						LOCTEXT("SoundTypeKey", "SoundType"),
						TEXT("BGM"),
						SoundTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("TrackId"),
						LOCTEXT("TrackIdKey", "TrackId"),
						TEXT("0"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("SoundSource"),
						LOCTEXT("SoundSourceKey", "SoundSource"),
						TEXT("None"),
						SoundInputTemplate,
						{{TEXT("SoundTypeSourceIndex"), TEXT("0")}},
						true
					},
					{
						TEXT("Volume"),
						LOCTEXT("VolumeKey", "Volume"),
						TEXT("1.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("Pitch"),
						LOCTEXT("PitchKey", "Pitch"),
						TEXT("1.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("StartTime"),
						LOCTEXT("StartTimeKey", "StartTime"),
						TEXT("0.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeType"),
						LOCTEXT("FadeTypeKey", "FadeType"),
						TEXT("FadeIn"),
						FadeTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeFunction"),
						LOCTEXT("FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						BGMFadeFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						LOCTEXT("FadeDurationKey", "FadeDuration"),
						TEXT("0.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("DisableAutoStopPreviousVoices"),
						LOCTEXT("DisableAutoStopPreviousVoicesKey", "DisableAutoStop\r\nPreviousVoices"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForFadeCompletion"),
						LOCTEXT("WaitForFadeCompletionKey", "WaitForFadeCompletion"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForSoundCompletion"),
						LOCTEXT("WaitForSoundCompletionKey", "WaitForSoundCompletion"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("AdjustBGMVolume"),
			FShidenCommandDefinition
			{
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAdjustBGMVolumeCommand")),
				.Args =
				{
					{
						TEXT("TrackId"),
						LOCTEXT("TrackIdKey", "TrackId"),
						TEXT("0"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("Volume"),
						LOCTEXT("VolumeKey", "Volume"),
						TEXT("1.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeFunction"),
						LOCTEXT("FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						BGMFadeFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						LOCTEXT("FadeDurationKey", "FadeDuration"),
						TEXT("0.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Options"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("OptionsNoteKey", "Only Integer type variables can be specified for DestinationVariableName."),
				.Style = InteractionStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenOptionsCommand")),
				.Args =
				{
					{
						TEXT("DestinationVariableKind"),
						LOCTEXT("DestinationVariableKindKey", "DestinationVariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableName"),
						LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{
								{TEXT("VariableKindSourceIndex"), TEXT("5")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")},
								{TEXT("AllowedVariableTypesForLiteral"), TEXT("Integer")}
						},
						false
					},
					{
						TEXT("HideTextLayer"),
						LOCTEXT("HideTextLayerKey", "HideTextLayer"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("Option 1"),
						LOCTEXT("Option1Key", "Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Option 2"),
						LOCTEXT("Option2Key", "Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Option 3"),
						LOCTEXT("Option3Key", "Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Option 4"),
						LOCTEXT("Option4Key", "Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Option 5"),
						LOCTEXT("Option5Key", "Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 2 Option 1"),
						LOCTEXT("Language2Option1Key", "Language 2 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 2 Option 2"),
						LOCTEXT("Language2Option2Key", "Language 2 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 2 Option 3"),
						LOCTEXT("Language2Option3Key", "Language 2 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 2 Option 4"),
						LOCTEXT("Language2Option4Key", "Language 2 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 2 Option 5"),
						LOCTEXT("Language2Option5Key", "Language 2 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 3 Option 1"),
						LOCTEXT("Language3Option1Key", "Language 3 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 3 Option 2"),
						LOCTEXT("Language3Option2Key", "Language 3 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 3 Option 3"),
						LOCTEXT("Language3Option3Key", "Language 3 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 3 Option 4"),
						LOCTEXT("Language3Option4Key", "Language 3 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 3 Option 5"),
						LOCTEXT("Language3Option5Key", "Language 3 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 4 Option 1"),
						LOCTEXT("Language4Option1Key", "Language 4 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 4 Option 2"),
						LOCTEXT("Language4Option2Key", "Language 4 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 4 Option 3"),
						LOCTEXT("Language4Option3Key", "Language 4 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 4 Option 4"),
						LOCTEXT("Language4Option4Key", "Language 4 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 4 Option 5"),
						LOCTEXT("Language4Option5Key", "Language 4 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 5 Option 1"),
						LOCTEXT("Language5Option1Key", "Language 5 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 5 Option 2"),
						LOCTEXT("Language5Option2Key", "Language 5 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 5 Option 3"),
						LOCTEXT("Language5Option3Key", "Language 5 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 5 Option 4"),
						LOCTEXT("Language5Option4Key", "Language 5 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 5 Option 5"),
						LOCTEXT("Language5Option5Key", "Language 5 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 6 Option 1"),
						LOCTEXT("Language6Option1Key", "Language 6 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 6 Option 2"),
						LOCTEXT("Language6Option2Key", "Language 6 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 6 Option 3"),
						LOCTEXT("Language6Option3Key", "Language 6 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 6 Option 4"),
						LOCTEXT("Language6Option4Key", "Language 6 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 6 Option 5"),
						LOCTEXT("Language6Option5Key", "Language 6 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 7 Option 1"),
						LOCTEXT("Language7Option1Key", "Language 7 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 7 Option 2"),
						LOCTEXT("Language7Option2Key", "Language 7 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 7 Option 3"),
						LOCTEXT("Language7Option3Key", "Language 7 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 7 Option 4"),
						LOCTEXT("Language7Option4Key", "Language 7 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 7 Option 5"),
						LOCTEXT("Language7Option5Key", "Language 7 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 8 Option 1"),
						LOCTEXT("Language8Option1Key", "Language 8 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 8 Option 2"),
						LOCTEXT("Language8Option2Key", "Language 8 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 8 Option 3"),
						LOCTEXT("Language8Option3Key", "Language 8 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 8 Option 4"),
						LOCTEXT("Language8Option4Key", "Language 8 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 8 Option 5"),
						LOCTEXT("Language8Option5Key", "Language 8 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 9 Option 1"),
						LOCTEXT("Language9Option1Key", "Language 9 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 9 Option 2"),
						LOCTEXT("Language9Option2Key", "Language 9 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 9 Option 3"),
						LOCTEXT("Language9Option3Key", "Language 9 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 9 Option 4"),
						LOCTEXT("Language9Option4Key", "Language 9 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 9 Option 5"),
						LOCTEXT("Language9Option5Key", "Language 9 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 10 Option 1"),
						LOCTEXT("Language10Option1Key", "Language 10 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 10 Option 2"),
						LOCTEXT("Language10Option2Key", "Language 10 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 10 Option 3"),
						LOCTEXT("Language10Option3Key", "Language 10 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 10 Option 4"),
						LOCTEXT("Language10Option4Key", "Language 10 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Language 10 Option 5"),
						LOCTEXT("Language10Option5Key", "Language 10 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
				}
			}
		},
		{
			TEXT("TextInput"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("TextInputNoteKey",
				                "You can input patterns like the following in AllowedCharacterRegex.\r\n- Allow only numbers: [0-9]\r\n- Allow only abc: [abc]\r\n- Allow anything except numbers: [^0-9]"),
				.Style = InteractionStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenTextInputCommand")),
				.Args =
				{
					{
						TEXT("MaxLength"),
						LOCTEXT("MaxLengthKey", "MaxLength"),
						TEXT(""),
						IntegerInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("MaxLineCount"),
						LOCTEXT("MaxLineCountKey", "MaxLineCount"),
						TEXT("1"),
						IntegerInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("AllowedCharacterRegex"),
						LOCTEXT("AllowedCharacterRegexKey", "AllowedCharacterRegex"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("DefaultText"),
						LOCTEXT("DefaultTextKey", "DefaultText"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("HintText"),
						LOCTEXT("HintTextKey", "HintText"),
						TEXT(""),
						MultilineTextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("DestinationVariableKind"),
						LOCTEXT("DestinationVariableKindKey", "DestinationVariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableName"),
						LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("5")},
							{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")}
						},
						false
					},
					{
						TEXT("HideTextLayer"),
						LOCTEXT("HideTextLayerKey", "HideTextLayer"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("AssignVariable"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("AssignVariableNoteKey", "Spaces cannot be used in the key."),
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAssignVariableCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")}
						},
						false
					},
					{
						TEXT("Value"),
						LOCTEXT("ValueKey", "Value"),
						TEXT(""),
						FlexInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("AssignExpression"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("AssignExpressionNoteKey",
				                "Assigns the result of evaluating an expression to a variable.\r\n\r\nThe following data types are supported:\r\n• Integer: Integer numbers\r\n• Float: Floating-point numbers\r\n• String: Text strings\r\n• Boolean: Boolean values\r\n• Vector2: 2D vectors\r\n• Vector3: 3D vectors\r\n\r\nStrings must be enclosed in double quotes like \"Hello\".\r\nBoolean values are expressed as true or false.\r\nVector2 is expressed in the format [x, y], and Vector3 is expressed in the format [x, y, z].\r\n\r\nThe following arithmetic operators are supported:\r\n• +: Addition\r\n• -: Subtraction\r\n• *: Multiplication\r\n• /: Division\r\n• %: Modulo\r\n• **: Exponentiation\r\n\r\nThe following comparison operators are supported:\r\n• ==: Equal to\r\n• !=: Not equal to\r\n• >: Greater than\r\n• <: Less than\r\n• >=: Greater than or equal to\r\n• <=: Less than or equal to\r\n\r\nThe following logical operators are supported:\r\n• &&: AND\r\n• ||: OR\r\n• !: NOT\r\n\r\nYou can insert variable values with { variable_name }.\r\nIf the variable name contains \":\", escape it as \"\\:\".\r\nVariable prefixes are as follows:\r\n• \"System::\": System variables\r\n• \"Local::\": Local variables\r\n• \"Predefined::\": Predefined system variables"),
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAssignExpressionCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")}
						},
						false
					},
					{
						TEXT("Expression"),
						LOCTEXT("ExpressionKey", "Expression"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		},
		{
			TEXT("Calculation"),
			FShidenCommandDefinition
			{
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenCalculationCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")},
							{TEXT("AllowedVariableTypesForLiteral"), TEXT("String,Integer,Float,Vector2,Vector3")}
						},
						false
					},
					{
						TEXT("Operator"),
						LOCTEXT("OperatorKey", "Operator"),
						TEXT(""),
						FlexCalculationOperatorInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					},
					{
						TEXT("Value"),
						LOCTEXT("ValueKey", "Value"),
						TEXT(""),
						FlexInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("If"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("IfNoteKey", "Evaluates an expression and branches the process based on the result."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenIfCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						LOCTEXT("OperatorKey", "Operator"),
						TEXT(""),
						FlexOperatorInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					},
					{
						TEXT("RightHandValue"),
						LOCTEXT("RightHandValueKey", "RightHandValue"),
						TEXT(""),
						FlexInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("ElseIf"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("ElseIfNoteKey", "Evaluates an expression and branches the process based on the result."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenElseIfCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						LOCTEXT("OperatorKey", "Operator"),
						TEXT(""),
						FlexOperatorInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					},
					{
						TEXT("RightHandValue"),
						LOCTEXT("RightHandValueKey", "RightHandValue"),
						TEXT(""),
						FlexInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("IfExpression"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("IfExpressionNoteKey",
				                "Evaluates an expression and branches the process based on the result.\r\n\r\nThe following data types are supported:\r\n• Integer: Integer numbers\r\n• Float: Floating-point numbers\r\n• String: Text strings\r\n• Boolean: Boolean values\r\n• Vector2: 2D vectors\r\n• Vector3: 3D vectors\r\n\r\nStrings must be enclosed in double quotes like \"Hello\".\r\nBoolean values are expressed as true or false.\r\nVector2 is expressed in the format [x, y], and Vector3 is expressed in the format [x, y, z].\r\n\r\nThe following arithmetic operators are supported:\r\n• +: Addition\r\n• -: Subtraction\r\n• *: Multiplication\r\n• /: Division\r\n• %: Modulo\r\n• **: Exponentiation\r\n\r\nThe following comparison operators are supported:\r\n• ==: Equal to\r\n• !=: Not equal to\r\n• >: Greater than\r\n• <: Less than\r\n• >=: Greater than or equal to\r\n• <=: Less than or equal to\r\n\r\nThe following logical operators are supported:\r\n• &&: AND\r\n• ||: OR\r\n• !: NOT\r\n\r\nYou can insert variable values with { variable_name }.\r\nIf the variable name contains \":\", escape it as \"\\:\".\r\nVariable prefixes are as follows:\r\n• \"System::\": System variables\r\n• \"Local::\": Local variables\r\n• \"Predefined::\": Predefined system variables"),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenIfExpressionCommand")),
				.Args =
				{
					{
						TEXT("Expression"),
						LOCTEXT("ExpressionKey", "Expression"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		},
		{
			TEXT("ElseIfExpression"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("ElseIfExpressionNoteKey",
				                "Evaluates an expression and branches the process based on the result.\r\n\r\nThe following data types are supported:\r\n• Integer: Integer numbers\r\n• Float: Floating-point numbers\r\n• String: Text strings\r\n• Boolean: Boolean values\r\n• Vector2: 2D vectors\r\n• Vector3: 3D vectors\r\n\r\nStrings must be enclosed in double quotes like \"Hello\".\r\nBoolean values are expressed as true or false.\r\nVector2 is expressed in the format [x, y], and Vector3 is expressed in the format [x, y, z].\r\n\r\nThe following arithmetic operators are supported:\r\n• +: Addition\r\n• -: Subtraction\r\n• *: Multiplication\r\n• /: Division\r\n• %: Modulo\r\n• **: Exponentiation\r\n\r\nThe following comparison operators are supported:\r\n• ==: Equal to\r\n• !=: Not equal to\r\n• >: Greater than\r\n• <: Less than\r\n• >=: Greater than or equal to\r\n• <=: Less than or equal to\r\n\r\nThe following logical operators are supported:\r\n• &&: AND\r\n• ||: OR\r\n• !: NOT\r\n\r\nYou can insert variable values with { variable_name }.\r\nIf the variable name contains \":\", escape it as \"\\:\".\r\nVariable prefixes are as follows:\r\n• \"System::\": System variables\r\n• \"Local::\": Local variables\r\n• \"Predefined::\": Predefined system variables"),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenElseIfExpressionCommand")),
				.Args =
				{
					{
						TEXT("Expression"),
						LOCTEXT("ExpressionKey", "Expression"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		},
		{
			TEXT("Else"),
			FShidenCommandDefinition
			{
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenElseCommand"))
			}
		},
		{
			TEXT("EndIf"),
			FShidenCommandDefinition
			{
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenEndIfCommand"))
			}
		},
		{
			TEXT("LoopWhile"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("LoopWhileNoteKey", "Evaluates an expression and loops if the result is true.\r\nThis command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenLoopWhileCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						LOCTEXT("OperatorKey", "Operator"),
						TEXT(""),
						FlexOperatorInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					},
					{
						TEXT("RightHandValue"),
						LOCTEXT("RightHandValueKey", "RightHandValue"),
						TEXT(""),
						FlexInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("LoopWhileExpression"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("LoopWhileExpressionNoteKey",
				                "Evaluates an expression and loops if the result is true.\r\n\r\nThe following data types are supported:\r\n• Integer: Integer numbers\r\n• Float: Floating-point numbers\r\n• String: Text strings\r\n• Boolean: Boolean values\r\n• Vector2: 2D vectors\r\n• Vector3: 3D vectors\r\n\r\nStrings must be enclosed in double quotes like \"Hello\".\r\nBoolean values are expressed as true or false.\r\nVector2 is expressed in the format [x, y], and Vector3 is expressed in the format [x, y, z].\r\n\r\nThe following arithmetic operators are supported:\r\n• +: Addition\r\n• -: Subtraction\r\n• *: Multiplication\r\n• /: Division\r\n• %: Modulo\r\n• **: Exponentiation\r\n\r\nThe following comparison operators are supported:\r\n• ==: Equal to\r\n• !=: Not equal to\r\n• >: Greater than\r\n• <: Less than\r\n• >=: Greater than or equal to\r\n• <=: Less than or equal to\r\n\r\nThe following logical operators are supported:\r\n• &&: AND\r\n• ||: OR\r\n• !: NOT\r\n\r\nYou can insert variable values with { variable_name }.\r\nIf the variable name contains \":\", escape it as \"\\:\".\r\nVariable prefixes are as follows:\r\n• \"System::\": System variables\r\n• \"Local::\": Local variables\r\n• \"Predefined::\": Predefined system variables\r\n\r\nThis command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenLoopWhileExpressionCommand")),
				.Args =
				{
					{
						TEXT("Expression"),
						LOCTEXT("ExpressionKey", "Expression"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		},
		{
			TEXT("EndLoopWhile"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("EndLoopWhileNoteKey", "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenEndLoopWhileCommand"))
			}
		},
		{
			TEXT("Tag"),
			FShidenCommandDefinition
			{
				.Style = TagStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenTagCommand")),
				.Args =
				{
					{
						TEXT("Name"),
						LOCTEXT("NameKey", "Name"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Jump"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("JumpNoteKey", "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenJumpCommand")),
				.Args =
				{
					{
						TEXT("DestinationTagName"),
						LOCTEXT("DestinationTagNameKey", "DestinationTagName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("EndScenario"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("EndScenarioNoteKey",
				                "In a macro, the next scenario specification is ignored,\r\nand it resumes from the continuation of the original scenario.\r\nThis command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenEndScenarioCommand")),
				.Args =
				{
					{
						TEXT("NextScenario"),
						LOCTEXT("NextScenarioKey", "NextScenario"),
						TEXT("None"),
						ScenarioInputTemplate,
						{
							{TEXT("Target"), TEXT("Scenario")},
							{TEXT("HasToggle"), TEXT("true")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("Break"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("BreakNoteKey", "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenBreakCommand")),
				.Args =
				{
					{
						TEXT("ReasonForInterruption"),
						LOCTEXT("ReasonForInterruptionKey", "ReasonForInterruption"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Comment"),
			FShidenCommandDefinition
			{
				.Style = CommentStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenCommentCommand")),
				.Args =
				{
					{
						TEXT("Comment"),
						LOCTEXT("CommentKey", "Comment"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("WidgetAnimation"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("WidgetAnimationNoteKey",
				                "You can specify the animation of a child widget as \"ChildWidgetName.AnimationName\".\nWhen RestoreState is true, the behavior may differ between gameplay and preview."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenWidgetAnimationCommand")),
				.Args =
				{
					{
						TEXT("AnimationName"),
						LOCTEXT("AnimationNameKey", "AnimationName"),
						TEXT("Shake"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("StartTime"),
						LOCTEXT("StartTimeKey", "StartTime"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("NumLoopToPlay"),
						LOCTEXT("NumLoopToPlayKey", "NumLoopToPlay"),
						TEXT("1"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("PlayMode"),
						LOCTEXT("PlayModeKey", "PlayMode"),
						TEXT("Forward"),
						ComboBoxInputTemplate,
						{
							{TEXT("Option1"), TEXT("Forward")},
							{TEXT("Option2"), TEXT("Reverse")},
							{TEXT("Option3"), TEXT("Ping Pong")}
						},
						false
					},
					{
						TEXT("PlaybackSpeed"),
						LOCTEXT("PlaybackSpeedKey", "PlaybackSpeed"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("RestoreState"),
						LOCTEXT("RestoreStateKey", "RestoreState"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Delay"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("DelayNoteKey", "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenDelayCommand")),
				.Args =
				{
					{
						TEXT("Duration"),
						LOCTEXT("DurationKey", "Duration"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("CallWidgetFunction"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("CallWidgetFunctionNoteKey", "Arg cannot be left empty midway."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenCallWidgetFunctionCommand")),
				.Args =
				{
					{
						TEXT("FunctionName"),
						LOCTEXT("FunctionNameKey", "FunctionName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 1"),
						LOCTEXT("Arg1Key", "Arg 1"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 2"),
						LOCTEXT("Arg2Key", "Arg 2"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 3"),
						LOCTEXT("Arg3Key", "Arg 3"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 4"),
						LOCTEXT("Arg4Key", "Arg 4"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 5"),
						LOCTEXT("Arg5Key", "Arg 5"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 6"),
						LOCTEXT("Arg6Key", "Arg 6"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 7"),
						LOCTEXT("Arg7Key", "Arg 7"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 8"),
						LOCTEXT("Arg8Key", "Arg 8"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 9"),
						LOCTEXT("Arg9Key", "Arg 9"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("Arg 10"),
						LOCTEXT("Arg10Key", "Arg 10"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		},
		{
			TEXT("Fade"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("FadeNoteKey",
				                "The larger the value of ZOrder, the more it will appear in the foreground.\r\nWhen UseGlobalFade is true, ZOrder may not preview correctly.\r\n\r\nWhen using the ShidenWidget Sample as is and UseGlobalFade is false,\r\nthe ZOrder values are as follows:\r\nGreater than 0: in front of images\r\nGreater than 50: in front of text\r\nGreater than 100: in front of movie\r\nGreater than 150: in front of menus"),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenFadeCommand")),
				.Args =
				{
					{
						TEXT("LayerName"),
						LOCTEXT("LayerNameKey", "LayerName"),
						TEXT("Default"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeType"),
						LOCTEXT("FadeTypeKey", "FadeType"),
						TEXT("FadeOut"),
						FadeTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						LOCTEXT("FadeDurationKey", "FadeDuration"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeFunction"),
						LOCTEXT("FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("TargetColor"),
						LOCTEXT("TargetColorKey", "TargetColor"),
						TEXT("R=0.0 G=0.0 B=0.0"),
						ColorInputTemplate,
						{},
						false
					},
					{
						TEXT("Steps"),
						LOCTEXT("StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						LOCTEXT("BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("ZOrder"),
						LOCTEXT("ZOrderKey", "ZOrder"),
						TEXT("250"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("UseGlobalFade"),
						LOCTEXT("UseGlobalFadeKey", "UseGlobalFade"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("ChangeVisibility"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("ChangeVisibilityNoteKey", "You can specify the widget of a child widget as \"ChildWidgetName.WidgetName\"."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenChangeVisibilityCommand")),
				.Args =
				{
					{
						TEXT("Name"),
						LOCTEXT("NameKey", "Name"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Visibility"),
						LOCTEXT("VisibilityKey", "Visibility"),
						TEXT("Visible"),
						SlateVisibilityInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("MoveCanvasPanelSlot"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("MoveCanvasPanelSlotNoteKey",
				                "Steps is used only when the EasingFunction is Step.\r\nBlend Exp is used only when the EasingFunction is Ease."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenMoveCanvasPanelSlotCommand")),
				.Args =
				{
					{
						TEXT("SlotName"),
						LOCTEXT("SlotNameKey", "SlotName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("EasingFunction"),
						LOCTEXT("EasingFunctionKey", "EasingFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("Duration"),
						LOCTEXT("DurationKey", "Duration"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("ChangeType"),
						LOCTEXT("ChangeTypeKey", "ChangeType"),
						TEXT("AbsoluteValue"),
						ComboBoxInputTemplate,
						{
							{TEXT("Option1"), TEXT("AbsoluteValue")},
							{TEXT("Option2"), TEXT("AddToCurrent")}
						},
						false
					},
					{
						TEXT("OverwritePosition"),
						LOCTEXT("OverwritePositionKey", "OverwritePosition"),
						TEXT("{EMPTY}"),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteSize"),
						LOCTEXT("OverwriteSizeKey", "OverwriteSize"),
						TEXT("{EMPTY}"),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteZOrder"),
						LOCTEXT("OverwriteZOrderKey", "OverwriteZOrder"),
						TEXT("{EMPTY}"),
						IntegerInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("Steps"),
						LOCTEXT("StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						LOCTEXT("BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("AutoSave"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("AutoSaveNoteKey", "This command is not supported in preview."),
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAutoSaveCommand")),
				.Args =
				{
					{
						TEXT("SlotName"),
						LOCTEXT("SlotNameKey", "SlotName"),
						TEXT("Auto Save"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("OverwriteThumbnail"),
						LOCTEXT("OverwriteThumbnailKey", "OverwriteThumbnail"),
						TEXT("None"),
						AssetInputTemplate,
						{
							{TEXT("Target"), TEXT("Texture")},
							{TEXT("HasToggle"), TEXT("true")}
						},
						true
					}
				}
			}
		},
		{
			TEXT("Random"),
			FShidenCommandDefinition
			{
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenRandomCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableName"),
						LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")},
							{TEXT("AllowedVariableTypesForLiteral"), TEXT("Integer,Float")}
						},
						false
					},
					{
						TEXT("Min"),
						LOCTEXT("MinKey", "Min"),
						TEXT("0"),
						FlexNumberInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					},
					{
						TEXT("Max"),
						LOCTEXT("MaxKey", "Max"),
						TEXT("1"),
						FlexNumberInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("WaitClick"),
			FShidenCommandDefinition
			{
				.Style = InteractionStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenWaitClickCommand")),
				.Args =
				{
					{
						TEXT("CanSkip"),
						LOCTEXT("CanSkipKey", "CanSkip"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("WaitUntil"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("WaitUntilNoteKey", "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenWaitUntilCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						LOCTEXT("VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						LOCTEXT("VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						LOCTEXT("OperatorKey", "Operator"),
						TEXT(""),
						FlexOperatorInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					},
					{
						TEXT("RightHandValue"),
						LOCTEXT("RightHandValueKey", "RightHandValue"),
						TEXT(""),
						FlexInputTemplate,
						{
							{TEXT("VariableKindSourceIndex"), TEXT("0")},
							{TEXT("VariableNameSourceIndex"), TEXT("1")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("ForceFeedback"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("ForceFeedbackNoteKey", "This command is not supported in preview."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenForceFeedbackCommand")),
				.Args =
				{
					{
						TEXT("FeedbackEffect"),
						LOCTEXT("FeedbackEffectKey", "FeedbackEffect"),
						TEXT("None"),
						AssetInputTemplate,
						{{TEXT("Target"), TEXT("ForceFeedback")}},
						true
					}
				}
			}
		},
		{
			TEXT("PlayMedia"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("PlayMediaNoteKey",
				                "It is recommended to use ElectraPlayer for the Player of MediaSource.\r\nThe larger the value of ZOrder, the more it will appear in the foreground.\r\nGreater than 0: in front of images\r\nGreater than 50: in front of text\r\nGreater than 150: in front of menus\r\nThis command is not supported in preview."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenPlayMediaCommand")),
				.Args =
				{
					{
						TEXT("MediaSource"),
						LOCTEXT("MediaSourceKey", "MediaSource"),
						TEXT("None"),
						AssetInputTemplate,
						{{TEXT("Target"), TEXT("MediaSource")}},
						true
					},
					{
						TEXT("CanOpenPauseMenu"),
						LOCTEXT("CanOpenPauseMenuKey", "CanOpenPauseMenu"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("MediaZOrder"),
						LOCTEXT("MediaZOrderKey", "MediaZOrder"),
						TEXT("100"),
						IntegerInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("ChangeMaterialScalarParameter"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("ChangeMaterialScalarNoteKey",
				                "You will manipulate the Effect of the Retainer Box.\r\nSteps is used only when the EasingFunction is Step.\r\nBlend Exp is used only when the EasingFunction is Ease."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenChangeMaterialScalarParameterCommand")),
				.Args =
				{
					{
						TEXT("Target"),
						LOCTEXT("TargetKey", "Target"),
						TEXT("Image"),
						ComboBoxInputTemplate,
						{
							{TEXT("Option1"), TEXT("Image")},
							{TEXT("Option2"), TEXT("RetainerBox")}
						},
						false
					},
					{
						TEXT("TargetName"),
						LOCTEXT("TargetNameKey", "TargetName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("ParameterName"),
						LOCTEXT("ParameterNameKey", "ParameterName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("EasingFunction"),
						LOCTEXT("EasingFunctionKey", "EasingFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("Duration"),
						LOCTEXT("DurationKey", "Duration"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("ChangeType"),
						LOCTEXT("ChangeTypeKey", "ChangeType"),
						TEXT("AbsoluteValue"),
						ComboBoxInputTemplate,
						{
							{TEXT("Option1"), TEXT("AbsoluteValue")},
							{TEXT("Option2"), TEXT("AddToCurrent")}
						},
						false
					},
					{
						TEXT("EndValue"),
						LOCTEXT("EndValueKey", "EndValue"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("Steps"),
						LOCTEXT("StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						LOCTEXT("BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("ChangeMaterialTextureParameter"),
			FShidenCommandDefinition
			{
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenChangeMaterialTextureParameterCommand")),
				.Args =
				{
					{
						TEXT("Target"),
						LOCTEXT("TargetKey", "Target"),
						TEXT("Image"),
						ComboBoxInputTemplate,
						{
							{TEXT("Option1"), TEXT("Image")},
							{TEXT("Option2"), TEXT("RetainerBox")}
						},
						false
					},
					{
						TEXT("TargetName"),
						LOCTEXT("TargetNameKey", "TargetName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("ParameterName"),
						LOCTEXT("ParameterNameKey", "ParameterName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Texture"),
						LOCTEXT("TextureKey", "Texture"),
						TEXT("None"),
						AssetInputTemplate,
						{{TEXT("Target"), TEXT("Texture")}},
						true
					}
				}
			}
		},
		{
			TEXT("RunMacro"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("RunMacroNoteKey",
				                "The arguments passed to the macro are treated as local variables."),
				.Style = MacroStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenRunMacroCommand")),
				.Args =
				{
					{
						TEXT("MacroName"),
						LOCTEXT("MacroNameKey", "MacroName"),
						TEXT("None"),
						ScenarioInputTemplate,
						{
							{TEXT("Target"), TEXT("macro")},
							{TEXT("HasAdditionalArgs"), TEXT("true")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("RunMacroAsParallel"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("RunMacroAsParallelNoteKey",
				                "\"Default\" cannot be used as a process name.\r\nThe arguments passed to the macro are treated as local variables.\r\nThis command is not supported in preview."),
				.Style = MacroStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenRunMacroAsParallelCommand")),
				.Args =
				{
					{
						TEXT("NewProcessName"),
						LOCTEXT("NewProcessNameKey", "NewProcessName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("MacroName"),
						LOCTEXT("MacroNameKey", "MacroName"),
						TEXT("None"),
						ScenarioInputTemplate,
						{
							{TEXT("Target"), TEXT("macro")},
							{TEXT("HasAdditionalArgs"), TEXT("true")}
						},
						false
					}
				}
			}
		},
		{
			TEXT("StopParallelProcess"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("StopParallelProcessNoteKey",
				                "Due to the potential for causing issues,\r\nit is recommended to set wait for completion to true.\r\nThis command is not supported in preview."),
				.Style = MacroStyle,
				.bCanCallInMacro = false,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenStopParallelProcessCommand")),
				.Args =
				{
					{
						TEXT("ProcessName"),
						LOCTEXT("ProcessNameKey", "ProcessName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Reason"),
						LOCTEXT("ReasonKey", "Reason"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					},
					{
						TEXT("StopTiming"),
						LOCTEXT("StopTimingKey", "StopTiming"),
						TEXT("Immediately"),
						ComboBoxInputTemplate,
						{
							{TEXT("Option1"), TEXT("Immediately")},
							{TEXT("Option2"), TEXT("OnCurrentCommandEnd")}
						},
						false
					},
					{
						TEXT("WaitForCompletion"),
						LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Print"),
			FShidenCommandDefinition
			{
				.Style = CommentStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenPrintCommand")),
				.Args =
				{
					{
						TEXT("Content"),
						LOCTEXT("ContentKey", "Content"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("ShowVariableInsertButton"), TEXT("true")}},
						false
					}
				}
			}
		}
	};
}

#undef LOCTEXT_NAMESPACE
