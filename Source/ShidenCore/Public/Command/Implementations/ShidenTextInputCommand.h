// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableKind.h"
#include "Variable/ShidenVariableDefinition.h"
#include "ShidenTextInputCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenTextInputCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FTextInputCommandArgs
	{
		int32 MaxLength;
		int32 MaxLineCount;
		FString AllowedCharacterRegex;
		FString DefaultText;
		FString HintText;
		EShidenVariableKind DestinationVariableKind;
		FString DestinationVariableName;
		bool bHideTextLayer;
		EShidenVariableType DestinationType;
	};

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FTextInputCommandArgs& Args, FString& ErrorMessage);

	static bool TryInitializeTextInput(const FTextInputCommandArgs& Args, bool bIsPreview, UShidenWidget* ShidenWidget, FString& ErrorMessage);

	FTextInputCommandArgs Args;
};
