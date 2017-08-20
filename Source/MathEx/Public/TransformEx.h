#pragma once



struct FTransformEx
{
	static bool IsIdentity(const FTransform& Transform)
	{
		return Transform.GetLocation() == FVector::ZeroVector
			&& Transform.GetRotation() == FQuat::Identity
			&& Transform.GetScale3D() == FVector(1, 1, 1);
	}
};
