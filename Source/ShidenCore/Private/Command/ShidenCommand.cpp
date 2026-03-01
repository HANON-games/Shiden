// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/ShidenCommand.h"

TOptional<FString> FShidenCommand::GetArg(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return *Value;
	}
	return TOptional<FString>();
}

TOptional<int32> FShidenCommand::GetArgAsInt(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return FCString::Atoi(**Value);
	}
	return TOptional<int32>();
}

TOptional<float> FShidenCommand::GetArgAsFloat(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return FCString::Atof(**Value);
	}
	return TOptional<float>();
}

TOptional<bool> FShidenCommand::GetArgAsBool(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		return Value->Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
	}
	return TOptional<bool>();
}

TOptional<FVector2D> FShidenCommand::GetArgAsVector2D(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		FVector2D Result;
		Result.InitFromString(*Value);
		return Result;
	}
	return TOptional<FVector2D>();
}

TOptional<FVector> FShidenCommand::GetArgAsVector(const FString& Key) const
{
	if (const FString* Value = Args.Find(Key))
	{
		FVector Result;
		Result.InitFromString(*Value);
		return Result;
	}
	return TOptional<FVector>();
}