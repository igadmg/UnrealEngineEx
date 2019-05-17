#pragma once

#include "Engine/EngineTypes.h"
#include "PhysicsEngine/ShapeElem.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/ConvexElem.h"
#include "PhysicsEngine/SphereElem.h"
#include "PhysicsEngine/SphylElem.h"
#include "PhysicsEngine/TaperedCapsuleElem.h"

#include "IntersectionEx.h"
#include "KShapeElemEx.h"
#include "MathEx.h"


struct FLineTraceEx
{
	static FBox CalcShapeAABB(const FKShapeElem& Shape, const FTransform& Transform, float Scale)
	{
		switch (FKShapeElemEx::GetShapeType(Shape))
		{
		case EAggCollisionShape::Sphere:
			return static_cast<const FKSphereElem&>(Shape).CalcAABB(Transform, Scale);
		case EAggCollisionShape::Box:
			return static_cast<const FKBoxElem&>(Shape).CalcAABB(Transform, Scale);
		case EAggCollisionShape::Sphyl:
			return static_cast<const FKSphylElem&>(Shape).CalcAABB(Transform, Scale);
		case EAggCollisionShape::Convex:
			return static_cast<const FKConvexElem&>(Shape).CalcAABB(Transform, FVector(Scale));
		case EAggCollisionShape::TaperedCapsule:
			return static_cast<const FKTaperedCapsuleElem&>(Shape).CalcAABB(Transform, Scale);
		}

		return FBox(ForceInit);
	}

	static bool LineTrace(const FKSphereElem& Sphere, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
	{
		FVector Dir = (TraceEnd - TraceStart);
		float Length = Dir.Size();
		Dir /= Length;

		if (!FIntersectionEx::LineSphereIntersection(TraceStart, Dir, Length, Sphere.Center, Sphere.Radius, OutHitResult.Time))
			return false;

		OutHitResult.bBlockingHit = true;
		OutHitResult.ImpactPoint = TraceStart + Dir * OutHitResult.Time * Length;

		return true;
	}

	static bool LineTrace(const FKBoxElem& Box, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
	{
		FTransform BoxTransform(Box.Rotation, Box.Center);
		FBox B(FVector(-Box.X, -Box.Y, -Box.Z), FVector(Box.X, Box.Y, Box.Z));

		FVector Start = BoxTransform.InverseTransformPosition(TraceStart);
		FVector End = BoxTransform.InverseTransformPosition(TraceEnd);

		if (!FIntersectionEx::LineBoxIntersection(B, Start, End, OutHitResult.Time))
			return false;

		OutHitResult.bBlockingHit = true;
		OutHitResult.ImpactPoint = FMath::Lerp(TraceStart, TraceEnd, OutHitResult.Time);

		return false;
	}

	static bool LineTrace(const FKSphylElem& Sphyl, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
	{
		FTransform SphylTransform = Sphyl.GetTransform();
		FVector SphylAxisEnd = SphylTransform.GetScaledAxis(EAxis::Z);
		FVector SphylAxisStart = -SphylAxisEnd;

		FVector TraceP, SphylP;
		FMath::SegmentDistToSegment(TraceStart, TraceEnd, SphylAxisStart, SphylAxisEnd, TraceP, SphylP);

		float R2 = Sphyl.Radius * Sphyl.Radius;
		FVector Distance = (TraceP - SphylP);
		if (Distance.SizeSquared() > R2)
			return false;

		FVector Dir = (TraceEnd - TraceStart);
		float Length = Dir.Size(); Dir /= Length;

		OutHitResult.Time = 1.0f;
		float Time;
		OutHitResult.bBlockingHit = false;
		if ((OutHitResult.bBlockingHit |= FIntersectionEx::LineSphereIntersection(TraceStart, Dir, Length, SphylAxisStart, Sphyl.Radius, Time)))
			OutHitResult.Time = FMath::Min(Time, OutHitResult.Time);
		if ((OutHitResult.bBlockingHit |= FIntersectionEx::LineSphereIntersection(TraceStart, Dir, Length, SphylAxisEnd, Sphyl.Radius, Time)))
			OutHitResult.Time = FMath::Min(Time, OutHitResult.Time);

		if (!OutHitResult.bBlockingHit || FMathEx::InRange(FMathEx::ProjectPointOnLineSegment(SphylAxisStart, SphylAxisEnd, FMath::Lerp(TraceStart, TraceEnd, OutHitResult.Time)), 0.f, 1.f))
		{
			FTransform InvSphylTransform = SphylTransform.Inverse();
			FVector LineStart = InvSphylTransform.TransformPosition(TraceStart);
			FVector LineEnd = InvSphylTransform.TransformPosition(TraceEnd);

			FIntersectionEx::LineCircleIntersection(FVector2D(LineStart), FVector2D(LineEnd), Sphyl.Radius, OutHitResult.Time);
		}

		OutHitResult.ImpactPoint = FMath::Lerp(TraceStart, TraceEnd, OutHitResult.Time);

		return OutHitResult.bBlockingHit;
	}

	static bool LineTrace(const FKConvexElem& Shape, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
	{
		return false;
	}

	static bool LineTrace(const FKTaperedCapsuleElem& Shape, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
	{
		return false;
	}

	static bool LineTrace(const FKShapeElem& Shape, const FTransform& Transform, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
	{
		OutHitResult.TraceStart = TraceStart;
		OutHitResult.TraceEnd = TraceEnd;

		FBox ShapeAABB = CalcShapeAABB(Shape, Transform, 1.f);
		if (!ShapeAABB.IsValid)
			return false;

		if (!FMath::LineBoxIntersection(ShapeAABB, TraceStart, TraceEnd, (TraceEnd - TraceStart).GetSafeNormal()))
			return false;

		FVector Start = Transform.TransformPosition(TraceStart);
		FVector End = Transform.TransformPosition(TraceStart);

		switch (FKShapeElemEx::GetShapeType(Shape))
		{
		case EAggCollisionShape::Sphere:
			LineTrace(static_cast<const FKSphereElem&>(Shape), Start, End, OutHitResult);
		case EAggCollisionShape::Box:
			LineTrace(static_cast<const FKBoxElem&>(Shape), Start, End, OutHitResult);
		case EAggCollisionShape::Sphyl:
			LineTrace(static_cast<const FKSphylElem&>(Shape), Start, End, OutHitResult);
		case EAggCollisionShape::Convex:
			LineTrace(static_cast<const FKConvexElem&>(Shape), Start, End, OutHitResult);
		case EAggCollisionShape::TaperedCapsule:
			LineTrace(static_cast<const FKTaperedCapsuleElem&>(Shape), Start, End, OutHitResult);
		}

		if (OutHitResult.bBlockingHit)
		{
			OutHitResult.ImpactPoint = Transform.InverseTransformPosition(OutHitResult.ImpactPoint);
			OutHitResult.ImpactNormal = Transform.InverseTransformVector(OutHitResult.ImpactNormal);
			OutHitResult.Location = OutHitResult.ImpactPoint;
		}

		return OutHitResult.bBlockingHit;
	}
};
