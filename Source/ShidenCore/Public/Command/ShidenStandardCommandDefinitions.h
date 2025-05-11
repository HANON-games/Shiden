// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenCommandDefinitions.h"
#include "ShidenStandardCommandDefinitions.generated.h"

UCLASS()
class SHIDENCORE_API UShidenStandardCommandDefinitions : public UShidenCommandDefinitions
{
	GENERATED_BODY()

public:
	// Styles
	static const FShidenCommandStyle TextStyle;
	static const FShidenCommandStyle MediaStyle;
	static const FShidenCommandStyle FlowControlStyle;
	static const FShidenCommandStyle UIControlStyle;
	static const FShidenCommandStyle DataStyle;
	static const FShidenCommandStyle MacroStyle;
	static const FShidenCommandStyle CommentStyle;
	static const FShidenCommandStyle TagStyle;
	static const FShidenCommandStyle InteractionStyle;

	// Templates
	static const FSoftObjectPath TextInputTemplate;
	static const FSoftObjectPath TextTypeInputTemplate;
	static const FSoftObjectPath MultilineTextInputTemplate;
	static const FSoftObjectPath BooleanInputTemplate;
	static const FSoftObjectPath IntegerInputTemplate;
	static const FSoftObjectPath FloatInputTemplate;
	static const FSoftObjectPath Vector2InputTemplate;
	static const FSoftObjectPath ColorInputTemplate;
	static const FSoftObjectPath VariableKindInputTemplate;
	static const FSoftObjectPath VariableNameInputTemplate;
	static const FSoftObjectPath FlexInputTemplate;
	static const FSoftObjectPath ComboBoxInputTemplate;
	static const FSoftObjectPath AssetInputTemplate;
	static const FSoftObjectPath SoundInputTemplate;
	static const FSoftObjectPath SoundTypeInputTemplate;
	static const FSoftObjectPath BGMFadeFuncInputTemplate;
	static const FSoftObjectPath EasingFuncInputTemplate;
	static const FSoftObjectPath FlexCalculationOperatorInputTemplate;
	static const FSoftObjectPath FadeTypeInputTemplate;
	static const FSoftObjectPath FlexOperatorInputTemplate;
	static const FSoftObjectPath FlexNumberInputTemplate;
	static const FSoftObjectPath ScenarioInputTemplate;
	static const FSoftObjectPath SlateVisibilityInputTemplate;

	UShidenStandardCommandDefinitions();
};
