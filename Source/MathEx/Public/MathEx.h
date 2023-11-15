#pragma once

#include "Core.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FMathExModule : public IModuleInterface
{
public:
	static inline FMathExModule& Get() { return FModuleManager::LoadModuleChecked<FMathExModule>("MathEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("MathEx"); }

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
	static bool InRange(float Value, float Min, float Max) { return Value < Max && Value > Min; }

	static FRotator ReverseRotator(const FRotator& Rotator)
	{
		return FRotationMatrix::MakeFromXZ(
			-FRotationMatrix(Rotator).GetUnitAxis(EAxis::X)
			, FRotationMatrix(Rotator).GetUnitAxis(EAxis::Z)
		).Rotator();
	}

	static MATHEX_API FVector FindCentroid(const TArray<FVector>& Values);
	static MATHEX_API float RandomSameDigitsNumber(float Value);

	template <typename T>
	static const T& RandomArrayElement(const TArray<T>& Array)
	{
		verify(Array.Num() > 0);
		return Array[FMath::RandRange(0, Array.Num() - 1)];
	}

	template <typename T>
	static T& RandomArrayElement(TArray<T>& Array)
	{
		verify(Array.Num() > 0);
		return Array[FMath::RandRange(0, Array.Num() - 1)];
	}

	static MATHEX_API float ProjectPointOnLineSegment(const FVector& LineStart, const FVector& LineEnd, const FVector& Point);
	static MATHEX_API FVector ClosestPointOnLineSegment(const FVector& LineStart, const FVector& LineEnd, const FVector& Point);

	static FQuat DeltaRotation(const FQuat& A, const FQuat& B)
	{
		return B * A.Inverse();
	}

	static FORCEINLINE FVector2D Round(const FVector2D &V)
	{
		return FVector2D(FMath::RoundToFloat(V.X), FMath::RoundToFloat(V.Y));
	}
};