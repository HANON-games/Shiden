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

	FShidenPluginVersion(const int32 InMajor, const int32 InMinor, const int32 InPatch)
		: Major(InMajor), Minor(InMinor), Patch(InPatch)
	{
	}

	bool operator==(const FShidenPluginVersion& That) const noexcept
	{
		return Major == That.Major
			&& Minor == That.Minor
			&& Patch == That.Patch;
	}

	bool operator!=(const FShidenPluginVersion& That) const noexcept
	{
		return !(*this == That);
	}

	bool operator<(const FShidenPluginVersion& That) const noexcept
	{
		return Major < That.Major
			|| (Major == That.Major && Minor < That.Minor)
			|| (Major == That.Major && Minor == That.Minor && Patch < That.Patch);
	}

	bool operator>(const FShidenPluginVersion& That) const noexcept
	{
		return Major > That.Major
			|| (Major == That.Major && Minor > That.Minor)
			|| (Major == That.Major && Minor == That.Minor && Patch > That.Patch);
	}
};
