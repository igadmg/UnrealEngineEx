#include "DrawDebugShape.h"

#include "KShapeElemEx.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/ConvexElem.h"
#include "PhysicsEngine/SphereElem.h"
#include "PhysicsEngine/SphylElem.h"
#include "PhysicsEngine/TaperedCapsuleElem.h"



void DrawDebugShape(const UWorld* InWorld, const FTransform& Transform, const FKShapeElem& Shape, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
	switch (FKShapeElemEx::GetShapeType(Shape))
	{
		case EAggCollisionShape::Sphere:
		{
			const FKSphereElem& Sphere = static_cast<const FKSphereElem&>(Shape);
			DrawDebugSphere(InWorld, Transform.TransformPosition(Sphere.Center), Sphere.Radius, 16.f, Color, bPersistentLines, LifeTime, DepthPriority);
		}
		case EAggCollisionShape::Box:
		{
			const FKBoxElem& Box = static_cast<const FKBoxElem&>(Shape);
			DrawDebugBox(InWorld, Transform.TransformPosition(Box.Center), FVector(Box.X, Box.Y, Box.Z), Transform.GetRotation() * Box.Rotation.Quaternion(), Color, bPersistentLines, LifeTime, DepthPriority);
		}
		case EAggCollisionShape::Sphyl:
		{
			const FKSphylElem& Sphyl = static_cast<const FKSphylElem&>(Shape);
			DrawDebugCapsule(InWorld, Transform.TransformPosition(Sphyl.Center), Sphyl.Length / 2.f + Sphyl.Radius, Sphyl.Radius, Transform.GetRotation() * Sphyl.Rotation.Quaternion(), Color, bPersistentLines, LifeTime, DepthPriority);
		}
		case EAggCollisionShape::Convex:
		{
			const FKConvexElem& Convex = static_cast<const FKConvexElem&>(Shape);
		}
		case EAggCollisionShape::TaperedCapsule:
		{
			const FKTaperedCapsuleElem& TaperedCapsule = static_cast<const FKTaperedCapsuleElem&>(Shape);
		}
	}
}
