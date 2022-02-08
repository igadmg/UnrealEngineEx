#include "MathEx.h"



#define LOCTEXT_NAMESPACE "FMathExModule"



void FMathExModule::StartupModule()
{
}

void FMathExModule::ShutdownModule()
{
}



#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FMathExModule, MathEx)

//DEFINE_LOG_CATEGORY(LogCoordinateSystemEx);


template <>
FRotator FMath::Clamp<FRotator>(const FRotator X, const FRotator Min, const FRotator Max)
{
	return FRotator(
		FMath::ClampAngle(X.Pitch, Min.Pitch, Max.Pitch),
		FMath::ClampAngle(X.Yaw, Min.Yaw, Max.Yaw),
		FMath::ClampAngle(X.Roll, Min.Roll, Max.Roll)
	);
}

FVector FMathEx::FindCentroid(const TArray<FVector>& Values)
{
	if (Values.Num() == 0)
		return FVector::ZeroVector;

	FVector Result = FVector::ZeroVector;

	for (const FVector& V : Values)
	{
		Result += V / Values.Num();
	}

	return Result;
}

float FMathEx::RandomSameDigitsNumber(float Value)
{
	int32 DecimalValue = FMath::TruncToInt(Value);

	int32 DecimalDigits = DecimalValue > 0 ? (int)FMath::LogX(10.f, (float)DecimalValue) + 1 : 1;
	int32 FractionalDigits = FMath::IsNearlyZero(Value - DecimalValue) ? 0 : 2;

	float Result = FMath::RandRange(0, (int32)FMath::Pow(10.f, (float)DecimalDigits) - 1);
	if (FractionalDigits > 0)
	{
		int32 Divider = (int32)FMath::Pow(10.f, (float)FractionalDigits);
		Result += (float)FMath::RandRange(0, Divider - 1) / Divider;
	}

	return Result;
}

float FMathEx::ProjectPointOnLineSegment(const FVector& LineStart, const FVector& LineEnd, const FVector& Point)
{
	const float A = (LineStart - Point) | (LineEnd - LineStart);
	const float B = (LineEnd - LineStart).SizeSquared();

	return -A / B;
}

FVector FMathEx::ClosestPointOnLineSegment(const FVector& LineStart, const FVector& LineEnd, const FVector& Point)
{
	float T = ProjectPointOnLineSegment(LineStart, LineEnd, Point);

	if (FMath::IsNaN(T))
		return LineStart;

	// Generate closest point
	const FVector ClosestPoint = LineStart + (T * (LineEnd - LineStart));
	return ClosestPoint;
}