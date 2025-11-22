// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenPluginVersion.generated.h"

USTRUCT(BlueprintType)
struct SHIDENEDITOR_API FShidenPluginVersion
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Default")
	int32 Major = 0;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	int32 Minor = 0;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	int32 Patch = 0;

	FShidenPluginVersion() = default;

	FShidenPluginVersion(const int32 InMajor, const int32 InMinor, const int32 InPatch);

	bool operator==(const FShidenPluginVersion& That) const noexcept;

	bool operator!=(const FShidenPluginVersion& That) const noexcept;

	bool operator<(const FShidenPluginVersion& That) const noexcept;

	bool operator>(const FShidenPluginVersion& That) const noexcept;

	FString ToString() const noexcept;
};
