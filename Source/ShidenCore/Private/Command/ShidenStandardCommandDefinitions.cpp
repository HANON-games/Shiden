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
						.ArgName = TEXT("TextWidgetName"),
						.DefaultValue = TEXT("Talk"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextWidgetNameKey", "TextWidgetName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("TextType"),
						.DefaultValue = TEXT("Default"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextTypeKey", "TextType"),
							.TemplateWidget = TextTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("Text"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextKey", "Text"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("WaitForInput"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForInputKey", "WaitForInput"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("InstantTextDisplay"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("InstantTextDisplayKey", "InstantTextDisplay"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("Voice"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VoiceKey", "Voice"),
							.TemplateWidget = SoundInputTemplate,
							.TemplateParameters = {{TEXT("Target"), TEXT("Voice")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ Voice })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("VoiceTrackId"),
						.DefaultValue = TEXT("0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VoiceTrackIdKey", "VoiceTrackId"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("TextBlip"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DialogueBlipKey", "TextBlip"),
							.TemplateWidget = SoundInputTemplate,
							.TemplateParameters = {{TEXT("Target"), TEXT("Voice")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ TextBlip })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("DisableAutoStopPreviousVoices"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DisableAutoStopPreviousVoicesKey", "DisableAutoStop\r\nPreviousVoices"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("ContinueFromThePreviousText"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ContinueFromThePreviousTextKey", "ContinueFrom\r\nThePreviousText"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("Language 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Key", "Language 2"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language3Key", "Language 3"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language4Key", "Language 4"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language5Key", "Language 5"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 6"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language6Key", "Language 6"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 7"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language7Key", "Language 7"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 8"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language8Key", "Language 8"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 9"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language9Key", "Language 9"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 10"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language10Key", "Language 10"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("SlotName"),
						.DefaultValue = TEXT("Background"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("SlotNameKey", "SlotName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Image"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ImageKey", "Image"),
							.TemplateWidget = AssetInputTemplate,
							.TemplateParameters = {{TEXT("Target"), TEXT("SlateBrush")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ Image })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("OverwritePosition"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwritePositionKey", "OverwritePosition"),
							.TemplateWidget = Vector2InputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("OverwriteSize"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwriteSizeKey", "OverwriteSize"),
							.TemplateWidget = Vector2InputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("OverwriteSizeToContent"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwriteSizeToContentKey", "OverwriteSizeToContent"),
							.TemplateWidget = BooleanInputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("FadeFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeFunctionKey", "FadeFunction"),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("Steps"),
						.DefaultValue = TEXT("2"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StepsKey", "Steps"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("1")}},
							.VisibilityCondition = TEXT("HasVariable({ FadeFunction }) || { FadeFunction } == \"Step\"")
						}
					},
					{
						.ArgName = TEXT("BlendExp"),
						.DefaultValue = TEXT("2.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("BlendExpKey", "BlendExp"),
							.TemplateWidget = FloatInputTemplate,
							.VisibilityCondition = TEXT("HasVariable({ FadeFunction }) || { FadeFunction } == \"Ease In\" || { FadeFunction } == \"Ease Out\" || { FadeFunction } == \"Ease In Out\"")
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
					}
				}
			}
		},
		{
			TEXT("Sound"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("SoundNoteKey",
				                "Track ID is not effective for SE.\r\nYou cannot stop a playing SE midway.\r\nYou can stop it by leaving the sound source field empty.\r\nWhen fading out the currently playing BGM or voice, the values of Volume and Pitch are ignored.\r\nFor MetaSound Source, it seems that the Start Time value is not reflected."),
				.Style = MediaStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenSoundCommand")),
				.Args =
				{
					{
						.ArgName = TEXT("SoundType"),
						.DefaultValue = TEXT("BGM"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("SoundTypeKey", "SoundType"),
							.TemplateWidget = SoundTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("TrackId"),
						.DefaultValue = TEXT("0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TrackIdKey", "TrackId"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}},
							.VisibilityCondition = TEXT("HasVariable({ SoundType }) || { SoundType } != \"SE\"")
						}
					},
					{
						.ArgName = TEXT("SoundSource"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("SoundSourceKey", "SoundSource"),
							.TemplateWidget = SoundInputTemplate,
							.TemplateParameters = {{TEXT("SoundTypeSourceArgName"), TEXT("SoundType")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ SoundSource })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("Volume"),
						.DefaultValue = TEXT("1.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VolumeKey", "Volume"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("Pitch"),
						.DefaultValue = TEXT("1.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("PitchKey", "Pitch"),
							.TemplateWidget = FloatInputTemplate
						}
					},
					{
						.ArgName = TEXT("StartTime"),
						.DefaultValue = TEXT("0.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StartTimeKey", "StartTime"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("FadeType"),
						.DefaultValue = TEXT("FadeIn"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeTypeKey", "FadeType"),
							.TemplateWidget = FadeTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeFunctionKey", "FadeFunction"),
							.TemplateWidget = BGMFadeFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("DisableAutoStopPreviousVoices"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DisableAutoStopPreviousVoicesKey", "DisableAutoStop\r\nPreviousVoices"),
							.TemplateWidget = BooleanInputTemplate,
							.VisibilityCondition = TEXT("HasVariable({ SoundType }) || { SoundType } == \"Voice\"")
						}
					},
					{
						.ArgName = TEXT("WaitForFadeCompletion"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForFadeCompletionKey", "WaitForFadeCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("WaitForSoundCompletion"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForSoundCompletionKey", "WaitForSoundCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("UseGlobalBGM"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("UseGlobalBGMKey", "UseGlobalBGM"),
							.TemplateWidget = BooleanInputTemplate,
							.VisibilityCondition = TEXT("HasVariable({ SoundType }) || { SoundType } == \"BGM\"")
						}
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
						.ArgName = TEXT("TrackId"),
						.DefaultValue = TEXT("0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TrackIdKey", "TrackId"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("Volume"),
						.DefaultValue = TEXT("1.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VolumeKey", "Volume"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("FadeFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeFunctionKey", "FadeFunction"),
							.TemplateWidget = BGMFadeFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("DestinationVariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableKindKey", "DestinationVariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("DestinationVariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("DestinationVariableKind")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")},
								{TEXT("AllowedVariableTypesForLiteral"), TEXT("Integer")}
							}
						}
					},
					{
						.ArgName = TEXT("HideTextLayer"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("HideTextLayerKey", "HideTextLayer"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Option1Key", "Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Option2Key", "Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Option3Key", "Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Option4Key", "Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Option5Key", "Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 2 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Option1Key", "Language 2 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 2 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Option2Key", "Language 2 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 2 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Option3Key", "Language 2 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 2 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Option4Key", "Language 2 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 2 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Option5Key", "Language 2 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 3 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language3Option1Key", "Language 3 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 3 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language3Option2Key", "Language 3 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 3 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language3Option3Key", "Language 3 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 3 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language3Option4Key", "Language 3 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 3 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language3Option5Key", "Language 3 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 4 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language4Option1Key", "Language 4 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 4 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language4Option2Key", "Language 4 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 4 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language4Option3Key", "Language 4 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 4 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language4Option4Key", "Language 4 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 4 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language4Option5Key", "Language 4 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 5 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language5Option1Key", "Language 5 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 5 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language5Option2Key", "Language 5 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 5 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language5Option3Key", "Language 5 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 5 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language5Option4Key", "Language 5 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 5 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language5Option5Key", "Language 5 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 6 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language6Option1Key", "Language 6 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 6 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language6Option2Key", "Language 6 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 6 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language6Option3Key", "Language 6 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 6 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language6Option4Key", "Language 6 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 6 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language6Option5Key", "Language 6 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 7 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language7Option1Key", "Language 7 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 7 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language7Option2Key", "Language 7 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 7 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language7Option3Key", "Language 7 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 7 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language7Option4Key", "Language 7 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 7 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language7Option5Key", "Language 7 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 8 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language8Option1Key", "Language 8 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 8 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language8Option2Key", "Language 8 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 8 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language8Option3Key", "Language 8 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 8 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language8Option4Key", "Language 8 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 8 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language8Option5Key", "Language 8 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 9 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language9Option1Key", "Language 9 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 9 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language9Option2Key", "Language 9 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 9 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language9Option3Key", "Language 9 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 9 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language9Option4Key", "Language 9 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 9 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language9Option5Key", "Language 9 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 10 Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language10Option1Key", "Language 10 Option 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 10 Option 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language10Option2Key", "Language 10 Option 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 10 Option 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language10Option3Key", "Language 10 Option 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 10 Option 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language10Option4Key", "Language 10 Option 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Language 10 Option 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language10Option5Key", "Language 10 Option 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("MaxLength"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MaxLengthKey", "MaxLength"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {
								{TEXT("HasToggle"), TEXT("true")},
								{TEXT("Min"), TEXT("1")}
							}
						}
					},
					{
						.ArgName = TEXT("MaxLineCount"),
						.DefaultValue = TEXT("1"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MaxLineCountKey", "MaxLineCount"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {
								{TEXT("HasToggle"), TEXT("true")},
								{TEXT("Min"), TEXT("1")}
							}
						}
					},
					{
						.ArgName = TEXT("AllowedCharacterRegex"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("AllowedCharacterRegexKey", "AllowedCharacterRegex"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("DefaultText"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DefaultTextKey", "DefaultText"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("HintText"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("HintTextKey", "HintText"),
							.TemplateWidget = MultilineTextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("DestinationVariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableKindKey", "DestinationVariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("DestinationVariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("DestinationVariableKind")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")}
							}
						}
					},
					{
						.ArgName = TEXT("HideTextLayer"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("HideTextLayerKey", "HideTextLayer"),
							.TemplateWidget = BooleanInputTemplate
						}
					}
				}
			}
		},
		{
			TEXT("AssignVariable"),
			FShidenCommandDefinition
			{
				.Style = DataStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenAssignVariableCommand")),
				.Args =
				{
					{
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")}
							}
						}
					},
					{
						.ArgName = TEXT("Value"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ValueKey", "Value"),
							.TemplateWidget = FlexInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")}
							}
						}
					},
					{
						.ArgName = TEXT("Expression"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ExpressionKey", "Expression"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")},
								{TEXT("AllowedVariableTypesForLiteral"), TEXT("String,Integer,Float,Vector2,Vector3")}
							}
						}
					},
					{
						.ArgName = TEXT("Operator"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OperatorKey", "Operator"),
							.TemplateWidget = FlexCalculationOperatorInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
					},
					{
						.ArgName = TEXT("Value"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ValueKey", "Value"),
							.TemplateWidget = FlexInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")}}
						}
					},
					{
						.ArgName = TEXT("Operator"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OperatorKey", "Operator"),
							.TemplateWidget = FlexOperatorInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
					},
					{
						.ArgName = TEXT("RightHandValue"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("RightHandValueKey", "RightHandValue"),
							.TemplateWidget = FlexInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")}}
						}
					},
					{
						.ArgName = TEXT("Operator"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OperatorKey", "Operator"),
							.TemplateWidget = FlexOperatorInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
					},
					{
						.ArgName = TEXT("RightHandValue"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("RightHandValueKey", "RightHandValue"),
							.TemplateWidget = FlexInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
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
						.ArgName = TEXT("Expression"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ExpressionKey", "Expression"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("Expression"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ExpressionKey", "Expression"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")}}
						}
					},
					{
						.ArgName = TEXT("Operator"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OperatorKey", "Operator"),
							.TemplateWidget = FlexOperatorInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
					},
					{
						.ArgName = TEXT("RightHandValue"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("RightHandValueKey", "RightHandValue"),
							.TemplateWidget = FlexInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
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
						.ArgName = TEXT("Expression"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ExpressionKey", "Expression"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("Name"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("NameKey", "Name"),
							.TemplateWidget = TextInputTemplate
						}
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
						.ArgName = TEXT("DestinationTagName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationTagNameKey", "DestinationTagName"),
							.TemplateWidget = TextInputTemplate
						}
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
						.ArgName = TEXT("NextScenario"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("NextScenarioKey", "NextScenario"),
							.TemplateWidget = ScenarioInputTemplate,
							.TemplateParameters = {
								{TEXT("Target"), TEXT("Scenario")},
								{TEXT("HasToggle"), TEXT("true")}
							}
						}
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
						.ArgName = TEXT("ReasonForInterruption"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ReasonForInterruptionKey", "ReasonForInterruption"),
							.TemplateWidget = TextInputTemplate
						}
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
						.ArgName = TEXT("Comment"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("CommentKey", "Comment"),
							.TemplateWidget = TextInputTemplate
						}
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
						.ArgName = TEXT("AnimationName"),
						.DefaultValue = TEXT("Shake"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("AnimationNameKey", "AnimationName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("StartTime"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StartTimeKey", "StartTime"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("NumLoopToPlay"),
						.DefaultValue = TEXT("1"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("NumLoopToPlayKey", "NumLoopToPlay"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("PlayMode"),
						.DefaultValue = TEXT("Forward"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("PlayModeKey", "PlayMode"),
							.TemplateWidget = ComboBoxInputTemplate,
							.TemplateParameters = {
								{TEXT("Option1"), TEXT("Forward")},
								{TEXT("Option2"), TEXT("Reverse")},
								{TEXT("Option3"), TEXT("Ping Pong")}
							},
						}
					},
					{
						.ArgName = TEXT("PlaybackSpeed"),
						.DefaultValue = TEXT("1.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("PlaybackSpeedKey", "PlaybackSpeed"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("RestoreState"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("RestoreStateKey", "RestoreState"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("Duration"),
						.DefaultValue = TEXT("1.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DurationKey", "Duration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
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
						.ArgName = TEXT("FunctionName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FunctionNameKey", "FunctionName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Arg 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg1Key", "Arg 1"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg2Key", "Arg 2"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 3"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg3Key", "Arg 3"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 4"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg4Key", "Arg 4"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 5"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg5Key", "Arg 5"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 6"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg6Key", "Arg 6"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 7"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg7Key", "Arg 7"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 8"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg8Key", "Arg 8"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 9"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg9Key", "Arg 9"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Arg 10"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg10Key", "Arg 10"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
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
						.ArgName = TEXT("LayerName"),
						.DefaultValue = TEXT("Default"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("LayerNameKey", "LayerName"),
							.TemplateWidget = TextInputTemplate,
							.ErrorMessages = {
								{
									.Condition = TEXT("StartsWith({ LayerName }, \"Global$\")"),
									.Message = LOCTEXT("LayerNameGlobalPrefixErrorKey", "LayerName cannot start with 'Global$' (reserved prefix).")
								}
							}
						}
					},
					{
						.ArgName = TEXT("FadeType"),
						.DefaultValue = TEXT("FadeOut"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeTypeKey", "FadeType"),
							.TemplateWidget = FadeTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("FadeFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeFunctionKey", "FadeFunction"),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("TargetColor"),
						.DefaultValue = TEXT("R=0.0 G=0.0 B=0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TargetColorKey", "TargetColor"),
							.TemplateWidget = ColorInputTemplate
						}
					},
					{
						.ArgName = TEXT("Steps"),
						.DefaultValue = TEXT("2"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StepsKey", "Steps"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("1")}},
							.VisibilityCondition = TEXT("HasVariable({ FadeFunction }) || { FadeFunction } == \"Step\"")
						}
					},
					{
						.ArgName = TEXT("BlendExp"),
						.DefaultValue = TEXT("2.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("BlendExpKey", "BlendExp"),
							.TemplateWidget = FloatInputTemplate,
							.VisibilityCondition = TEXT("HasVariable({ FadeFunction }) || { FadeFunction } == \"Ease In\" || { FadeFunction } == \"Ease Out\" || { FadeFunction } == \"Ease In Out\"")
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("ZOrder"),
						.DefaultValue = TEXT("250"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ZOrderKey", "ZOrder"),
							.TemplateWidget = IntegerInputTemplate
						}
					},
					{
						.ArgName = TEXT("UseGlobalFade"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("UseGlobalFadeKey", "UseGlobalFade"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("Name"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("NameKey", "Name"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Visibility"),
						.DefaultValue = TEXT("Visible"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VisibilityKey", "Visibility"),
							.TemplateWidget = SlateVisibilityInputTemplate
						}
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
						.ArgName = TEXT("SlotName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("SlotNameKey", "SlotName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("EasingFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("EasingFunctionKey", "EasingFunction"),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("Duration"),
						.DefaultValue = TEXT("1.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DurationKey", "Duration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("ChangeType"),
						.DefaultValue = TEXT("AbsoluteValue"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ChangeTypeKey", "ChangeType"),
							.TemplateWidget = ComboBoxInputTemplate,
							.TemplateParameters = {
								{TEXT("Option1"), TEXT("AbsoluteValue")},
								{TEXT("Option2"), TEXT("AddToCurrent")}
							}
						}
					},
					{
						.ArgName = TEXT("OverwritePosition"),
						.DefaultValue = TEXT("{EMPTY}"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwritePositionKey", "OverwritePosition"),
							.TemplateWidget = Vector2InputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("OverwriteSize"),
						.DefaultValue = TEXT("{EMPTY}"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwriteSizeKey", "OverwriteSize"),
							.TemplateWidget = Vector2InputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("OverwriteZOrder"),
						.DefaultValue = TEXT("{EMPTY}"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwriteZOrderKey", "OverwriteZOrder"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("HasToggle"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("Steps"),
						.DefaultValue = TEXT("2"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StepsKey", "Steps"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("1")}},
							.VisibilityCondition = TEXT("HasVariable({ EasingFunction }) || { EasingFunction } == \"Step\"")
						}
					},
					{
						.ArgName = TEXT("BlendExp"),
						.DefaultValue = TEXT("2.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("BlendExpKey", "BlendExp"),
							.TemplateWidget = FloatInputTemplate,
							.VisibilityCondition = TEXT("HasVariable({ EasingFunction }) || { EasingFunction } == \"Ease In\" || { EasingFunction } == \"Ease Out\" || { EasingFunction } == \"Ease In Out\"")
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("SlotName"),
						.DefaultValue = TEXT("Auto Save"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("SlotNameKey", "SlotName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("OverwriteThumbnail"),
						.DefaultValue = TEXT("{EMPTY}"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwriteThumbnailKey", "OverwriteThumbnail"),
							.TemplateWidget = AssetInputTemplate,
							.TemplateParameters = {
								{TEXT("Target"), TEXT("Texture")},
								{TEXT("HasToggle"), TEXT("true")}
							},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ OverwriteThumbnail })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("DestinationVariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("ExcludeReadOnlyFromLiteral"), TEXT("true")},
								{TEXT("AllowedVariableTypesForLiteral"), TEXT("Integer,Float")}
							}
						}
					},
					{
						.ArgName = TEXT("Min"),
						.DefaultValue = TEXT("0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MinKey", "Min"),
							.TemplateWidget = FlexNumberInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("DestinationVariableName")}
							}
						}
					},
					{
						.ArgName = TEXT("Max"),
						.DefaultValue = TEXT("1"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MaxKey", "Max"),
							.TemplateWidget = FlexNumberInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("DestinationVariableName")}
							},
							.ErrorMessages = {
								{
									.Condition = TEXT("!HasVariable({ Min }) && !HasVariable({ Max }) && ToFloat({ Max }) < ToFloat({ Min })"),
									.Message = LOCTEXT("MaxLessThanMinError", "The Max value must be greater than or equal to the Min value.")
								}
							}
						}
					}
				}
			}
		},
		{
			TEXT("WaitForClick"),
			FShidenCommandDefinition
			{
				.Style = InteractionStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenWaitForClickCommand")),
				.Args =
				{
					{
						.ArgName = TEXT("CanSkip"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("CanSkipKey", "CanSkip"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("VariableKind"),
						.DefaultValue = TEXT("UserVariable"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableKindKey", "VariableKind"),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.TemplateWidget = VariableNameInputTemplate,
							.TemplateParameters = {{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")}}
						}
					},
					{
						.ArgName = TEXT("Operator"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OperatorKey", "Operator"),
							.TemplateWidget = FlexOperatorInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
					},
					{
						.ArgName = TEXT("RightHandValue"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("RightHandValueKey", "RightHandValue"),
							.TemplateWidget = FlexInputTemplate,
							.TemplateParameters = {
								{TEXT("VariableKindSourceArgName"), TEXT("VariableKind")},
								{TEXT("VariableNameSourceArgName"), TEXT("VariableName")}
							}
						}
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
						.ArgName = TEXT("FeedbackEffect"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FeedbackEffectKey", "FeedbackEffect"),
							.TemplateWidget = AssetInputTemplate,
							.TemplateParameters = {{TEXT("Target"), TEXT("ForceFeedback")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ FeedbackEffect })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
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
						.ArgName = TEXT("MediaSource"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MediaSourceKey", "MediaSource"),
							.TemplateWidget = AssetInputTemplate,
							.TemplateParameters = {{TEXT("Target"), TEXT("MediaSource")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ MediaSource })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("CanOpenPauseMenu"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("CanOpenPauseMenuKey", "CanOpenPauseMenu"),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("MediaZOrder"),
						.DefaultValue = TEXT("100"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MediaZOrderKey", "MediaZOrder"),
							.TemplateWidget = IntegerInputTemplate
						}
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
						.ArgName = TEXT("Target"),
						.DefaultValue = TEXT("Image"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TargetKey", "Target"),
							.TemplateWidget = ComboBoxInputTemplate,
							.TemplateParameters = {
								{TEXT("Option1"), TEXT("Image")},
								{TEXT("Option2"), TEXT("RetainerBox")}
							},
						}
					},
					{
						.ArgName = TEXT("TargetName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TargetNameKey", "TargetName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("ParameterName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ParameterNameKey", "ParameterName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("EasingFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("EasingFunctionKey", "EasingFunction"),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("Duration"),
						.DefaultValue = TEXT("1.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DurationKey", "Duration"),
							.TemplateWidget = FloatInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("0")}}
						}
					},
					{
						.ArgName = TEXT("ChangeType"),
						.DefaultValue = TEXT("AbsoluteValue"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ChangeTypeKey", "ChangeType"),
							.TemplateWidget = ComboBoxInputTemplate,
							.TemplateParameters = {
								{TEXT("Option1"), TEXT("AbsoluteValue")},
								{TEXT("Option2"), TEXT("AddToCurrent")}
							},
						}
					},
					{
						.ArgName = TEXT("EndValue"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("EndValueKey", "EndValue"),
							.TemplateWidget = FloatInputTemplate
						}
					},
					{
						.ArgName = TEXT("Steps"),
						.DefaultValue = TEXT("2"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StepsKey", "Steps"),
							.TemplateWidget = IntegerInputTemplate,
							.TemplateParameters = {{TEXT("Min"), TEXT("1")}},
							.VisibilityCondition = TEXT("HasVariable({ EasingFunction }) || { EasingFunction } == \"Step\"")
						}
					},
					{
						.ArgName = TEXT("BlendExp"),
						.DefaultValue = TEXT("2.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("BlendExpKey", "BlendExp"),
							.TemplateWidget = FloatInputTemplate,
							.VisibilityCondition = TEXT("HasVariable({ EasingFunction }) || { EasingFunction } == \"Ease In\" || { EasingFunction } == \"Ease Out\" || { EasingFunction } == \"Ease In Out\"")
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("Target"),
						.DefaultValue = TEXT("Image"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TargetKey", "Target"),
							.TemplateWidget = ComboBoxInputTemplate,
							.TemplateParameters = {
								{TEXT("Option1"), TEXT("Image")},
								{TEXT("Option2"), TEXT("RetainerBox")}
							},
						}
					},
					{
						.ArgName = TEXT("TargetName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TargetNameKey", "TargetName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("ParameterName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ParameterNameKey", "ParameterName"),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Texture"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextureKey", "Texture"),
							.TemplateWidget = AssetInputTemplate,
							.TemplateParameters = {{TEXT("Target"), TEXT("Texture")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ Texture })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							}
						}
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
						.ArgName = TEXT("MacroName"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MacroNameKey", "MacroName"),
							.TemplateWidget = ScenarioInputTemplate,
							.TemplateParameters = {
								{TEXT("Target"), TEXT("macro")},
								{TEXT("HasAdditionalArgs"), TEXT("true")}
							},
						}
					}
				}
			}
		},
		{
			TEXT("RunMacroInParallel"),
			FShidenCommandDefinition
			{
				.Note = LOCTEXT("RunMacroInParallelNoteKey",
				                "The arguments passed to the macro are treated as local variables.\r\nThis command is not supported in preview."),
				.Style = MacroStyle,
				.bCanCallInMacro = true,
				.CommandSoftObjectPath = FSoftObjectPath(TEXT("/Script/ShidenCore.ShidenRunMacroInParallelCommand")),
				.Args =
				{
					{
						.ArgName = TEXT("NewProcessName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("NewProcessNameKey", "NewProcessName"),
							.TemplateWidget = TextInputTemplate,
							.ErrorMessages = {
								{
									.Condition = TEXT("{ NewProcessName } == \"Default\""),
									.Message = LOCTEXT("NewProcessNameError", "\"Default\" cannot be used as a process name.")
								},
								{
									.Condition = TEXT("IsEmpty({ NewProcessName })"),
									.Message = LOCTEXT("EmptyProcessNameError", "Please specify the process name.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("MacroName"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MacroNameKey", "MacroName"),
							.TemplateWidget = ScenarioInputTemplate,
							.TemplateParameters = {
								{TEXT("Target"), TEXT("macro")},
								{TEXT("HasAdditionalArgs"), TEXT("true")}
							},
						}
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
						.ArgName = TEXT("ProcessName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ProcessNameKey", "ProcessName"),
							.TemplateWidget = TextInputTemplate,
							.ErrorMessages = {
								{
									.Condition = TEXT("{ NewProcessName } == \"Default\""),
									.Message = LOCTEXT("NewProcessNameError", "\"Default\" cannot be used as a process name.")
								},
								{
									.Condition = TEXT("IsEmpty({ ProcessName })"),
									.Message = LOCTEXT("EmptyProcessNameError", "Please specify the process name.")
								}
							}
						}
					},
					{
						.ArgName = TEXT("Reason"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ReasonKey", "Reason"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					},
					{
						.ArgName = TEXT("StopTiming"),
						.DefaultValue = TEXT("Immediately"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StopTimingKey", "StopTiming"),
							.TemplateWidget = ComboBoxInputTemplate,
							.TemplateParameters = {
								{TEXT("Option1"), TEXT("Immediately")},
								{TEXT("Option2"), TEXT("OnCurrentCommandEnd")}
							},
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.TemplateWidget = BooleanInputTemplate
						}
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
						.ArgName = TEXT("Content"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ContentKey", "Content"),
							.TemplateWidget = TextInputTemplate,
							.TemplateParameters = {{TEXT("ShowVariableInsertButton"), TEXT("true")}}
						}
					}
				}
			}
		}
	};
}

#undef LOCTEXT_NAMESPACE
