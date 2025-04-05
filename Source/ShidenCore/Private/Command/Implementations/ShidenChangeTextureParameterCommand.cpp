// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenChangeTextureParameterCommand.h"
#include "Engine/AssetManager.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

void UShidenChangeTextureParameterCommand::ParseFromCommand(const FShidenCommand& Command, FChangeTextureParameterCommandArgs& Args)
{
	Args.Target = Command.GetArg("Target");
	Args.TargetName = Command.GetArg("TargetName");
	Args.ParameterName = Command.GetArg("ParameterName");
	Args.TexturePath = Command.GetArg("Texture");
}

void UShidenChangeTextureParameterCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* Widget,
                                                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                              UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                              FString& ErrorMessage)
{
	for (const TPair<FString, FString>& Pair : ScenarioProperties)
	{
		FString TargetType, TargetName, ParameterName;
		Tie(TargetType, TargetName, ParameterName) = ParseScenarioPropertyKey(Pair.Key);

		Args = FChangeTextureParameterCommandArgs
		{
			.Target = TargetType,
			.TargetName = TargetName,
			.ParameterName = ParameterName,
			.TexturePath = Pair.Value
		};

		UTexture* Texture;
		if (!TryLoadTexture(Args, Texture, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (!TryChangeTextureParameter(Args, Widget, Texture, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenChangeTextureParameterCommand::ProcessCommand_Implementation(const FString& ProcessName,
                                                                         const FShidenCommand& Command, UShidenWidget* Widget,
                                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                         const float DeltaTime,
                                                                         UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                                         FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	UTexture* Texture;
	if (!TryLoadTexture(Args, Texture, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	if (!TryChangeTextureParameter(Args, Widget, Texture, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	const FString Key = MakeScenarioPropertyKey(Args.Target, Args.TargetName, Args.ParameterName);
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Key, Args.TexturePath);
	Status = EShidenProcessStatus::Next;
}

void UShidenChangeTextureParameterCommand::PreviewCommand_Implementation(const FShidenCommand& Command,
                                                                         UShidenWidget* Widget,
                                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                         bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	UTexture* Texture;
	if (!TryLoadTexture(Args, Texture, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryChangeTextureParameter(Args, Widget, Texture, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenChangeTextureParameterCommand::TryLoadTexture(FChangeTextureParameterCommandArgs& Args, UTexture*& Texture, FString& ErrorMessage)
{
	if (Args.TexturePath.IsEmpty() || Args.TexturePath == TEXT("None"))
	{
		Texture = GetClearTexture();
		return true;
	}

	bool bSuccess;
	UObject* Object;
	UShidenBlueprintLibrary::GetOrLoadAsset(Args.TexturePath, Object, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Failed to load texture asset %s."), *Args.TexturePath);
		return false;
	}

	Texture = Cast<UTexture>(Object);
	if (!Texture)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Asset %s is not a texture."), *Args.TexturePath);
		return false;
	}

	return true;
}

bool UShidenChangeTextureParameterCommand::TryChangeTextureParameter(const FChangeTextureParameterCommandArgs& Args, const UShidenWidget* Widget,
                                                                     UTexture* Texture, FString& ErrorMessage)
{
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	if (Args.Target == TEXT("Image"))
	{
		bool bSuccess;
		UImage* Image;
		Widget->FindImage(Args.TargetName, Image, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Target %s is not found."), *Args.TargetName);
			return false;
		}
		FSlateBrush Brush = Image->GetBrush();
		const TObjectPtr<UObject> Resource = Brush.GetResourceObject();
		DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource);
		if (!DynamicMaterial)
		{
			if (UMaterialInterface* Material = Cast<UMaterialInterface>(Resource))
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(Material, nullptr);
				Brush.SetResourceObject(DynamicMaterial);
				Image->SetBrush(Brush);
			}
		}
	}
	else if (Args.Target == TEXT("RetainerBox"))
	{
		bool bSuccess;
		URetainerBox* RetainerBox;
		Widget->FindRetainerBox(Args.TargetName, RetainerBox, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Target %s is not found."), *Args.TargetName);
			return false;
		}
		const TObjectPtr<UObject> Resource = IsValid(RetainerBox) ? RetainerBox->GetEffectMaterial() : nullptr;
		DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource);
	}
	else
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Target %s is not supported."), *Args.Target);
		return false;
	}

	if (!DynamicMaterial)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Target %s is not a dynamic material."), *Args.TargetName);
		return false;
	}

	DynamicMaterial->SetTextureParameterValue(FName(Args.ParameterName), Texture);
	return true;
}

UTexture* UShidenChangeTextureParameterCommand::GetClearTexture()
{
	static UTexture* Texture;
	if (Texture)
	{
		return Texture;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	const TObjectPtr<UObject> Asset = Streamable.LoadSynchronous(FSoftObjectPath(TEXT("/Shiden/Misc/ClearTexture")), false);
	Texture = Cast<UTexture>(Asset);
	return Texture;
}

FString UShidenChangeTextureParameterCommand::MakeScenarioPropertyKey(const FString& TargetType,
                                                                      const FString& TargetName, const FString& ParameterName)
{
	return FString::Printf(TEXT("%s::%s::%s"),
	                       *TargetType.Replace(TEXT(":"), TEXT("\\:")),
	                       *TargetName.Replace(TEXT(":"), TEXT("\\:")),
	                       *ParameterName.Replace(TEXT(":"), TEXT("\\:")));
}

TTuple<FString, FString, FString> UShidenChangeTextureParameterCommand::ParseScenarioPropertyKey(const FString& Key)
{
	TArray<FString> TempArray;
	Key.ParseIntoArray(TempArray, TEXT("::"), true);
	if (TempArray.Num() != 3)
	{
		return TTuple<FString, FString, FString>(TEXT(""), TEXT(""), TEXT(""));
	}

	const FString TargetType = TempArray[0].Replace(TEXT("\\:"), TEXT(":"));
	const FString TargetName = TempArray[1].Replace(TEXT("\\:"), TEXT(":"));
	const FString ParameterName = TempArray[2].Replace(TEXT("\\:"), TEXT(":"));
	return TTuple<FString, FString, FString>(TargetType, TargetName, ParameterName);
}
