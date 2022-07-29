#pragma once

#include "SplineBuilder.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UNREALENGINEEX_API USplineBuilder : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceThreshold = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FTransform> Points;


public:
	UFUNCTION(BlueprintCallable)
	bool AddPoint(FTransform Point);

	UFUNCTION(BlueprintCallable)
	bool BuildSpline(class USplineComponent* SplineComponent);


public:
	USplineBuilder(const FObjectInitializer& ObjectInitializer);
};

