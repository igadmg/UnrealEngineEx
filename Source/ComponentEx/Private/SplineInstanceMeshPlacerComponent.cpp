#include "SplineInstanceMeshPlacerComponent.h"

#include "Components/SplineComponent.h"

#include "IsValidEx.h"



USplineInstanceMeshPlacerComponent::USplineInstanceMeshPlacerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USplineInstanceMeshPlacerComponent::FillSplineMesh(float Interval, FVector InstanceOffset)
{
	ClearInstances();
	SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IsValid(SplineComponent))
	{
		float SplineLength = SplineComponent->GetSplineLength();
		float Distance = 0;

		while (Distance < SplineLength)
		{
			auto DistanceTransform = SplineComponent->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

			DistanceTransform.AddToTranslation(DistanceTransform.TransformVector(InstanceOffset));
			AddInstanceWorldSpace(DistanceTransform);

			Distance += Interval;
		}
	}
}

bool USplineInstanceMeshPlacerComponent::OnWorldSaved()
{
	SetCollisionEnabled(CollisionEnabled);

	return true;
}

void USplineInstanceMeshPlacerComponent::OnHiddenInGameChanged()
{
	Super::OnHiddenInGameChanged();

	SetCollisionEnabled(bHiddenInGame ? ECollisionEnabled::NoCollision : CollisionEnabled);
}
