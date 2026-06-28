// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenChangeMaterialTextureParameterCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenChangeMaterialTextureParameterCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FChangeTextureParameterCommandArgs
	{
		FString Target;
		FString TargetName;
		FString ParameterName;
		FString TexturePath;
	};

	UShidenChangeMaterialTextureParameterCommand();

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage) override;

	// Restore after brush-setting commands (e.g. Image), since this operates on a dynamic material
	// created from the target's brush. See GetRestoreFromSaveDataPriority in the base class.
	virtual int32 GetRestoreFromSaveDataPriority() const override { return 100; }

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FChangeTextureParameterCommandArgs& Args);

	bool TryGetOrLoadTexture(UTexture*& Texture, FString& ErrorMessage) const;

	static bool TryChangeTextureParameter(const FChangeTextureParameterCommandArgs& Args, const UShidenWidget* ShidenWidget, UTexture* Texture,
	                                      FString& ErrorMessage);

	UPROPERTY()
	TObjectPtr<UTexture> ClearTexture;

	FChangeTextureParameterCommandArgs Args;
};
