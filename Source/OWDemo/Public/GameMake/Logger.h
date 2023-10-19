#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(OWDemo, Log, All)

#define DEFAULT_KEY      (INDEX_NONE)
#define DEFAULT_DURATION (5.0f)
#define DEFAULT_COLOR    (FColor::Cyan)

#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_S(Verbosity) UE_LOG(OWDemo, Verbosity, TEXT("%s"), *LOG_CALLINFO)
#define LOG(Verbosity, Format, ...) UE_LOG(OWDemo, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define SMOOTH_CHECK(Expr, ...) { if(!(Expr)) {LOG(Error, TEXT("ASSERTION : %s"), \
TEXT("'"#Expr"'")); return __VA_ARGS__; } }

class Logger
{
public:

	static void Print(int32    const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);
	static void Print(float    const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);
	static void Print(FString  const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);
	static void Print(FVector  const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);
	static void Print(FRotator const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);
	static void Print(FTransform const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);
	static void Print(UObject* const& Data, int32 const Key = DEFAULT_KEY, float const Duration = DEFAULT_DURATION, FColor const Color = DEFAULT_COLOR);

	/*
		Logs Something.
		
		@param Data Integer Data To Log
	*/
	static void Log(int32    const& Data);
	static void Log(float    const& Data);
	static void Log(FString  const& Data);
	static void Log(FVector  const& Data);
	static void Log(FRotator const& Data);
	static void Log(UObject* const& Data);
};