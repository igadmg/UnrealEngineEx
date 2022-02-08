#pragma once

#include "Engine/Scene.h"

#include "Interpolator.generated.h"



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

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorFloat : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<float> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<float> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	float First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	float Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	float Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	static float InterpolateFloat(float A, float B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorVector2D : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector2D> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector2D> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FVector2D First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FVector2D Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	FVector2D Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	static FVector2D InterpolateVector2D(const FVector2D& A, const FVector2D& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorVector : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FVector> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FVector First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FVector Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	FVector Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	static FVector InterpolateVector(const FVector& A, const FVector& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorRotator : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "UnrealEngineEx: Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "UnrealEngineEx: Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "UnrealEngineEx: Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool bShortestPath;

	UPROPERTY(Category = "UnrealEngineEx: Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FRotator> Start;

	UPROPERTY(Category = "UnrealEngineEx: Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FRotator> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FRotator First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FRotator Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	FRotator Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	static FRotator InterpolateRotator(const FRotator& A, const FRotator& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorVector4 : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite)
	TArray<FVector4> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite)
	TArray<FVector4> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FVector4 First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FVector4 Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	FVector4 Interpolate(float Alpha);

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	static FVector4 InterpolateVector4(const FVector4& A, const FVector4& B, float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorColor : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FLinearColor> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FLinearColor> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FLinearColor First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FLinearColor Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	FLinearColor Interpolate(float Alpha);
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolatorPostProcessSettings : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorType CurveType;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	EInterpolatorDegree CurveDegree;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FPostProcessSettings> Start;

	UPROPERTY(Category = "Interpolator", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FPostProcessSettings> End;



public:
	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "First", CompactNodeTitle = "First"))
	FPostProcessSettings First() { return Start[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure, meta = (DisplayName = "Last", CompactNodeTitle = "Last"))
	FPostProcessSettings Last() { return End[0]; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolator", BlueprintPure)
	FPostProcessSettings Interpolate(float Alpha);
};
