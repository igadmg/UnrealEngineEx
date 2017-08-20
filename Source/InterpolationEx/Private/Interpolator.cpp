#include "InterpolationEx.h"

#include "Interpolator.h"

#include "linear_spline.h"
#include "cubic_hermite_spline.h"
#include "quintic_hermite_spline.h"



namespace
{
	template <typename T> T zero();

	template <> float zero<float>() { return 0.f; }
	template <> FVector2D zero<FVector2D>() { return FVector2D::ZeroVector; }
	template <> FVector zero<FVector>() { return FVector::ZeroVector; }
	template <> FRotator zero<FRotator>() { return FRotator::ZeroRotator; }
	template <> FVector4 zero<FVector4>() { return FVector4(0, 0, 0, 0); }
	template <> FLinearColor zero<FLinearColor>() { return FLinearColor(ForceInitToZero); }
	template <> FPostProcessSettings zero<FPostProcessSettings>() { return FPostProcessSettings(); }
}


template <typename T, typename TT>
T DoInterpolate(const TArray<T>& Start, const TArray<T>& End, EInterpolatorType CurveType, EInterpolatorDegree CurveDegree, TT Alpha)
{
	if (CurveType == EInterpolatorType::IT_None || CurveDegree == EInterpolatorDegree::ID_Linear)
	{
		if (Start.Num() > 0 && End.Num() > 0)
		{
			return ipl::linear_spline(Start[0], End[0], Alpha);
		}
		else
		{
			return zero<T>();
		}
	}
	else if (CurveType == EInterpolatorType::IT_Hetmite)
	{
		if (CurveDegree == EInterpolatorDegree::ID_Cubic)
		{
			if (Start.Num() > 1 && End.Num() > 1)
			{
				return ipl::cubic_hermite_spline(Start[0], Start[1], End[0], End[1], Alpha);
			}
			else
			{
				return zero<T>();
			}
		}
		else if (CurveDegree == EInterpolatorDegree::ID_Quintic)
		{
			if (Start.Num() > 2 && End.Num() > 2)
			{
				return ipl::quintic_hermite_spline(Start[0], Start[1], Start[2], End[0], End[1], End[2], Alpha);
			}
			else
			{
				return zero<T>();
			}
		}
	}

	return zero<T>();
}


float UInterpolatorFloat::InterpolateFloat(float A, float B, float Alpha)
{
	return ipl::linear_spline(A, B, Alpha);
}

float UInterpolatorFloat::Interpolate(float Alpha)
{
	return DoInterpolate(Start, End, CurveType, CurveDegree, Alpha);
}

FVector2D UInterpolatorVector2D::InterpolateVector2D(const FVector2D& A, const FVector2D& B, float Alpha)
{
	return ipl::linear_spline(A, B, Alpha);
}

FVector2D UInterpolatorVector2D::Interpolate(float Alpha)
{
	return DoInterpolate(Start, End, CurveType, CurveDegree, Alpha);
}

FVector UInterpolatorVector::InterpolateVector(const FVector& A, const FVector& B, float Alpha)
{
	return ipl::linear_spline(A, B, Alpha);
}

FVector UInterpolatorVector::Interpolate(float Alpha)
{
	return DoInterpolate(Start, End, CurveType, CurveDegree, Alpha);
}

FRotator UInterpolatorRotator::InterpolateRotator(const FRotator& A, const FRotator& B, float Alpha)
{
	FQuat AQuat(A);
	FQuat BQuat(B);

	FQuat Result = FQuat::Slerp(AQuat, BQuat, Alpha);
	Result.Normalize();

	return Result.Rotator();
}

FRotator UInterpolatorRotator::Interpolate(float Alpha)
{
	// if shortest path, we use Quaternion to interpolate instead of using FRotator
	if (bShortestPath)
	{
		FQuat AQuat(Start[0]);
		FQuat BQuat(End[0]);

		FQuat Result = FQuat::Slerp(AQuat, BQuat, Alpha);
		Result.Normalize();

		return Result.Rotator();
	}

	FRotator DeltaAngle = End[0] - Start[0];
	return Start[0] + Alpha*DeltaAngle;
}

FVector4 UInterpolatorVector4::InterpolateVector4(const FVector4& A, const FVector4& B, float Alpha)
{
	return ipl::linear_spline(A, B, Alpha);
}

FVector4 UInterpolatorVector4::Interpolate(float Alpha)
{
	return DoInterpolate(Start, End, CurveType, CurveDegree, Alpha);
}


FLinearColor UInterpolatorColor::Interpolate(float Alpha)
{
	return DoInterpolate(Start, End, CurveType, CurveDegree, Alpha);
}

namespace
{
	template <typename T, typename U >
	TArray<T> GatherMembers(const TArray<U>& Data, uint32 offset)
	{
		TArray<T> Result; Result.Reserve(Data.Num());

		for (int i = 0; i < Data.Num(); i++)
		{
			Result.Add(*((T*)((uint8 *)&Data[i] + offset)));
		}

		return Result;
	}
}

#define offset_off(Object, Member) (reinterpret_cast<uint8*>(&(Object.Member)) - reinterpret_cast<uint8*>(&(Object)))

#define INTERPOLATE_STRUCT_FIELD(Type, Object, Field) do {\
	uint32 offset = offset_off(Object, Field);\
	Object.Field = DoInterpolate(GatherMembers<Type>(Start, offset), GatherMembers<Type>(End, offset), CurveType, CurveDegree, Alpha);\
} while(false)

#define INTERPOLATE_POSTPROCESS_FIELD(Type, Object, Field)\
if (Start[0].bOverride_##Field || End[0].bOverride_##Field) { INTERPOLATE_STRUCT_FIELD(Type, Object, Field); Object.bOverride_##Field = true; }

FPostProcessSettings UInterpolatorPostProcessSettings::Interpolate(float Alpha)
{
	FPostProcessSettings Result = Start[0];

	if (Alpha == 0)
		return Start[0];
	if (Alpha == 1)
		return End[0];

	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldFstop);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldSensorWidth);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldFocalDistance);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldDepthBlurAmount);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldDepthBlurRadius);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldFocalRegion);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldNearTransitionRegion);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldFarTransitionRegion);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldScale);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldMaxBokehSize);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldNearBlurSize);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldFarBlurSize);

	// Result.DepthOfFieldMethod : 1;
	// uint32 bOverride_MobileHQGaussian : 1;
	// bOverride_DepthOfFieldBokehShape : 1;

	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldOcclusion);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldColorThreshold);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldSizeThreshold);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldSkyFocusDistance);
	INTERPOLATE_POSTPROCESS_FIELD(float, Result, DepthOfFieldVignetteSize);

	return Result;
}

#undef INTERPOLATE_STRUCT_FIELD
#undef INTERPOLATE_POSTPROCESS_FIELD
#undef offset_off
