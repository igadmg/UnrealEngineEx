#include "SplineBuilder.h"

#include "Components/SplineComponent.h"

#include "UnrealEngineEx.final.h"


USplineBuilder::USplineBuilder(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool USplineBuilder::AddPoint(FTransform Point)
{
	if (Points.Num() > 0 && FVector::DistSquared(Points.Last().GetLocation(), Point.GetLocation()) < DistanceThreshold)
		return false;

	Points.Add(Point);

	return true;
}

bool USplineBuilder::BuildSpline(USplineComponent* SplineComponent)
{
	if (!IsValid(SplineComponent))
		return false;

	SplineComponent->ClearSplinePoints(false);

	for (auto Point : Points)
	{
		SplineComponent->AddSplinePoint(Point.GetLocation(), ESplineCoordinateSpace::World, false);
	}
	SplineComponent->UpdateSpline();

	return Points.Num() != 0;
}
