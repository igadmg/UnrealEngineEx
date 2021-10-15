#include "MathExStatics.h"

#include "MathEx.h"



bool UMathExStatics::AreVertexOrdered(FVector A, FVector B, FVector C, ETriangleVertexOrder Order)
{
	float Sum = A.X * B.Y - A.Y * B.X;
	Sum += B.X * C.Y - B.Y * C.X;
	Sum += C.X * A.Y - C.Y * A.X;

	switch (Order)
	{
	case ETriangleVertexOrder::Clockwise:
		return Sum < 0;
	case ETriangleVertexOrder::CounterClockwise:
		return Sum > 0;
	}

	return false;
}

float UMathExStatics::AngleBetweenVectors(FVector A, FVector B, FVector O, ETriangleVertexOrder Order)
{
	bool bIsSameOrder = AreVertexOrdered(A, B, O, Order);
	auto OAn = bIsSameOrder ? (A - O).GetSafeNormal() : (B - O).GetSafeNormal();
	auto OBn = bIsSameOrder ? (B - O).GetSafeNormal() : (A - O).GetSafeNormal();

	float CosA = FVector::DotProduct(OAn, OBn);
	float SinA = FVector::CrossProduct(OAn, OBn).Size();

	return FMath::Atan2(SinA, CosA);
}

float UMathExStatics::RandomSameDigitsNumber(float Value)
{
	return FMathEx::RandomSameDigitsNumber(Value);
}

FVector UMathExStatics::SphereCenterByTwoPointsAndRadiusVector(FVector A, FVector B, FVector R)
{
	// https://math.stackexchange.com/a/1781546

	float RadiusSquared = R.SizeSquared();
	float ABDistanceSquared = (B - A).SizeSquared();

	FVector C = (A + B) * 0.5f;

	if (RadiusSquared <= ABDistanceSquared)
		return C;

	FVector Rn = R.GetSafeNormal();
	FVector CAn = (A - C).GetSafeNormal();
	float b = FMath::Sqrt(RadiusSquared - (C - A).SizeSquared());

	// Renormalize R vector so it is pependicular to AB
	FVector Nn = FVector::CrossProduct(CAn, Rn).GetSafeNormal();
	Rn = FVector::CrossProduct(Nn, CAn).GetSafeNormal();

	return C + Rn * b;
}