// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableDefinition.h"
#include "Command/ShidenCommandDefinition.h"
#include "ShidenMacroParameter.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenMacroParameterEditorSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FText ToolTip;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString VisibilityCondition;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenConditionalMessage> InformationMessages;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenConditionalMessage> WarningMessages;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FShidenConditionalMessage> ErrorMessages;
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenMacroParameter : public FShidenVariableDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool bIsEnumParameter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FString> EnumValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FShidenMacroParameterEditorSettings EditorSettings;
};
