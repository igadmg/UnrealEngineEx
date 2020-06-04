#pragma once

#include "DrawDebugHelpers.h"
#include "PhysicsEngine/ShapeElem.h"



MATHEX_API void DrawDebugShape(const UWorld* InWorld, const FTransform& Transfom, const FKShapeElem& Shape, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0);
