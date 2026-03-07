// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/ShidenCommand.h"
#include "System/ShidenStructuredLog.h"

TOptional<FString> FShidenCommand::GetOptionalArg(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return *Value;
	}
	return TOptional<FString>();
}

FString FShidenCommand::GetArg(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return *Value;
	}
	SHIDEN_WARNING("Argument {key} not found. Please use GetOptionalArg if the argument is optional.", *Key);
	return TEXT("");
}

TOptional<int32> FShidenCommand::GetOptionalArgAsInt(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return FCString::Atoi(**Value);
	}
	return TOptional<int32>();
}

int32 FShidenCommand::GetArgAsInt(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return FCString::Atoi(**Value);
	}
	SHIDEN_WARNING("Argument {key} not found. Please use GetOptionalArgAsInt if the argument is optional.", *Key);
	return 0;
}

TOptional<float> FShidenCommand::GetOptionalArgAsFloat(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return FCString::Atof(**Value);
	}
	return TOptional<float>();
}

float FShidenCommand::GetArgAsFloat(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return FCString::Atof(**Value);
	}
	SHIDEN_WARNING("Argument {key} not found. Please use GetOptionalArgAsFloat if the argument is optional.", *Key);
	return 0.f;
}

TOptional<bool> FShidenCommand::GetOptionalArgAsBool(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return Value->Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
	}
	return TOptional<bool>();
}

bool FShidenCommand::GetArgAsBool(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return Value->Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
	}
	SHIDEN_WARNING("Argument {key} not found. Please use GetOptionalArgAsBool if the argument is optional.", *Key);
	return false;
}

TOptional<FVector2D> FShidenCommand::GetOptionalArgAsVector2D(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		FVector2D Result;
		Result.InitFromString(*Value);
		return Result;
	}
	return TOptional<FVector2D>();
}

FVector2D FShidenCommand::GetArgAsVector2D(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		FVector2D Result;
		Result.InitFromString(*Value);
		return Result;
	}
	SHIDEN_WARNING("Argument {key} not found. Please use GetOptionalArgAsVector2D if the argument is optional.", *Key);
	return FVector2D();
}

TOptional<FVector> FShidenCommand::GetOptionalArgAsVector3D(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		FVector Result;
		Result.InitFromString(*Value);
		return Result;
	}
	return TOptional<FVector>();
}

FVector FShidenCommand::GetArgAsVector3D(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		FVector Result;
		Result.InitFromString(*Value);
		return Result;
	}
	SHIDEN_WARNING("Argument {key} not found. Please use GetOptionalArgAsVector3D if the argument is optional.", *Key);
	return FVector();
}