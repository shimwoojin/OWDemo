#pragma once

#include "CoreMinimal.h"
#include "OWDemoSetting.h"
#include <type_traits>
#include "Logger.h"

#define CreateDefaultSubobjectAuto(Component)\
(Component = CreateDefaultSubobject<std::remove_reference<decltype(*Component)>::type>(#Component))

#define CreateDefaultSubobjectArray(Array, Index)\
(Array[Index] = CreateDefaultSubobject<std::remove_reference<decltype(*Array[Index])>::type>(FName(#Array"[" + FString::FromInt(Index) + "]")))

#define ENABLE_DRAW_DEBUG 0

#if ENABLE_DRAW_DEBUG

#define DRAW_DEBUG_ONEFRAME EDrawDebugTrace::ForOneFrame

#else

#define DRAW_DEBUG_ONEFRAME EDrawDebugTrace::None

#endif