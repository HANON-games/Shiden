// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/ShidenStandardCommandDefinitions.h"

const FShidenCommandStyle UShidenStandardCommandDefinitions::TextStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.150000f, 0.150000f, 0.150000f, 1.00000f),
	FLinearColor(0.150000f, 0.150000f, 0.150000f, 1.00000f),
	FLinearColor(0.150000f, 0.150000f, 0.150000f, 1.00000f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::MediaStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.200000f, 0.100000f, 0.000000f, 1.0f),
	FLinearColor(0.250000f, 0.125000f, 0.000000f, 1.0f),
	FLinearColor(0.250000f, 0.125000f, 0.000000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::FlowControlStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.000000f, 0.000000f, 0.150000f, 1.0f),
	FLinearColor(0.000000f, 0.000000f, 0.200000f, 1.0f),
	FLinearColor(0.000000f, 0.000000f, 0.200000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::UIControlStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.000000f, 0.200000f, 0.000000f, 1.0f),
	FLinearColor(0.000000f, 0.250000f, 0.000000f, 1.0f),
	FLinearColor(0.000000f, 0.250000f, 0.000000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::DataStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.000000f, 0.150000f, 0.200000f, 1.0f),
	FLinearColor(0.000000f, 0.175000f, 0.250000f, 1.0f),
	FLinearColor(0.000000f, 0.175000f, 0.250000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::MacroStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.050000f, 0.050000f, 0.050000f, 1.0f),
	FLinearColor(0.070000f, 0.070000f, 0.070000f, 1.0f),
	FLinearColor(0.070000f, 0.070000f, 0.070000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::CommentStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.250000f, 0.250000f, 0.000000f, 1.0f),
	FLinearColor(0.300000f, 0.300000f, 0.000000f, 1.0f),
	FLinearColor(0.300000f, 0.300000f, 0.000000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::TagStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.200000f, 0.000000f, 0.000000f, 1.0f),
	FLinearColor(0.250000f, 0.000000f, 0.000000f, 1.0f),
	FLinearColor(0.250000f, 0.000000f, 0.000000f, 1.0f)
};

