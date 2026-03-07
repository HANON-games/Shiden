// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenChangeMaterialTextureParameterCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"

UShidenChangeMaterialTextureParameterCommand::UShidenChangeMaterialTextureParameterCommand()
{
	if (const TObjectPtr<UTexture2D> NewTex = UTexture2D::CreateTransient(1, 1, PF_B8G8R8A8))
	{
#if WITH_EDITORONLY_DATA
		NewTex->MipGenSettings = TMGS_NoMipmaps;
#endif
		NewTex->CompressionSettings = TC_Default;
		NewTex->SRGB = true;
		NewTex->NeverStream = false;
		NewTex->VirtualTextureStreaming = false;

		if (FTexturePlatformData* PlatformData = NewTex->GetPlatformData())
		{
			if (PlatformData->Mips.Num() > 0)
			{
				FTexture2DMipMap& Mip = PlatformData->Mips[0];
				if (void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE))
				{
					// Texture size is 1x1, so just zero out the single pixel
					FMemory::Memzero(Data, sizeof(FColor));
				}
				Mip.BulkData.Unlock();
				NewTex->UpdateResource();
			}
		}

		ClearTexture = NewTex;
	}
}

void UShidenChangeMaterialTextureParameterCommand::ParseFromCommand(const FShidenCommand& Command, FChangeTextureParameterCommandArgs& Args)
{
	Args.Target = Command.GetArg(TEXT("Target"));
	Args.TargetName = Command.GetArg(TEXT("TargetName"));
	Args.ParameterName = Command.GetArg(TEXT("ParameterName"));
	Args.TexturePath = Command.GetArg(TEXT("Texture"));
}

void UShidenChangeMaterialTextureParameterCommand::RestoreFromSaveData_Implementation(
	const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
	UShidenWidget* ShidenWidget,
	const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
	FString& ErrorMessage)
{
	for (const TPair<FString, FShidenScenarioProperty>& Pair : ScenarioProperties)
	{
		const auto [TargetType, TargetName, ParameterName] = ShidenMaterialParameterHelpers::ParseScenarioPropertyKey(Pair.Key);

		Args = FChangeTextureParameterCommandArgs
		{
			.Target = TargetType,
			.TargetName = TargetName,
			.ParameterName = ParameterName,
			.TexturePath = Pair.Value.GetValueAsString()
		};

		UTexture* Texture;
		if (!TryGetOrLoadTexture(Texture, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (!TryChangeTextureParameter(Args, ShidenWidget, Texture, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenChangeMaterialTextureParameterCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                                 UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                 const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                                 FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	UTexture* Texture;
	if (!TryGetOrLoadTexture(Texture, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	if (!TryChangeTextureParameter(Args, ShidenWidget, Texture, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	const FString Key = ShidenMaterialParameterHelpers::MakeScenarioPropertyKey(Args.Target, Args.TargetName, Args.ParameterName);
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Key, Args.TexturePath);
	Status = EShidenProcessStatus::Next;
}

void UShidenChangeMaterialTextureParameterCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                                 const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                                 bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	UTexture* Texture;
	if (!TryGetOrLoadTexture(Texture, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryChangeTextureParameter(Args, ShidenWidget, Texture, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenChangeMaterialTextureParameterCommand::TryGetOrLoadTexture(UTexture*& Texture, FString& ErrorMessage) const
{
	if (Args.TexturePath.IsEmpty() || Args.TexturePath == TEXT("None"))
	{
		Texture = ClearTexture;
		return true;
	}

	UObject* Object = nullptr;
	if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(Args.TexturePath, Object) || !Object)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Failed to load texture asset '%s'."), *Args.TexturePath);
		return false;
	}

	Texture = Cast<UTexture>(Object);
	if (!Texture)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Asset '%s' is not a texture."), *Args.TexturePath);
		return false;
	}

	return true;
}

bool UShidenChangeMaterialTextureParameterCommand::TryChangeTextureParameter(const FChangeTextureParameterCommandArgs& Args, const UShidenWidget* ShidenWidget,
                                                                             UTexture* Texture, FString& ErrorMessage)
{
	if (!Texture)
	{
		ErrorMessage = TEXT("Failed to change texture parameter. Texture is null.");
		return false;
	}

	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	if (Args.Target == TEXT("Image"))
	{
		UImage* Image = nullptr;
		if (!ShidenWidget->TryFindImage(Args.TargetName, Image) || !Image)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Image target '%s' is not found."), *Args.TargetName);
			return false;
		}
		DynamicMaterial = Image->GetDynamicMaterial();
	}
	else if (Args.Target == TEXT("RetainerBox"))
	{
		URetainerBox* RetainerBox = nullptr;
		if (!ShidenWidget->TryFindRetainerBox(Args.TargetName, RetainerBox) || !RetainerBox)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. RetainerBox target '%s' is not found."), *Args.TargetName);
			return false;
		}
		const TObjectPtr<UObject> Resource = RetainerBox->GetEffectMaterial();
		DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource);
	}
	else
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Target type '%s' is not supported."), *Args.Target);
		return false;
	}

	if (!DynamicMaterial)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to change texture parameter. Target '%s' does not have a dynamic material."), *Args.TargetName);
		return false;
	}

	DynamicMaterial->SetTextureParameterValue(FName(Args.ParameterName), Texture);
	return true;
}
