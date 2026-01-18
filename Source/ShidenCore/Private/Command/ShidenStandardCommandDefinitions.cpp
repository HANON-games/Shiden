// Copyright (c) 2026 HANON. All Rights Reserved.

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
							.ToolTip = LOCTEXT("TextWidgetNameTooltip", "Specifies the text widget name where text will be displayed."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("TextType"),
						.DefaultValue = TEXT("Default"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextTypeKey", "TextType"),
							.ToolTip = LOCTEXT("TextTypeTooltip", "Specifies the text type.\r\nThe display frame changes based on this type."),
							.TemplateWidget = TextTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("Text"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextKey", "Text"),
							.ToolTip = LOCTEXT("TextTooltip", "Specifies the text to display."),
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
							.ToolTip = LOCTEXT("WaitForInputTooltip", "Specifies whether to wait for user interaction after text display."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("InstantTextDisplay"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("InstantTextDisplayKey", "InstantTextDisplay"),
							.ToolTip = LOCTEXT("InstantTextDisplayTooltip", "Specifies whether to display text immediately."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("Voice"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VoiceKey", "Voice"),
							.ToolTip = LOCTEXT("VoiceTooltip", "Specifies the voice to play alongside the text."),
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
							.ToolTip = LOCTEXT("VoiceTrackIdTooltip", "Specifies the voice track ID."),
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
							.ToolTip = LOCTEXT("TextBlipTooltip", "Specifies the text blip sound."),
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
							.ToolTip = LOCTEXT("DisableAutoStopPreviousVoicesTooltip", "Specifies whether to prevent stopping the previous voice playback,\r\neven when the setting to stop previous voices\r\nduring dialogue advancement or voice playback is enabled.\r\nUse this when you want voices to overlap."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("ContinueFromThePreviousText"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ContinueFromThePreviousTextKey", "ContinueFrom\r\nThePreviousText"),
							.ToolTip = LOCTEXT("ContinueFromThePreviousTextTooltip", "Specifies whether to append to the previous text."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("Language 2"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Language2Key", "Language 2"),
							.ToolTip = LOCTEXT("Language2Tooltip", "Specifies the display text for the second language."),
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
							.ToolTip = LOCTEXT("Language3Tooltip", "Specifies the display text for the third language."),
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
							.ToolTip = LOCTEXT("Language4Tooltip", "Specifies the display text for the fourth language."),
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
							.ToolTip = LOCTEXT("Language5Tooltip", "Specifies the display text for the fifth language."),
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
							.ToolTip = LOCTEXT("Language6Tooltip", "Specifies the display text for the sixth language."),
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
							.ToolTip = LOCTEXT("Language7Tooltip", "Specifies the display text for the seventh language."),
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
							.ToolTip = LOCTEXT("Language8Tooltip", "Specifies the display text for the eighth language."),
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
							.ToolTip = LOCTEXT("Language9Tooltip", "Specifies the display text for the ninth language."),
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
							.ToolTip = LOCTEXT("Language10Tooltip", "Specifies the display text for the tenth language."),
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
							.ToolTip = LOCTEXT("ImageSlotNameTooltip", "Specifies the slot where the image will be displayed."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Image"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ImageKey", "Image"),
							.ToolTip = LOCTEXT("ImageTooltip", "Specifies the image to display.\r\nIf not specified, the image will fade out."),
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
							.ToolTip = LOCTEXT("ImageOverwritePositionTooltip", "Specifies the position to override when displaying the image."),
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
							.ToolTip = LOCTEXT("ImageOverwriteSizeTooltip", "Specifies the size to override when displaying the image."),
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
							.ToolTip = LOCTEXT("ImageOverwriteSizeToContentTooltip", "Specifies the SizeToContent setting to override when displaying the image."),
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
							.ToolTip = LOCTEXT("FadeFunctionTooltip", "Specifies how the fade works."),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.ToolTip = LOCTEXT("FadeDurationTooltip", "Specifies the fade duration in seconds."),
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
							.ToolTip = LOCTEXT("StepsTooltip", "Specifies the number of steps, used by the Step fade function."),
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
							.ToolTip = LOCTEXT("BlendExpTooltip", "Specifies the exponent used by certain blend functions\r\nto control the shape of the curve."),
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
							.ToolTip = LOCTEXT("WaitForAnimationCompletionTooltip", "Specifies whether to wait for animation completion before proceeding to the next command."),
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
							.ToolTip = LOCTEXT("SoundTypeTooltip", "Specifies the sound type."),
							.TemplateWidget = SoundTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("TrackId"),
						.DefaultValue = TEXT("0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TrackIdKey", "TrackId"),
							.ToolTip = LOCTEXT("TrackIdTooltip", "Specifies the track ID for playback.\r\nIf a sound is already playing on that track ID, it will be stopped."),
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
							.ToolTip = LOCTEXT("SoundSourceTooltip", "Specifies the sound source to play.\r\nIf empty, stops the currently playing sound on that track ID."),
							.TemplateWidget = SoundInputTemplate,
							.TemplateParameters = {{TEXT("SoundTypeSourceArgName"), TEXT("SoundType")}},
							.WarningMessages = {
								{
									.Condition = TEXT("IsWritableVariable({ SoundSource })"),
									.Message = LOCTEXT("LazyLoadAssetWarning", "This asset will be loaded when this line is reached.\nSince it may cause loading delays, it is not suitable for large assets.\nAdditionally, assets specified via this variable will not appear\nin the Shiden Asset List.")
								}
							},
							.ErrorMessages = {
								{
									.Condition = TEXT("{ SoundType } == \"SE\" && ({ SoundSource } == \"{EMPTY}\" || { SoundSource } == \"None\")"),
									.Message = LOCTEXT("SESoundSourceEmptyError", "Sound source cannot be empty when SoundType is SE.")
								}
							},
						}
					},
					{
						.ArgName = TEXT("Volume"),
						.DefaultValue = TEXT("1.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VolumeKey", "Volume"),
							.ToolTip = LOCTEXT("VolumeTooltip", "Specifies the playback volume."),
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
							.ToolTip = LOCTEXT("PitchTooltip", "Specifies the sound pitch."),
							.TemplateWidget = FloatInputTemplate
						}
					},
					{
						.ArgName = TEXT("StartTime"),
						.DefaultValue = TEXT("0.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StartTimeKey", "StartTime"),
							.ToolTip = LOCTEXT("StartTimeTooltip", "Specifies the starting position of the sound."),
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
							.ToolTip = LOCTEXT("FadeTypeTooltip", "Specifies whether to fade in or fade out the sound."),
							.TemplateWidget = FadeTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeFunctionKey", "FadeFunction"),
							.ToolTip = LOCTEXT("FadeFunctionTooltip", "Specifies how the fade works."),
							.TemplateWidget = BGMFadeFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.ToolTip = LOCTEXT("FadeDurationTooltip", "Specifies the fade duration in seconds."),
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
							.ToolTip = LOCTEXT("DisableAutoStopPreviousVoicesTooltip", "Specifies whether to prevent stopping the previous voice playback,\r\neven when the setting to stop previous voices\r\nduring dialogue advancement or voice playback is enabled.\r\nUse this when you want voices to overlap."),
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
							.ToolTip = LOCTEXT("WaitForFadeCompletionTooltip", "Specifies whether to wait for fade completion before proceeding to the next command."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("WaitForSoundCompletion"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForSoundCompletionKey", "WaitForSoundCompletion"),
							.ToolTip = LOCTEXT("WaitForSoundCompletionTooltip", "Specifies whether to wait for sound completion.\r\nNote: This will cause an error if the specified SoundCue is set to loop."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("UseGlobalBGM"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("UseGlobalBGMKey", "UseGlobalBGM"),
							.ToolTip = LOCTEXT("UseGlobalBGMTooltip", "Specifies whether to use global BGM.\r\nGlobal BGM can be played across scenarios."),
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
							.ToolTip = LOCTEXT("AdjustBGMVolumeTrackIdTooltip", "Specifies the target BGM track ID."),
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
							.ToolTip = LOCTEXT("VolumeTooltip", "Specifies the playback volume."),
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
							.ToolTip = LOCTEXT("FadeFunctionTooltip", "Specifies how the fade works."),
							.TemplateWidget = BGMFadeFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.00"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.ToolTip = LOCTEXT("FadeDurationTooltip", "Specifies the fade duration in seconds."),
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
							.ToolTip = LOCTEXT("AdjustBGMVolumeWaitForCompletionTooltip", "Specifies whether to wait for volume adjustment completion."),
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
							.ToolTip = LOCTEXT("OptionsDestinationVariableKindTooltip", "Specifies the variable kind to store the selection result."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("DestinationVariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
							.ToolTip = LOCTEXT("OptionsAssignVariableNameTooltip", "Specifies the variable name to store the selection result.\r\nAfter a choice is made, this variable will be assigned\r\na value from 0-4 based on the selected choice."),
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
							.ToolTip = LOCTEXT("OptionsHideTextLayerTooltip", "Specifies whether to hide the text layer while displaying options."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("Option 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Option1Key", "Option 1"),
							.ToolTip = LOCTEXT("Option1Tooltip", "Specifies the display content for option 1."),
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
							.ToolTip = LOCTEXT("Option2Tooltip", "Specifies the display content for option 2."),
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
							.ToolTip = LOCTEXT("Option3Tooltip", "Specifies the display content for option 3."),
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
							.ToolTip = LOCTEXT("Option4Tooltip", "Specifies the display content for option 4."),
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
							.ToolTip = LOCTEXT("Option5Tooltip", "Specifies the display content for option 5."),
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
							.ToolTip = LOCTEXT("Language2Option1Tooltip", "Specifies the option 1 content for language 2."),
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
							.ToolTip = LOCTEXT("Language2Option2Tooltip", "Specifies the option 2 content for language 2."),
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
							.ToolTip = LOCTEXT("Language2Option3Tooltip", "Specifies the option 3 content for language 2."),
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
							.ToolTip = LOCTEXT("Language2Option4Tooltip", "Specifies the option 4 content for language 2."),
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
							.ToolTip = LOCTEXT("Language2Option5Tooltip", "Specifies the option 5 content for language 2."),
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
							.ToolTip = LOCTEXT("Language3Option1Tooltip", "Specifies the option 1 content for language 3."),
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
							.ToolTip = LOCTEXT("Language3Option2Tooltip", "Specifies the option 2 content for language 3."),
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
							.ToolTip = LOCTEXT("Language3Option3Tooltip", "Specifies the option 3 content for language 3."),
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
							.ToolTip = LOCTEXT("Language3Option4Tooltip", "Specifies the option 4 content for language 3."),
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
							.ToolTip = LOCTEXT("Language3Option5Tooltip", "Specifies the option 5 content for language 3."),
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
							.ToolTip = LOCTEXT("Language4Option1Tooltip", "Specifies the option 1 content for language 4."),
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
							.ToolTip = LOCTEXT("Language4Option2Tooltip", "Specifies the option 2 content for language 4."),
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
							.ToolTip = LOCTEXT("Language4Option3Tooltip", "Specifies the option 3 content for language 4."),
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
							.ToolTip = LOCTEXT("Language4Option4Tooltip", "Specifies the option 4 content for language 4."),
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
							.ToolTip = LOCTEXT("Language4Option5Tooltip", "Specifies the option 5 content for language 4."),
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
							.ToolTip = LOCTEXT("Language5Option1Tooltip", "Specifies the option 1 content for language 5."),
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
							.ToolTip = LOCTEXT("Language5Option2Tooltip", "Specifies the option 2 content for language 5."),
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
							.ToolTip = LOCTEXT("Language5Option3Tooltip", "Specifies the option 3 content for language 5."),
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
							.ToolTip = LOCTEXT("Language5Option4Tooltip", "Specifies the option 4 content for language 5."),
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
							.ToolTip = LOCTEXT("Language5Option5Tooltip", "Specifies the option 5 content for language 5."),
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
							.ToolTip = LOCTEXT("Language6Option1Tooltip", "Specifies the option 1 content for language 6."),
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
							.ToolTip = LOCTEXT("Language6Option2Tooltip", "Specifies the option 2 content for language 6."),
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
							.ToolTip = LOCTEXT("Language6Option3Tooltip", "Specifies the option 3 content for language 6."),
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
							.ToolTip = LOCTEXT("Language6Option4Tooltip", "Specifies the option 4 content for language 6."),
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
							.ToolTip = LOCTEXT("Language6Option5Tooltip", "Specifies the option 5 content for language 6."),
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
							.ToolTip = LOCTEXT("Language7Option1Tooltip", "Specifies the option 1 content for language 7."),
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
							.ToolTip = LOCTEXT("Language7Option2Tooltip", "Specifies the option 2 content for language 7."),
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
							.ToolTip = LOCTEXT("Language7Option3Tooltip", "Specifies the option 3 content for language 7."),
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
							.ToolTip = LOCTEXT("Language7Option4Tooltip", "Specifies the option 4 content for language 7."),
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
							.ToolTip = LOCTEXT("Language7Option5Tooltip", "Specifies the option 5 content for language 7."),
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
							.ToolTip = LOCTEXT("Language8Option1Tooltip", "Specifies the option 1 content for language 8."),
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
							.ToolTip = LOCTEXT("Language8Option2Tooltip", "Specifies the option 2 content for language 8."),
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
							.ToolTip = LOCTEXT("Language8Option3Tooltip", "Specifies the option 3 content for language 8."),
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
							.ToolTip = LOCTEXT("Language8Option4Tooltip", "Specifies the option 4 content for language 8."),
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
							.ToolTip = LOCTEXT("Language8Option5Tooltip", "Specifies the option 5 content for language 8."),
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
							.ToolTip = LOCTEXT("Language9Option1Tooltip", "Specifies the option 1 content for language 9."),
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
							.ToolTip = LOCTEXT("Language9Option2Tooltip", "Specifies the option 2 content for language 9."),
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
							.ToolTip = LOCTEXT("Language9Option3Tooltip", "Specifies the option 3 content for language 9."),
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
							.ToolTip = LOCTEXT("Language9Option4Tooltip", "Specifies the option 4 content for language 9."),
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
							.ToolTip = LOCTEXT("Language9Option5Tooltip", "Specifies the option 5 content for language 9."),
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
							.ToolTip = LOCTEXT("Language10Option1Tooltip", "Specifies the option 1 content for language 10."),
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
							.ToolTip = LOCTEXT("Language10Option2Tooltip", "Specifies the option 2 content for language 10."),
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
							.ToolTip = LOCTEXT("Language10Option3Tooltip", "Specifies the option 3 content for language 10."),
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
							.ToolTip = LOCTEXT("Language10Option4Tooltip", "Specifies the option 4 content for language 10."),
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
							.ToolTip = LOCTEXT("Language10Option5Tooltip", "Specifies the option 5 content for language 10."),
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
							.ToolTip = LOCTEXT("MaxLengthTooltip", "Specifies the maximum number of characters that can be entered.\r\nUnlimited if not specified."),
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
							.ToolTip = LOCTEXT("MaxLineCountTooltip", "Specifies the maximum number of lines that can be entered.\r\nUnlimited if not specified."),
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
							.ToolTip = LOCTEXT("AllowedCharacterRegexTooltip", "Specifies allowed characters using regular expression."),
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
							.ToolTip = LOCTEXT("DefaultTextTooltip", "Specifies the content that is pre-filled in the input field."),
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
							.ToolTip = LOCTEXT("HintTextTooltip", "Specifies the content shown when the input field is empty."),
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
							.ToolTip = LOCTEXT("TextInputDestinationVariableKindTooltip", "Specifies the variable type to store the input result."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("DestinationVariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
							.ToolTip = LOCTEXT("TextInputAssignVariableNameTooltip", "Specifies the variable name to store the input result."),
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
							.ToolTip = LOCTEXT("TextInputHideTextLayerTooltip", "Specifies whether to hide the text layer while displaying the input field."),
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
							.ToolTip = LOCTEXT("AssignVariableKindTooltip", "Specifies the variable type to assign the value to."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("AssignVariableNameTooltip", "Specifies the variable name to assign the value to."),
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
							.ToolTip = LOCTEXT("AssignVariableValueTooltip", "Specifies the value to assign."),
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
							.ToolTip = LOCTEXT("AssignVariableKindTooltip", "Specifies the variable type to assign the value to."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("AssignVariableNameTooltip", "Specifies the variable name to assign the value to."),
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
							.ToolTip = LOCTEXT("ExpressionToEvaluateTooltip", "Specifies the expression to evaluate."),
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
							.ToolTip = LOCTEXT("CalculationVariableKindTooltip", "Specifies the variable type to modify."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("CalculationVariableNameTooltip", "Specifies the variable name to modify.\r\nThe calculation result will be stored in this variable."),
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
							.ToolTip = LOCTEXT("CalculationOperatorTooltip", "Specifies the calculation operator."),
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
							.ToolTip = LOCTEXT("CalculationValueTooltip", "Specifies the value to use in the calculation."),
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
							.ToolTip = LOCTEXT("ConditionVariableKindTooltip", "Specifies the variable type to use for the condition."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("ConditionVariableNameTooltip", "Specifies the variable name to use for the condition."),
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
							.ToolTip = LOCTEXT("ConditionOperatorTooltip", "Specifies the condition operator."),
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
							.ToolTip = LOCTEXT("ConditionRightHandValueTooltip", "Specifies the right-hand value for the condition."),
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
							.ToolTip = LOCTEXT("ConditionVariableKindTooltip", "Specifies the variable type to use for the condition."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("ConditionVariableNameTooltip", "Specifies the variable name to use for the condition."),
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
							.ToolTip = LOCTEXT("ConditionOperatorTooltip", "Specifies the condition operator."),
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
							.ToolTip = LOCTEXT("ConditionRightHandValueTooltip", "Specifies the right-hand value for the condition."),
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
							.ToolTip = LOCTEXT("ExpressionToEvaluateTooltip", "Specifies the expression to evaluate."),
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
							.ToolTip = LOCTEXT("ExpressionToEvaluateTooltip", "Specifies the expression to evaluate."),
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
							.ToolTip = LOCTEXT("ConditionVariableKindTooltip", "Specifies the variable type to use for the condition."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("ConditionVariableNameTooltip", "Specifies the variable name to use for the condition."),
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
							.ToolTip = LOCTEXT("ConditionOperatorTooltip", "Specifies the condition operator."),
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
							.ToolTip = LOCTEXT("ConditionRightHandValueTooltip", "Specifies the right-hand value for the condition."),
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
							.ToolTip = LOCTEXT("ExpressionToEvaluateTooltip", "Specifies the expression to evaluate."),
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
							.ToolTip = LOCTEXT("TagNameTooltip", "Specifies the tag name.\r\nPlease avoid using the same tag name twice in the same scenario."),
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
							.ToolTip = LOCTEXT("JumpDestinationTagNameTooltip", "Specifies the tag name to jump to."),
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
							.ToolTip = LOCTEXT("NextScenarioTooltip", "Specifies the next scenario to play."),
							.TemplateWidget = ScenarioInputTemplate,
							.TemplateParameters = {
								{TEXT("Target"), TEXT("Scenario")},
								{TEXT("HasToggle"), TEXT("true")}
							},
							.VisibilityCondition = TEXT("!IsCalledFromMacro()")
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
							.ToolTip = LOCTEXT("BreakReasonTooltip", "Specifies the reason for interruption."),
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
							.ToolTip = LOCTEXT("CommentTooltip", "Specifies the comment text."),
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
							.ToolTip = LOCTEXT("AnimationNameTooltip", "Specifies the animation name."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("StartTime"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StartTimeKey", "StartTime"),
							.ToolTip = LOCTEXT("WidgetAnimationStartTimeTooltip", "Specifies the start position of the animation."),
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
							.ToolTip = LOCTEXT("NumLoopToPlayTooltip", "Specifies the number of times to loop the animation."),
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
							.ToolTip = LOCTEXT("PlayModeTooltip", "Specifies the playback mode."),
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
							.ToolTip = LOCTEXT("PlaybackSpeedTooltip", "Specifies the playback speed."),
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
							.ToolTip = LOCTEXT("RestoreStateTooltip", "Specifies whether to restore the state."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("WaitForCompletion"),
						.DefaultValue = TEXT("true"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("WaitForCompletionKey", "WaitForCompletion"),
							.ToolTip = LOCTEXT("WaitForAnimationCompletionTooltip", "Specifies whether to wait for animation completion before proceeding to the next command."),
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
							.ToolTip = LOCTEXT("DelayDurationTooltip", "Specifies the wait time in seconds."),
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
							.ToolTip = LOCTEXT("FunctionNameTooltip", "Specifies the function name to call."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Arg 1"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("Arg1Key", "Arg 1"),
							.ToolTip = LOCTEXT("Arg1Tooltip", "Specifies argument 1."),
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
							.ToolTip = LOCTEXT("Arg2Tooltip", "Specifies argument 2."),
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
							.ToolTip = LOCTEXT("Arg3Tooltip", "Specifies argument 3."),
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
							.ToolTip = LOCTEXT("Arg4Tooltip", "Specifies argument 4."),
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
							.ToolTip = LOCTEXT("Arg5Tooltip", "Specifies argument 5."),
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
							.ToolTip = LOCTEXT("Arg6Tooltip", "Specifies argument 6."),
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
							.ToolTip = LOCTEXT("Arg7Tooltip", "Specifies argument 7."),
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
							.ToolTip = LOCTEXT("Arg8Tooltip", "Specifies argument 8."),
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
							.ToolTip = LOCTEXT("Arg9Tooltip", "Specifies argument 9."),
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
							.ToolTip = LOCTEXT("Arg10Tooltip", "Specifies argument 10."),
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
							.ToolTip = LOCTEXT("FadeLayerNameTooltip", "Specifies the layer name for the fade."),
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
							.ToolTip = LOCTEXT("FadeFadeTypeTooltip", "Specifies fade in or fade out."),
							.TemplateWidget = FadeTypeInputTemplate
						}
					},
					{
						.ArgName = TEXT("FadeDuration"),
						.DefaultValue = TEXT("0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("FadeDurationKey", "FadeDuration"),
							.ToolTip = LOCTEXT("FadeDurationTooltip", "Specifies the fade duration in seconds."),
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
							.ToolTip = LOCTEXT("FadeFunctionTooltip", "Specifies how the fade works."),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("TargetColor"),
						.DefaultValue = TEXT("R=0.0 G=0.0 B=0.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TargetColorKey", "TargetColor"),
							.ToolTip = LOCTEXT("FadeTargetColorTooltip", "Specifies the target color for the fade."),
							.TemplateWidget = ColorInputTemplate
						}
					},
					{
						.ArgName = TEXT("Steps"),
						.DefaultValue = TEXT("2"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StepsKey", "Steps"),
							.ToolTip = LOCTEXT("StepsTooltip", "Specifies the number of steps, used by the Step fade function."),
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
							.ToolTip = LOCTEXT("BlendExpTooltip", "Specifies the exponent used by certain blend functions\r\nto control the shape of the curve."),
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
							.ToolTip = LOCTEXT("WaitForFadeCompletionTooltip", "Specifies whether to wait for fade completion before proceeding to the next command."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("ZOrder"),
						.DefaultValue = TEXT("250"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ZOrderKey", "ZOrder"),
							.ToolTip = LOCTEXT("FadeZOrderTooltip", "Specifies the ZOrder for the fade."),
							.TemplateWidget = IntegerInputTemplate
						}
					},
					{
						.ArgName = TEXT("UseGlobalFade"),
						.DefaultValue = TEXT("false"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("UseGlobalFadeKey", "UseGlobalFade"),
							.ToolTip = LOCTEXT("UseGlobalFadeTooltip", "Specifies whether to use global fade.\r\nGlobal fade can be applied across scenarios."),
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
							.ToolTip = LOCTEXT("ChangeVisibilityNameTooltip", "Specifies the widget name to modify."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Visibility"),
						.DefaultValue = TEXT("Visible"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VisibilityKey", "Visibility"),
							.ToolTip = LOCTEXT("ChangeVisibilityVisibilityTooltip", "Specifies the post-change Visibility."),
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
							.ToolTip = LOCTEXT("MoveCanvasPanelSlotSlotNameTooltip", "Specifies the slot name."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("EasingFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("EasingFunctionKey", "EasingFunction"),
							.ToolTip = LOCTEXT("EasingFunctionTooltip", "Specifies the easing function."),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("Duration"),
						.DefaultValue = TEXT("1.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DurationKey", "Duration"),
							.ToolTip = LOCTEXT("MoveDurationTooltip", "Specifies the movement duration in seconds."),
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
							.ToolTip = LOCTEXT("ChangeTypeTooltip", "Specifies whether to use absolute coordinates or relative coordinates from current position."),
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
							.ToolTip = LOCTEXT("MoveCanvasPanelSlotOverwritePositionTooltip", "Specifies the position to override."),
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
							.ToolTip = LOCTEXT("MoveCanvasPanelSlotOverwriteSizeTooltip", "Specifies the size to override."),
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
							.ToolTip = LOCTEXT("MoveCanvasPanelSlotOverwriteZOrderTooltip", "Specifies the ZOrder to override."),
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
							.ToolTip = LOCTEXT("StepsTooltip", "Specifies the number of steps, used by the Step fade function."),
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
							.ToolTip = LOCTEXT("BlendExpTooltip", "Specifies the exponent used by certain blend functions\r\nto control the shape of the curve."),
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
							.ToolTip = LOCTEXT("WaitForAnimationCompletionTooltip", "Specifies whether to wait for animation completion before proceeding to the next command."),
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
							.ToolTip = LOCTEXT("AutoSaveSlotNameTooltip", "Specifies the save slot."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("OverwriteThumbnail"),
						.DefaultValue = TEXT("{EMPTY}"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("OverwriteThumbnailKey", "OverwriteThumbnail"),
							.ToolTip = LOCTEXT("AutoSaveOverwriteThumbnailTooltip", "Specifies image as thumbnail.\r\nIf not specified, uses a screenshot as thumbnail."),
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
							.ToolTip = LOCTEXT("AssignVariableKindTooltip", "Specifies the variable type to assign the value to."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("DestinationVariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DestinationVariableNameKey", "DestinationVariableName"),
							.ToolTip = LOCTEXT("AssignVariableNameTooltip", "Specifies the variable name to assign the value to."),
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
							.ToolTip = LOCTEXT("RandomMinTooltip", "Specifies the minimum value for the random number."),
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
							.ToolTip = LOCTEXT("RandomMaxTooltip", "Specifies the maximum value for the random number."),
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
							.ToolTip = LOCTEXT("WaitForClickCanSkipTooltip", "Specifies whether this command can be skipped with the skip button."),
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
							.ToolTip = LOCTEXT("ConditionVariableKindTooltip", "Specifies the variable type to use for the condition."),
							.TemplateWidget = VariableKindInputTemplate
						}
					},
					{
						.ArgName = TEXT("VariableName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("VariableNameKey", "VariableName"),
							.ToolTip = LOCTEXT("ConditionVariableNameTooltip", "Specifies the variable name to use for the condition."),
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
							.ToolTip = LOCTEXT("ConditionOperatorTooltip", "Specifies the condition operator."),
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
							.ToolTip = LOCTEXT("ConditionRightHandValueTooltip", "Specifies the right-hand value for the condition."),
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
							.ToolTip = LOCTEXT("ForceFeedbackFeedbackEffectTooltip", "Specifies the feedback effect to play."),
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
							.ToolTip = LOCTEXT("PlayMediaMediaSourceTooltip", "Specifies the media source to play."),
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
							.ToolTip = LOCTEXT("PlayMediaCanOpenPauseMenuTooltip", "Specifies whether the pause menu can be opened during playback."),
							.TemplateWidget = BooleanInputTemplate
						}
					},
					{
						.ArgName = TEXT("MediaZOrder"),
						.DefaultValue = TEXT("100"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("MediaZOrderKey", "MediaZOrder"),
							.ToolTip = LOCTEXT("PlayMediaMediaZOrderTooltip", "Specifies the media's ZOrder."),
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
							.ToolTip = LOCTEXT("ChangeMaterialParameterTargetTooltip", "Specifies Image or Retainer Box."),
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
							.ToolTip = LOCTEXT("ChangeMaterialParameterTargetNameTooltip", "Specifies the name of the Image or Retainer Box."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("ParameterName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ParameterNameKey", "ParameterName"),
							.ToolTip = LOCTEXT("ChangeMaterialParameterParameterNameTooltip", "Specifies the parameter name contained in the material."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("EasingFunction"),
						.DefaultValue = TEXT("Linear"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("EasingFunctionKey", "EasingFunction"),
							.ToolTip = LOCTEXT("EasingFunctionTooltip", "Specifies the easing function."),
							.TemplateWidget = EasingFuncInputTemplate
						}
					},
					{
						.ArgName = TEXT("Duration"),
						.DefaultValue = TEXT("1.0"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("DurationKey", "Duration"),
							.ToolTip = LOCTEXT("MoveDurationTooltip", "Specifies the movement duration in seconds."),
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
							.ToolTip = LOCTEXT("ChangeTypeTooltip", "Specifies whether to use absolute coordinates or relative coordinates from current position."),
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
							.ToolTip = LOCTEXT("ChangeMaterialScalarParameterEndValueTooltip", "Specifies the target value after change."),
							.TemplateWidget = FloatInputTemplate
						}
					},
					{
						.ArgName = TEXT("Steps"),
						.DefaultValue = TEXT("2"),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("StepsKey", "Steps"),
							.ToolTip = LOCTEXT("StepsTooltip", "Specifies the number of steps, used by the Step fade function."),
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
							.ToolTip = LOCTEXT("BlendExpTooltip", "Specifies the exponent used by certain blend functions\r\nto control the shape of the curve."),
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
							.ToolTip = LOCTEXT("WaitForAnimationCompletionTooltip", "Specifies whether to wait for animation completion before proceeding to the next command."),
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
							.ToolTip = LOCTEXT("ChangeMaterialParameterTargetTooltip", "Specifies Image or Retainer Box."),
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
							.ToolTip = LOCTEXT("ChangeMaterialParameterTargetNameTooltip", "Specifies the name of the Image or Retainer Box."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("ParameterName"),
						.DefaultValue = TEXT(""),
						.bIsAssetToBeLoaded = false,
						.EditorSettings = {
							.DisplayName = LOCTEXT("ParameterNameKey", "ParameterName"),
							.ToolTip = LOCTEXT("ChangeMaterialParameterParameterNameTooltip", "Specifies the parameter name contained in the material."),
							.TemplateWidget = TextInputTemplate
						}
					},
					{
						.ArgName = TEXT("Texture"),
						.DefaultValue = TEXT("None"),
						.bIsAssetToBeLoaded = true,
						.EditorSettings = {
							.DisplayName = LOCTEXT("TextureKey", "Texture"),
							.ToolTip = LOCTEXT("ChangeMaterialTextureParameterTextureTooltip", "Specifies the texture after change."),
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
							.ToolTip = LOCTEXT("MacroNameToExecuteTooltip", "Specifies the macro name to execute."),
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
							.ToolTip = LOCTEXT("RunMacroInParallelNewProcessNameTooltip", "Specifies the new process name."),
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
							.ToolTip = LOCTEXT("MacroNameToExecuteTooltip", "Specifies the macro name to execute."),
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
							.ToolTip = LOCTEXT("StopParallelProcessProcessNameTooltip", "Specifies the process name to stop."),
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
							.ToolTip = LOCTEXT("StopParallelProcessReasonTooltip", "Specifies the reason for stopping."),
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
							.ToolTip = LOCTEXT("StopParallelProcessStopTimingTooltip", "Specifies when to stop."),
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
							.ToolTip = LOCTEXT("StopParallelProcessWaitForCompletionTooltip", "Specifies whether to wait for target process completion\r\nbefore proceeding to the next command."),
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
							.ToolTip = LOCTEXT("PrintContentTooltip", "Specifies the content to output."),
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