const FShidenCommandStyle UShidenStandardCommandDefinitions::InteractionStyle = FShidenCommandStyle{
	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
	FLinearColor(0.200000f, 0.000000f, 0.200000f, 1.0f),
	FLinearColor(0.250000f, 0.000000f, 0.250000f, 1.0f),
	FLinearColor(0.250000f, 0.000000f, 0.250000f, 1.0f)
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
				.Note = NSLOCTEXT("ShidenNamespace", "TextNoteKey",
				                  "You can insert variable values using { variable_name }.\r\nIf \":\" is included in the variable name, escape it as \"\\:\".\r\nThe prefixes for variables are as follows:\r\n* \"System::\": System variables\r\n* \"Local::\": Local variables\r\n* \"Predefined::\": Predefined system variables\r\nThe track ID for Voice will be 0.\r\nIf both Voice and DialogueBlip are specified, Voice takes precedence."),
				.Style = TextStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenTextCommand")),
				.Args =
				{
					{
						TEXT("TextWidgetName"),
						NSLOCTEXT("ShidenNamespace", "TextWidgetNameKey", "TextWidgetName"),
						TEXT("Talk"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("TextType"),
						NSLOCTEXT("ShidenNamespace", "TextTypeKey", "TextType"),
						TEXT("Default"),
						TextTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("Text"),
						NSLOCTEXT("ShidenNamespace", "TextKey", "Text"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForInput"),
						NSLOCTEXT("ShidenNamespace", "WaitForInputKey", "WaitForInput"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("InstantTextDisplay"),
						NSLOCTEXT("ShidenNamespace", "InstantTextDisplayKey", "InstantTextDisplay"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("Voice"),
						NSLOCTEXT("ShidenNamespace", "VoiceKey", "Voice"),
						TEXT("None"),
						SoundInputTemplate,
						{{TEXT("Target"), TEXT("Voice")}},
						true
					},
					{
						TEXT("VoiceTrackId"),
						NSLOCTEXT("ShidenNamespace", "VoiceTrackIdKey", "VoiceTrackId"),
						TEXT("0"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("TextBlip"),
						NSLOCTEXT("ShidenNamespace", "DialogueBlipKey", "TextBlip"),
						TEXT("None"),
						SoundInputTemplate,
						{{TEXT("Target"), TEXT("Voice")}},
						true
					},
					{
						TEXT("DisableAutoStopPreviousVoices"),
						NSLOCTEXT("ShidenNamespace", "DisableAutoStopPreviousVoicesKey", "DisableAutoStop\r\nPreviousVoices"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("ContinueFromThePreviousText"),
						NSLOCTEXT("ShidenNamespace", "ContinueFromThePreviousTextKey", "ContinueFrom\r\nThePreviousText"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2"),
						NSLOCTEXT("ShidenNamespace", "Language2Key", "Language 2"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 3"),
						NSLOCTEXT("ShidenNamespace", "Language3Key", "Language 3"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 4"),
						NSLOCTEXT("ShidenNamespace", "Language4Key", "Language 4"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 5"),
						NSLOCTEXT("ShidenNamespace", "Language5Key", "Language 5"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 6"),
						NSLOCTEXT("ShidenNamespace", "Language6Key", "Language 6"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 7"),
						NSLOCTEXT("ShidenNamespace", "Language7Key", "Language 7"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 8"),
						NSLOCTEXT("ShidenNamespace", "Language8Key", "Language 8"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 9"),
						NSLOCTEXT("ShidenNamespace", "Language9Key", "Language 9"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 10"),
						NSLOCTEXT("ShidenNamespace", "Language10Key", "Language 10"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Image"),
			FShidenCommandDefinition
			{
				.Note = NSLOCTEXT("ShidenNamespace", "ImageNoteKey",
				                  "If no image is specified, the values of Position, Size,\r\nand SizeToContent are ignored, and the current image fades out.\r\nSteps is used only when the FadeFunction is Step.\r\nBlend Exp is used only when the FadeFunction is Ease.\r\nFor SlateBrush created from Material, fading does not work,\r\nso please change the opacity separately."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenImageCommand")),
				.Args =
				{
					{
						TEXT("SlotName"),
						NSLOCTEXT("ShidenNamespace", "SlotNameKey", "SlotName"),
						TEXT("Background"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Image"),
						NSLOCTEXT("ShidenNamespace", "ImageKey", "Image"),
						TEXT("None"),
						AssetInputTemplate,
						{{TEXT("Target"), TEXT("SlateBrush")}},
						true
					},
					{
						TEXT("OverwritePosition"),
						NSLOCTEXT("ShidenNamespace", "OverwritePositionKey", "OverwritePosition"),
						TEXT(""),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteSize"),
						NSLOCTEXT("ShidenNamespace", "OverwriteSizeKey", "OverwriteSize"),
						TEXT(""),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteSizeToContent"),
						NSLOCTEXT("ShidenNamespace", "OverwriteSizeToContentKey", "OverwriteSizeToContent"),
						TEXT(""),
						BooleanInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("FadeFunction"),
						NSLOCTEXT("ShidenNamespace", "FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						NSLOCTEXT("ShidenNamespace", "FadeDurationKey", "FadeDuration"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("Steps"),
						NSLOCTEXT("ShidenNamespace", "StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						NSLOCTEXT("ShidenNamespace", "BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "SoundNoteKey",
				                  "Track ID is not effective for SE.\r\nYou cannot stop a playing SE midway.\r\nYou can stop it by leaving the sound source field empty.\r\nThe value for DisableAutoStopPreviousVoices is only effective for Voice.\r\nWhen fading out the currently playing BGM or voice, the values of Volume and Pitch are ignored.\r\nFor MetaSound Source, it seems that the Start Time value is not reflected."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenSoundCommand")),
				.Args =
				{
					{
						TEXT("SoundType"),
						NSLOCTEXT("ShidenNamespace", "SoundTypeKey", "SoundType"),
						TEXT("BGM"),
						SoundTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("TrackId"),
						NSLOCTEXT("ShidenNamespace", "TrackIdKey", "TrackId"),
						TEXT("0"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("SoundSource"),
						NSLOCTEXT("ShidenNamespace", "SoundSourceKey", "SoundSource"),
						TEXT("None"),
						SoundInputTemplate,
						{{TEXT("SoundTypeSourceIndex"), TEXT("0")}},
						true
					},
					{
						TEXT("Volume"),
						NSLOCTEXT("ShidenNamespace", "VolumeKey", "Volume"),
						TEXT("1.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("Pitch"),
						NSLOCTEXT("ShidenNamespace", "PitchKey", "Pitch"),
						TEXT("1.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("StartTime"),
						NSLOCTEXT("ShidenNamespace", "StartTimeKey", "StartTime"),
						TEXT("0.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeType"),
						NSLOCTEXT("ShidenNamespace", "FadeTypeKey", "FadeType"),
						TEXT("FadeIn"),
						FadeTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeFunction"),
						NSLOCTEXT("ShidenNamespace", "FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						BGMFadeFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						NSLOCTEXT("ShidenNamespace", "FadeDurationKey", "FadeDuration"),
						TEXT("0.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("DisableAutoStopPreviousVoices"),
						NSLOCTEXT("ShidenNamespace", "DisableAutoStopPreviousVoicesKey", "DisableAutoStop\r\nPreviousVoices"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForFadeCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForFadeCompletionKey", "WaitForFadeCompletion"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForSoundCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForSoundCompletionKey", "WaitForSoundCompletion"),
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
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAdjustVolumeCommand")),
				.Args =
				{
					{
						TEXT("TrackId"),
						NSLOCTEXT("ShidenNamespace", "TrackIdKey", "TrackId"),
						TEXT("0"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("Volume"),
						NSLOCTEXT("ShidenNamespace", "VolumeKey", "Volume"),
						TEXT("1.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeFunction"),
						NSLOCTEXT("ShidenNamespace", "FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						BGMFadeFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						NSLOCTEXT("ShidenNamespace", "FadeDurationKey", "FadeDuration"),
						TEXT("0.00"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "OptionsNoteKey",
				                  "Only Integer type variables can be specified for DestinationVariableName."),
				.Style = InteractionStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenOptionsCommand")),
				.Args =
				{
					{
						TEXT("Option 1"),
						NSLOCTEXT("ShidenNamespace", "Option1Key", "Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Option 2"),
						NSLOCTEXT("ShidenNamespace", "Option2Key", "Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Option 3"),
						NSLOCTEXT("ShidenNamespace", "Option3Key", "Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Option 4"),
						NSLOCTEXT("ShidenNamespace", "Option4Key", "Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Option 5"),
						NSLOCTEXT("ShidenNamespace", "Option5Key", "Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableKind"),
						NSLOCTEXT("ShidenNamespace", "DestinationVariableKindKey", "DestinationVariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableName"),
						NSLOCTEXT("ShidenNamespace", "DestinationVariableNameKey", "DestinationVariableName"),
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
						NSLOCTEXT("ShidenNamespace", "HideTextLayerKey", "HideTextLayer"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language2Option1Key", "Language 2 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language2Option2Key", "Language 2 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language2Option3Key", "Language 2 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language2Option4Key", "Language 2 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 2 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language2Option5Key", "Language 2 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 3 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language3Option1Key", "Language 3 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 3 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language3Option2Key", "Language 3 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 3 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language3Option3Key", "Language 3 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 3 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language3Option4Key", "Language 3 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 3 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language3Option5Key", "Language 3 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 4 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language4Option1Key", "Language 4 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 4 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language4Option2Key", "Language 4 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 4 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language4Option3Key", "Language 4 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 4 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language4Option4Key", "Language 4 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 4 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language4Option5Key", "Language 4 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 5 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language5Option1Key", "Language 5 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 5 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language5Option2Key", "Language 5 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 5 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language5Option3Key", "Language 5 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 5 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language5Option4Key", "Language 5 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 5 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language5Option5Key", "Language 5 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 6 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language6Option1Key", "Language 6 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 6 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language6Option2Key", "Language 6 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 6 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language6Option3Key", "Language 6 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 6 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language6Option4Key", "Language 6 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 6 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language6Option5Key", "Language 6 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 7 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language7Option1Key", "Language 7 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 7 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language7Option2Key", "Language 7 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 7 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language7Option3Key", "Language 7 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 7 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language7Option4Key", "Language 7 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 7 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language7Option5Key", "Language 7 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 8 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language8Option1Key", "Language 8 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 8 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language8Option2Key", "Language 8 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 8 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language8Option3Key", "Language 8 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 8 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language8Option4Key", "Language 8 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 8 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language8Option5Key", "Language 8 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 9 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language9Option1Key", "Language 9 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 9 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language9Option2Key", "Language 9 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 9 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language9Option3Key", "Language 9 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 9 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language9Option4Key", "Language 9 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 9 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language9Option5Key", "Language 9 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 10 Option 1"),
						NSLOCTEXT("ShidenNamespace", "Language10Option1Key", "Language 10 Option 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 10 Option 2"),
						NSLOCTEXT("ShidenNamespace", "Language10Option2Key", "Language 10 Option 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 10 Option 3"),
						NSLOCTEXT("ShidenNamespace", "Language10Option3Key", "Language 10 Option 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 10 Option 4"),
						NSLOCTEXT("ShidenNamespace", "Language10Option4Key", "Language 10 Option 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Language 10 Option 5"),
						NSLOCTEXT("ShidenNamespace", "Language10Option5Key", "Language 10 Option 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
				}
			}
		},
		{
			TEXT("TextInput"),
			FShidenCommandDefinition
			{
				.Note = NSLOCTEXT("ShidenNamespace", "TextInputNoteKey",
				                  "You can input patterns like the following in AllowedCharacterRegex.\r\n- Allow only numbers: [0-9]\r\n- Allow only abc: [abc]\r\n- Allow anything except numbers: [^0-9]"),
				.Style = InteractionStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenTextInputCommand")),
				.Args =
				{
					{
						TEXT("MaxLength"),
						NSLOCTEXT("ShidenNamespace", "MaxLengthKey", "MaxLength"),
						TEXT(""),
						IntegerInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("MaxLineCount"),
						NSLOCTEXT("ShidenNamespace", "MaxLineCountKey", "MaxLineCount"),
						TEXT("1"),
						IntegerInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("AllowedCharacterRegex"),
						NSLOCTEXT("ShidenNamespace", "AllowedCharacterRegexKey", "AllowedCharacterRegex"),
						TEXT(""),
						TextInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("DefaultText"),
						NSLOCTEXT("ShidenNamespace", "DefaultTextKey", "DefaultText"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("HintText"),
						NSLOCTEXT("ShidenNamespace", "HintTextKey", "HintText"),
						TEXT(""),
						MultilineTextInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableKind"),
						NSLOCTEXT("ShidenNamespace", "DestinationVariableKindKey", "DestinationVariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableName"),
						NSLOCTEXT("ShidenNamespace", "DestinationVariableNameKey", "DestinationVariableName"),
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
						NSLOCTEXT("ShidenNamespace", "HideTextLayerKey", "HideTextLayer"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "AssignVariableNoteKey", "Spaces cannot be used in the key."),
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAssignVariableCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						NSLOCTEXT("ShidenNamespace", "VariableNameKey", "VariableName"),
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
						NSLOCTEXT("ShidenNamespace", "ValueKey", "Value"),
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
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						NSLOCTEXT("ShidenNamespace", "VariableNameKey", "VariableName"),
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
						NSLOCTEXT("ShidenNamespace", "OperatorKey", "Operator"),
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
						NSLOCTEXT("ShidenNamespace", "ValueKey", "Value"),
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
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenIfCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						NSLOCTEXT("ShidenNamespace", "VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						NSLOCTEXT("ShidenNamespace", "OperatorKey", "Operator"),
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
						NSLOCTEXT("ShidenNamespace", "RightHandValueKey", "RightHandValue"),
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
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenElseIfCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						NSLOCTEXT("ShidenNamespace", "VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						NSLOCTEXT("ShidenNamespace", "OperatorKey", "Operator"),
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
						NSLOCTEXT("ShidenNamespace", "RightHandValueKey", "RightHandValue"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "LoopWhileNoteKey",
				                  "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenLoopWhileCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						NSLOCTEXT("ShidenNamespace", "VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						NSLOCTEXT("ShidenNamespace", "OperatorKey", "Operator"),
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
						NSLOCTEXT("ShidenNamespace", "RightHandValueKey", "RightHandValue"),
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
			TEXT("EndLoopWhile"),
			FShidenCommandDefinition
			{
				.Note = NSLOCTEXT("ShidenNamespace", "EndLoopWhileNoteKey",
				                  "This command is not supported in preview."),
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
						NSLOCTEXT("ShidenNamespace", "NameKey", "Name"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "JumpNoteKey",
				                  "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenJumpCommand")),
				.Args =
				{
					{
						TEXT("DestinationTagName"),
						NSLOCTEXT("ShidenNamespace", "DestinationTagNameKey", "DestinationTagName"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "EndScenarioNoteKey",
				                  "In a macro, the next scenario specification is ignored,\r\nand it resumes from the continuation of the original scenario.\r\nThis command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenEndScenarioCommand")),
				.Args =
				{
					{
						TEXT("NextScenario"),
						NSLOCTEXT("ShidenNamespace", "NextScenarioKey", "NextScenario"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "BreakNoteKey",
				                  "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenBreakCommand")),
				.Args =
				{
					{
						TEXT("ReasonForInterruption"),
						NSLOCTEXT("ShidenNamespace", "ReasonForInterruptionKey", "ReasonForInterruption"),
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
						NSLOCTEXT("ShidenNamespace", "CommentKey", "Comment"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "WidgetAnimationNoteKey",
				                  "You can specify the animation of a child widget as \"ChildWidgetName.AnimationName\".\nWhen RestoreState is true, the behavior may differ between gameplay and preview."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenWidgetAnimationCommand")),
				.Args =
				{
					{
						TEXT("AnimationName"),
						NSLOCTEXT("ShidenNamespace", "AnimationNameKey", "AnimationName"),
						TEXT("Shake"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("StartTime"),
						NSLOCTEXT("ShidenNamespace", "StartTimeKey", "StartTime"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("NumLoopToPlay"),
						NSLOCTEXT("ShidenNamespace", "NumLoopToPlayKey", "NumLoopToPlay"),
						TEXT("1"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("PlayMode"),
						NSLOCTEXT("ShidenNamespace", "PlayModeKey", "PlayMode"),
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
						NSLOCTEXT("ShidenNamespace", "PlaybackSpeedKey", "PlaybackSpeed"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("RestoreState"),
						NSLOCTEXT("ShidenNamespace", "RestoreStateKey", "RestoreState"),
						TEXT("false"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "DelayNoteKey",
				                  "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenDelayCommand")),
				.Args =
				{
					{
						TEXT("Duration"),
						NSLOCTEXT("ShidenNamespace", "DurationKey", "Duration"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "CallWidgetFunctionNoteKey", "Arg cannot be left empty midway."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenCallWidgetFunctionCommand")),
				.Args =
				{
					{
						TEXT("FunctionName"),
						NSLOCTEXT("ShidenNamespace", "FunctionNameKey", "FunctionName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 1"),
						NSLOCTEXT("ShidenNamespace", "Arg1Key", "Arg 1"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 2"),
						NSLOCTEXT("ShidenNamespace", "Arg2Key", "Arg 2"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 3"),
						NSLOCTEXT("ShidenNamespace", "Arg3Key", "Arg 3"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 4"),
						NSLOCTEXT("ShidenNamespace", "Arg4Key", "Arg 4"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 5"),
						NSLOCTEXT("ShidenNamespace", "Arg5Key", "Arg 5"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 6"),
						NSLOCTEXT("ShidenNamespace", "Arg6Key", "Arg 6"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 7"),
						NSLOCTEXT("ShidenNamespace", "Arg7Key", "Arg 7"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 8"),
						NSLOCTEXT("ShidenNamespace", "Arg8Key", "Arg 8"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 9"),
						NSLOCTEXT("ShidenNamespace", "Arg9Key", "Arg 9"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Arg 10"),
						NSLOCTEXT("ShidenNamespace", "Arg10Key", "Arg 10"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					}
				}
			}
		},
		{
			TEXT("Fade"),
			FShidenCommandDefinition
			{
				.Note = NSLOCTEXT("ShidenNamespace", "FadeNoteKey",
				                  "The larger the value of z-order, the more it will appear in the foreground.\r\nGreater than 0: in front of images\r\nGreater than 50: in front of text\r\nGreater than 100: in front of movie\r\nGreater than 150: in front of menus"),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenFadeCommand")),
				.Args =
				{
					{
						TEXT("LayerName"),
						NSLOCTEXT("ShidenNamespace", "LayerNameKey", "LayerName"),
						TEXT("Default"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeType"),
						NSLOCTEXT("ShidenNamespace", "FadeTypeKey", "FadeType"),
						TEXT("FadeOut"),
						FadeTypeInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeDuration"),
						NSLOCTEXT("ShidenNamespace", "FadeDurationKey", "FadeDuration"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("FadeFunction"),
						NSLOCTEXT("ShidenNamespace", "FadeFunctionKey", "FadeFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("TargetColor"),
						NSLOCTEXT("ShidenNamespace", "TargetColorKey", "TargetColor"),
						TEXT("R=0.0 G=0.0 B=0.0"),
						ColorInputTemplate,
						{},
						false
					},
					{
						TEXT("Steps"),
						NSLOCTEXT("ShidenNamespace", "StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						NSLOCTEXT("ShidenNamespace", "BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("ZOrder"),
						NSLOCTEXT("ShidenNamespace", "ZOrderKey", "ZOrder"),
						TEXT("250"),
						IntegerInputTemplate,
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
				.Note = NSLOCTEXT("ShidenNamespace", "ChangeVisibilityNoteKey",
				                  "You can specify the widget of a child widget as \"ChildWidgetName.WidgetName\"."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenChangeVisibilityCommand")),
				.Args =
				{
					{
						TEXT("Name"),
						NSLOCTEXT("ShidenNamespace", "NameKey", "Name"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Visibility"),
						NSLOCTEXT("ShidenNamespace", "VisibilityKey", "Visibility"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "MoveCanvasPanelSlotNoteKey",
				                  "Steps is used only when the EasingFunction is Step.\r\nBlend Exp is used only when the EasingFunction is Ease."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenMoveCanvasPanelSlotCommand")),
				.Args =
				{
					{
						TEXT("SlotName"),
						NSLOCTEXT("ShidenNamespace", "SlotNameKey", "SlotName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("EasingFunction"),
						NSLOCTEXT("ShidenNamespace", "EasingFunctionKey", "EasingFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("Duration"),
						NSLOCTEXT("ShidenNamespace", "DurationKey", "Duration"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("ChangeType"),
						NSLOCTEXT("ShidenNamespace", "ChangeTypeKey", "ChangeType"),
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
						NSLOCTEXT("ShidenNamespace", "OverwritePositionKey", "OverwritePosition"),
						TEXT("{EMPTY}"),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteSize"),
						NSLOCTEXT("ShidenNamespace", "OverwriteSizeKey", "OverwriteSize"),
						TEXT("{EMPTY}"),
						Vector2InputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("OverwriteZOrder"),
						NSLOCTEXT("ShidenNamespace", "OverwriteZOrderKey", "OverwriteZOrder"),
						TEXT("{EMPTY}"),
						IntegerInputTemplate,
						{{TEXT("HasToggle"), TEXT("true")}},
						false
					},
					{
						TEXT("Steps"),
						NSLOCTEXT("ShidenNamespace", "StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						NSLOCTEXT("ShidenNamespace", "BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "AutoSaveNoteKey", "This command is not supported in preview."),
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAutoSaveCommand")),
				.Args =
				{
					{
						TEXT("SlotName"),
						NSLOCTEXT("ShidenNamespace", "SlotNameKey", "SlotName"),
						TEXT("Auto Save"),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("OverwriteThumbnail"),
						NSLOCTEXT("ShidenNamespace", "OverwriteThumbnailKey", "OverwriteThumbnail"),
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
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("DestinationVariableName"),
						NSLOCTEXT("ShidenNamespace", "DestinationVariableNameKey", "DestinationVariableName"),
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
						NSLOCTEXT("ShidenNamespace", "MinKey", "Min"),
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
						NSLOCTEXT("ShidenNamespace", "MaxKey", "Max"),
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
						NSLOCTEXT("ShidenNamespace", "CanSkipKey", "CanSkip"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "WaitUntilNoteKey", "This command is not supported in preview."),
				.Style = FlowControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenWaitUntilCommand")),
				.Args =
				{
					{
						TEXT("VariableKind"),
						NSLOCTEXT("ShidenNamespace", "VariableKindKey", "VariableKind"),
						TEXT("UserVariable"),
						VariableKindInputTemplate,
						{},
						false
					},
					{
						TEXT("VariableName"),
						NSLOCTEXT("ShidenNamespace", "VariableNameKey", "VariableName"),
						TEXT(""),
						VariableNameInputTemplate,
						{{TEXT("VariableKindSourceIndex"), TEXT("0")}},
						false
					},
					{
						TEXT("Operator"),
						NSLOCTEXT("ShidenNamespace", "OperatorKey", "Operator"),
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
						NSLOCTEXT("ShidenNamespace", "RightHandValueKey", "RightHandValue"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "ForceFeedbackNoteKey", "This command is not supported in preview."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenForceFeedbackCommand")),
				.Args =
				{
					{
						TEXT("FeedbackEffect"),
						NSLOCTEXT("ShidenNamespace", "FeedbackEffectKey", "FeedbackEffect"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "PlayMediaNoteKey",
				                  "It is recommended to use ElectraPlayer for the Player of MediaSource.\r\nThe larger the value of Z-order, the more it will appear in the foreground.\r\nGreater than 0: in front of images\r\nGreater than 50: in front of text\r\nGreater than 150: in front of menus\r\nThis command is not supported in preview."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenPlayMediaCommand")),
				.Args =
				{
					{
						TEXT("MediaSource"),
						NSLOCTEXT("ShidenNamespace", "MediaSourceKey", "MediaSource"),
						TEXT("None"),
						AssetInputTemplate,
						{{TEXT("Target"), TEXT("MediaSource")}},
						true
					},
					{
						TEXT("CanOpenPauseMenu"),
						NSLOCTEXT("ShidenNamespace", "CanOpenPauseMenuKey", "CanOpenPauseMenu"),
						TEXT("true"),
						BooleanInputTemplate,
						{},
						false
					},
					{
						TEXT("MediaZOrder"),
						NSLOCTEXT("ShidenNamespace", "MediaZOrderKey", "MediaZOrder"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "ChangeMaterialScalarNoteKey",
				                  "You will manipulate the Effect of the Retainer Box.\r\nSteps is used only when the EasingFunction is Step.\r\nBlend Exp is used only when the EasingFunction is Ease."),
				.Style = UIControlStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenChangeMaterialScalarParameterCommand")),
				.Args =
				{
					{
						TEXT("Target"),
						NSLOCTEXT("ShidenNamespace", "TargetKey", "Target"),
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
						NSLOCTEXT("ShidenNamespace", "TargetNameKey", "TargetName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("ParameterName"),
						NSLOCTEXT("ShidenNamespace", "ParameterNameKey", "ParameterName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("EasingFunction"),
						NSLOCTEXT("ShidenNamespace", "EasingFunctionKey", "EasingFunction"),
						TEXT("Linear"),
						EasingFuncInputTemplate,
						{},
						false
					},
					{
						TEXT("Duration"),
						NSLOCTEXT("ShidenNamespace", "DurationKey", "Duration"),
						TEXT("1.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("ChangeType"),
						NSLOCTEXT("ShidenNamespace", "ChangeTypeKey", "ChangeType"),
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
						NSLOCTEXT("ShidenNamespace", "EndValueKey", "EndValue"),
						TEXT("0.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("Steps"),
						NSLOCTEXT("ShidenNamespace", "StepsKey", "Steps"),
						TEXT("2"),
						IntegerInputTemplate,
						{},
						false
					},
					{
						TEXT("BlendExp"),
						NSLOCTEXT("ShidenNamespace", "BlendExpKey", "BlendExp"),
						TEXT("2.0"),
						FloatInputTemplate,
						{},
						false
					},
					{
						TEXT("WaitForCompletion"),
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
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
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenChangeTextureParameterCommand")),
				.Args =
				{
					{
						TEXT("Target"),
						NSLOCTEXT("ShidenNamespace", "TargetKey", "Target"),
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
						NSLOCTEXT("ShidenNamespace", "TargetNameKey", "TargetName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("ParameterName"),
						NSLOCTEXT("ShidenNamespace", "ParameterNameKey", "ParameterName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Texture"),
						NSLOCTEXT("ShidenNamespace", "TextureKey", "Texture"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "RunMacroNoteKey",
				                  "The arguments passed to the macro are treated as local variables."),
				.Style = MacroStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenRunMacroCommand")),
				.Args =
				{
					{
						TEXT("MacroName"),
						NSLOCTEXT("ShidenNamespace", "MacroNameKey", "MacroName"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "RunMacroAsParallelNoteKey",
				                  "\"Default\" cannot be used as a process name.\r\nThe arguments passed to the macro are treated as local variables.\r\nThis command is not supported in preview."),
				.Style = MacroStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenRunMacroAsParallelCommand")),
				.Args =
				{
					{
						TEXT("NewProcessName"),
						NSLOCTEXT("ShidenNamespace", "NewProcessNameKey", "NewProcessName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("MacroName"),
						NSLOCTEXT("ShidenNamespace", "MacroNameKey", "MacroName"),
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
				.Note = NSLOCTEXT("ShidenNamespace", "StopParallelProcessNoteKey",
				                  "Due to the potential for causing issues,\r\nit is recommended to set wait for completion to true.\r\nThis command is not supported in preview."),
				.Style = MacroStyle,
				.bCanCallInMacro = false,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenStopParallelProcessCommand")),
				.Args =
				{
					{
						TEXT("ProcessName"),
						NSLOCTEXT("ShidenNamespace", "ProcessNameKey", "ProcessName"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("Reason"),
						NSLOCTEXT("ShidenNamespace", "ReasonKey", "Reason"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					},
					{
						TEXT("StopTiming"),
						NSLOCTEXT("ShidenNamespace", "StopTimingKey", "StopTiming"),
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
						NSLOCTEXT("ShidenNamespace", "WaitForCompletionKey", "WaitForCompletion"),
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
						NSLOCTEXT("ShidenNamespace", "ContentKey", "Content"),
						TEXT(""),
						TextInputTemplate,
						{},
						false
					}
				}
			}
		}
	};
}
