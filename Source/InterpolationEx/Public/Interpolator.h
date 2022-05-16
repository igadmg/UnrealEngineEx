#pragma once

#include "UObject/Object.h"
#include "Engine/Scene.h"
#include "Kismet/KismetMathLibrary.h"
#include "InterpolationTimer.h"

#include "Interpolator.generated.h"


template<typename FloatType, TEMPLATE_REQUIRES(TIsFloatingPoint<FloatType>::Value)>
FloatType EaseAlpha(FloatType InAlpha, uint8 EasingFunc, FloatType BlendExp = 2, int32 Steps = 2)
{
	switch (EasingFunc)
	{
	case EEasingFunc::Step:					return FMath::InterpStep<FloatType>(0.f, 1.f, InAlpha, Steps);
	case EEasingFunc::SinusoidalIn:			return FMath::InterpSinIn<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::SinusoidalOut:		return FMath::InterpSinOut<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::SinusoidalInOut:		return FMath::InterpSinInOut<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::EaseIn:				return FMath::InterpEaseIn<FloatType>(0.f, 1.f, InAlpha, BlendExp);
	case EEasingFunc::EaseOut:				return FMath::InterpEaseOut<FloatType>(0.f, 1.f, InAlpha, BlendExp);
	case EEasingFunc::EaseInOut:			return FMath::InterpEaseInOut<FloatType>(0.f, 1.f, InAlpha, BlendExp);
	case EEasingFunc::ExpoIn:				return FMath::InterpExpoIn<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::ExpoOut:				return FMath::InterpExpoOut<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::ExpoInOut:			return FMath::InterpExpoInOut<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::CircularIn:			return FMath::InterpCircularIn<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::CircularOut:			return FMath::InterpCircularOut<FloatType>(0.f, 1.f, InAlpha);
	case EEasingFunc::CircularInOut:		return FMath::InterpCircularInOut<FloatType>(0.f, 1.f, InAlpha);
	}
	return InAlpha;
}

UENUM()
enum class EInterpolatorType : uint8
{
	IT_None						UMETA(DisplayName = "Linear Interpolation"),
	IT_Hetmite					UMETA(DisplayName = "Hermite Interpolation"),
};

UENUM()
enum class EInterpolatorDegree : uint8
{
	ID_Linear					UMETA(DisplayName = "Linear"),
	ID_Cubic					UMETA(DisplayName = "Cubic"),
	ID_Quintic					UMETA(DisplayName = "Quintic"),
};

template <typename T, typename TT>
T DoInterpolate(const TArray<T>& Start, const TArray<T>& End, EInterpolatorType CurveType, EInterpolatorDegree CurveDegree, TT Alpha);

namespace XX
{
	template <typename T> struct TLerp
	{
		static T Lerp(const T& A, const T& B, float V)
		{
			return A + V * (B - A);
		}
	};

	template <typename T>
	T Lerp(const T& A, const T& B, float V)
	{
		return TLerp<T>::Lerp(A, B, V);
	}
}

template <typename T>
struct TLerpInterpolator
{
	T Start;
	T End;
	EEasingFunc::Type EasingFunc = EEasingFunc::Linear;

	TLerpInterpolator()
	{
	}

	TLerpInterpolator(T Start, T End, EEasingFunc::Type EasingFunc = EEasingFunc::Linear)
		: Start(Start)
		, End(End)
		, EasingFunc(EasingFunc)
	{
	}

	T Interpolate(float Alpha) const
	{
		return XX::Lerp(Start, End, EaseAlpha(Alpha, EasingFunc));
	}
};

template <typename T>
struct TLerpInterpolationTimer
{
	TLerpInterpolator<T> Interpolator;
	FInterpolationTimer Timer;

	TLerpInterpolationTimer()
	{
	}

	TLerpInterpolationTimer(T Start, T End, float Duration, EEasingFunc::Type EasingFunc = EEasingFunc::Linear, bool bIsCyclic = false)
		: Interpolator(Start, End, EasingFunc)
		, Timer(Duration, bIsCyclic)
	{
	}

	bool IsFinished() const { return Timer.IsFinished(); }
	float Alpha() const { return Timer.Alpha(); }
	T Value() const { return Interpolator.Interpolate(Timer.Alpha()); }
	T Advance(float DeltaSeconds) { Timer.Advance(DeltaSeconds); return Value(); }
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolator : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorFloat : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<float> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<float> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	float First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	float Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	float Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	static float InterpolateFloat(float A, float B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorVector2D : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector2D> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector2D> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FVector2D First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FVector2D Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	FVector2D Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	static FVector2D InterpolateVector2D(const FVector2D& A, const FVector2D& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorVector : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FVector First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FVector Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	FVector Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	static FVector InterpolateVector(const FVector& A, const FVector& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorRotator : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "UnrealEngineEx|Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool bShortestPath;

	UPROPERTY(Category = "UnrealEngineEx|Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FRotator> Start;

	UPROPERTY(Category = "UnrealEngineEx|Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FRotator> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FRotator First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FRotator Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	FRotator Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	static FRotator InterpolateRotator(const FRotator& A, const FRotator& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorVector4 : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite)
	TArray<FVector4> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite)
	TArray<FVector4> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FVector4 First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FVector4 Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	FVector4 Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	static FVector4 InterpolateVector4(const FVector4& A, const FVector4& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorColor : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FLinearColor> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FLinearColor> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FLinearColor First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FLinearColor Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	FLinearColor Interpolate(float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorPostProcessSettings : public UInterpolator
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FPostProcessSettings> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FPostProcessSettings> End;


public:
	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FPostProcessSettings First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FPostProcessSettings Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx|Interpolator", BlueprintPure)
	FPostProcessSettings Interpolate(float Alpha);
};
