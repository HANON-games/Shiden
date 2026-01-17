// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenPluginVersion.h"

FShidenPluginVersion::FShidenPluginVersion(const int32 InMajor, const int32 InMinor, const int32 InPatch)
	: Major(InMajor), Minor(InMinor), Patch(InPatch)
{
}

bool FShidenPluginVersion::operator==(const FShidenPluginVersion& That) const noexcept
{
	return Major == That.Major
		&& Minor == That.Minor
		&& Patch == That.Patch;
}

bool FShidenPluginVersion::operator!=(const FShidenPluginVersion& That) const noexcept
{
	return !(*this == That);
}

bool FShidenPluginVersion::operator<(const FShidenPluginVersion& That) const noexcept
{
	if (Major != That.Major)
	{
		return Major < That.Major;
	}
	if (Minor != That.Minor)
	{
		return Minor < That.Minor;
	}
	return Patch < That.Patch;
}

bool FShidenPluginVersion::operator>(const FShidenPluginVersion& That) const noexcept
{
	if (Major != That.Major)
	{
		return Major > That.Major;
	}
	if (Minor != That.Minor)
	{
		return Minor > That.Minor;
	}
	return Patch > That.Patch;
}

FString FShidenPluginVersion::ToString() const noexcept
{
	return FString::Printf(TEXT("%d.%d.%d"), Major, Minor, Patch);
}
