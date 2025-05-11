// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "UObject/ConstructorHelpers.h"
#include "ShidenChangeTextureParameterCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenChangeTextureParameterCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FChangeTextureParameterCommandArgs
	{
		FString Target;
		FString TargetName;
		FString ParameterName;
		FString TexturePath;
	};

	UShidenChangeTextureParameterCommand() : Super()
	{
		static ConstructorHelpers::FObjectFinder<UTexture> ClearTextureFinder(TEXT("/Shiden/Misc/ClearTexture"));
		if (ClearTextureFinder.Succeeded())
		{
			ClearTexture = ClearTextureFinder.Object;
		}
	}

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
	                                                FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status,
	                                           FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FChangeTextureParameterCommandArgs& Args);

	static bool TryLoadTexture(const FChangeTextureParameterCommandArgs& Args, UTexture*& Texture, FString& ErrorMessage);

	static bool TryChangeTextureParameter(const FChangeTextureParameterCommandArgs& Args, const UShidenWidget* ShidenWidget, UTexture* Texture,
	                                      FString& ErrorMessage);

	static FString MakeScenarioPropertyKey(const FString& TargetType, const FString& TargetName, const FString& ParameterName);

	static TTuple<FString, FString, FString> ParseScenarioPropertyKey(const FString& Key);

	UPROPERTY()
	TObjectPtr<UTexture> ClearTexture;

	FChangeTextureParameterCommandArgs Args;
};
