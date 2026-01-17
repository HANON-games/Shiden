// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

SHIDENCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogShiden, Log, All);

#define SHIDEN_LOG(Format, ...) UE_LOGFMT(LogShiden, Log, "[{FUNCTION_NAME}] " TEXT(Format), __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
#define SHIDEN_WARNING(Format, ...) UE_LOGFMT(LogShiden, Warning, "[{FUNCTION_NAME}] " TEXT(Format), __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
#define SHIDEN_ERROR(Format, ...) UE_LOGFMT(LogShiden, Error, "[{FUNCTION_NAME}] " TEXT(Format), __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
#define SHIDEN_DISPLAY(Format, ...) UE_LOGFMT(LogShiden, Display, "[{FUNCTION_NAME}] " TEXT(Format), __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
#define SHIDEN_VERBOSE(Format, ...) UE_LOGFMT(LogShiden, Verbose, "[{FUNCTION_NAME}] " TEXT(Format), __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
#define SHIDEN_VERY_VERBOSE(Format, ...) UE_LOGFMT(LogShiden, VeryVerbose, "[{FUNCTION_NAME}] " TEXT(Format), __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
