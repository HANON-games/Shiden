// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/ShidenCommand.h"

FString FShidenCommand::GetArg(const FString& Key) const
{
	return Args.FindRef(Key);
}

int32 FShidenCommand::GetArgAsInt(const FString& Key) const
{
	return FCString::Atoi(*Args.FindRef(Key));
}

float FShidenCommand::GetArgAsFloat(const FString& Key) const
{
	return FCString::Atof(*Args.FindRef(Key));
}

bool FShidenCommand::GetArgAsBool(const FString& Key) const
{
	return Args.FindRef(Key).Compare(TEXT("true"), ESearchCase::IgnoreCase) == 0;
}

FVector2D FShidenCommand::GetArgAsVector2D(const FString& Key) const
{
	FVector2D Result;
	Result.InitFromString(Args.FindRef(Key));
	return Result;
}

FVector FShidenCommand::GetArgAsVector(const FString& Key) const
{
	FVector Result;
	Result.InitFromString(Args.FindRef(Key));
	return Result;
}
