#include "ComponentExPrivatePCH.h"
#include "CompositeColliderComponent.h"

#include "Components/ShapeComponent.h"



UCompositeColliderComponent::UCompositeColliderComponent()
{
	bWantsInitializeComponent = true;
}

void UCompositeColliderComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitializeCollider();
}

void UCompositeColliderComponent::InitializeCollider()
{
	for (UShapeComponent* Collider : Colliders)
	{
		if (!IsValid(Collider))
			continue;

		Collider->OnComponentBeginOverlap.AddDynamic(this, &UCompositeColliderComponent::OnOverlapBegin);
		Collider->OnComponentEndOverlap.AddDynamic(this, &UCompositeColliderComponent::OnOverlapEnd);
	}
}

void UCompositeColliderComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || (OtherActor == (AActor*)GetOuter()) || !IsValid(OtherComp))
		return;

	int32 ComponentIndex;
	if (Colliders.Find(Cast<UShapeComponent>(OverlappedComponent), ComponentIndex))
	{
		CollisionFlags |= 0x01 << ComponentIndex;

		if (CollisionFlags == (0x01 << Colliders.Num()) - 1)
		{
			OnComponentHit.Broadcast(OverlappedComponent, OtherActor, OtherComp, FVector::ZeroVector, SweepResult);
		}
	}
}

void UCompositeColliderComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || (OtherActor == (AActor*)GetOuter()) || !IsValid(OtherComp))
		return;

	int32 ComponentIndex;
	if (Colliders.Find(Cast<UShapeComponent>(OverlappedComponent), ComponentIndex))
	{
		CollisionFlags &= ~(0x01 << ComponentIndex);
	}
}
