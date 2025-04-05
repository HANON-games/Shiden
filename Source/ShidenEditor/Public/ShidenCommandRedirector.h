// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShidenCommandRedirector.generated.h"

USTRUCT(BlueprintType)
struct SHIDENEDITOR_API FShidenCommandRedirector
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Command Redirect")
    FString TargetCommandName;

    UPROPERTY(EditAnywhere, Category = "Command Redirect")
    FString NewCommandName;
    
    UPROPERTY(EditAnywhere, Category = "Command Redirect")
    TMap<FString, FString> ArgumentNameMapping;
};

UCLASS(BlueprintType)
class SHIDENEDITOR_API UShidenCommandRedirectors : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Shiden Visual Novel|Command Redirector")
    TArray<FShidenCommandRedirector> Redirectors;
    
    static TArray<FShidenCommandRedirector> GetBuiltIn();
};
