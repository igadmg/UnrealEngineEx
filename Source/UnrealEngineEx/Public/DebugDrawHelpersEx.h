#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

class UCanvas;

 // Define that controls debug drawing
#ifndef ENABLE_DRAW_DEBUG
#define ENABLE_DRAW_DEBUG  !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#endif

#if ENABLE_DRAW_DEBUG

/* Draws a 2D Histogram of size 'DrawSize' based FDebugFloatHistory struct, using DrawTransform for the position in the world. */
UNREALENGINEEX_API void DrawDebugFloatHistoryEx(UWorld const & WorldRef, FDebugFloatHistory const & FloatHistory, FTransform const & DrawTransform, FVector2D const & DrawSize, FColor const & DrawColor, bool const & bPersistent = false, float const & LifeTime = -1.f, uint8 const & DepthPriority = 0);

#elif !defined(SHIPPING_DRAW_DEBUG_ERROR) || !SHIPPING_DRAW_DEBUG_ERROR

// Empty versions of above functions

FORCEINLINE void DrawDebugFloatHistoryEx(UWorld const & WorldRef, FDebugFloatHistory const & FloatHistory, FTransform const & DrawTransform, FVector2D const & DrawSize, FColor const & DrawColor, bool const & bPersistent = false, float const & LifeTime = -1.f, uint8 const & DepthPriority = 0) {}

#else

// Remove 'no-op' debug draw commands, users need to make sure they are not calling these functions in Shipping/Test builds or it will generate a compile error

#endif // ENABLE_DRAW_DEBUG
