#include "Logger.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(OWDemo)

void Logger::Print(int32 const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::FromInt(Data));
}

void Logger::Print(float const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::SanitizeFloat(Data));
}

void Logger::Print(FString const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Data);
}

void Logger::Print(FVector const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Data.ToString());
}

void Logger::Print(FRotator const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Data.ToString());
}

void Logger::Print(FTransform const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Data.ToString());
}

void Logger::Print(UObject* const& Data, int32 const Key, float const Duration, FColor const Color)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Data->GetFName().ToString());
}

void Logger::Log(int32 const& Data)
{
	UE_LOG(OWDemo, Log, TEXT("%i"), Data);
}

void Logger::Log(float const& Data)
{
	UE_LOG(OWDemo, Log, TEXT("%f"), Data);
}

void Logger::Log(FString const& Data)
{
	UE_LOG(OWDemo, Log, TEXT("%s"), *Data);
}

void Logger::Log(FVector const& Data)
{
	UE_LOG(OWDemo, Log, TEXT("%s"), *Data.ToString());
}

void Logger::Log(FRotator const& Data)
{
	UE_LOG(OWDemo, Log, TEXT("%s"), *Data.ToString());
}

void Logger::Log(UObject* const& Data)
{
	UE_LOG(OWDemo, Log, TEXT("%s"), *Data->GetFName().ToString());
}