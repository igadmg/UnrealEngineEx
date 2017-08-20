#pragma once

#include "Core.h"
#include "Engine.h"
#include "ModuleManager.h"



class FMathExModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};



#include "TransformEx.h"



template <>
MATHEX_API FRotator FMath::Clamp<FRotator>(const FRotator X, const FRotator Min, const FRotator Max);

template<>
FORCEINLINE bool FMath::IsWithinInclusive<FVector4>(const FVector4& TestValue, const FVector4& MinValue, const FVector4& MaxValue)
{
	return FMath::IsWithinInclusive(TestValue.X, MinValue.X, MaxValue.X)
		&& FMath::IsWithinInclusive(TestValue.Y, MinValue.Y, MaxValue.Y)
		&& FMath::IsWithinInclusive(TestValue.Z, MinValue.Z, MaxValue.Z)
		&& FMath::IsWithinInclusive(TestValue.W, MinValue.W, MaxValue.W);
}

struct FMathEx
{
	static MATHEX_API FVector FindCentroid(const TArray<FVector>& Values);
	static MATHEX_API float RandomSameDigitsNumber(float Value);

	static MATHEX_API float ProjectPointOnLineSegment(const FVector& LineStart, const FVector& LineEnd, const FVector& Point);
	static MATHEX_API FVector ClosestPointOnLineSegment(const FVector& LineStart, const FVector& LineEnd, const FVector& Point);
};