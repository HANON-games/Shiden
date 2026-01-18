// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Save/ShidenBaseSaveGame.h"
#include "Variable/ShidenVariable.h"
#include "ShidenSystemSaveGame.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenSystemSaveGame : public UShidenBaseSaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenVariable SystemVariable;

	static TObjectPtr<UShidenSystemSaveGame> GetOrCreate();
	
	void Apply() const;

	void Prepare();

	bool TryCommit();

	static bool DoesExist();
	
	static bool TryDelete();

private:
	constexpr static TCHAR SystemDataSlotName[] = TEXT("ShidenSystemData");
};
