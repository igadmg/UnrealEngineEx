#include "SplineMeshBuilderComponent.h"
#include "ComponentExPrivatePCH.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "ComponentExStatics.h"

#include "IsValidEx.h"



USplineMeshBuilderComponent::USplineMeshBuilderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USplineMeshBuilderComponent::BuildSplineMesh()
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("USplineMeshBuilderComponent::BuildSplineMesh"), STAT_SplineMeshBuilderComponentBuildSplineMesh, STATGROUP_ComponentEx);

	auto SpareSplineMeshComponents = SplineMeshComponents;
	Algo::Reverse(SpareSplineMeshComponents);
	SplineMeshComponents.Empty();

	if (IsValid(SplineComponent)
		&& IsValid(Mesh))
	{
		auto SplineLength = SplineComponent->GetSplineLength();
		auto MeshSize = UComponentExStatics::GetMeshSize(Mesh);
		auto MeshLength = MeshSize.X;

		float IntervalStart = 0;
		while (IntervalStart < SplineLength)
		{
			float IntervalEnd = FMath::Clamp(IntervalStart + MeshLength, 0.f, SplineLength);

			USplineMeshComponent* SplineMeshComponent = nullptr;
			if (SpareSplineMeshComponents.Num() > 0)
			{
				SplineMeshComponent = SpareSplineMeshComponents.Last();
				SpareSplineMeshComponents.SetNum(SpareSplineMeshComponents.Num() - 1);
			}
			if (!IsValid(SplineMeshComponent))
			{
				SplineMeshComponent = NewObject<USplineMeshComponent>(GetOuter(), NAME_None, RF_Transactional);

				SplineMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
				SplineMeshComponent->RegisterComponent();
			}

			SplineMeshComponent->SetStaticMesh(Mesh);
			if (IsValid(Material))
				SplineMeshComponent->SetMaterial(0, Material);
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SplineMeshComponent->SetCollisionObjectType(CollisionChannel);
			SplineMeshComponent->SetCullDistance(SplineMeshCullDistance);
			SplineMeshComponent->SetRelativeTransform(SplineComponent->GetRelativeTransform());

			UComponentExStatics::SetupSplineMeshComponentFromSpline(SplineMeshComponent, SplineComponent
				, IntervalStart, IntervalEnd, ESplineCoordinateSpace::Local);

			SplineMeshComponents.Add(SplineMeshComponent);
			IntervalStart = IntervalEnd;
		}
	}

	for (auto SplineMeshComponent : SpareSplineMeshComponents)
	{
		if (!IsValid(SplineMeshComponent))
			continue;

		GetOwner()->RemoveInstanceComponent(SplineMeshComponent);
		SplineMeshComponent->UnregisterComponent();
		SplineMeshComponent->DestroyComponent();
	}
}

void USplineMeshBuilderComponent::SetCollisionEnabled(ECollisionEnabled::Type NewType)
{
	for (auto CollisionComponent : SplineMeshComponents)
	{
		bool bIsEnabled = NewType != ECollisionEnabled::NoCollision;
		CollisionComponent->SetCollisionEnabled(NewType);
		//CollisionComponent->SetCollisionProfileName(CollisionProfileName, true);
		CollisionComponent->SetNotifyRigidBodyCollision(bIsEnabled);
		CollisionComponent->SetGenerateOverlapEvents(bIsEnabled);
		CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	}
}

bool USplineMeshBuilderComponent::OnWorldLoaded()
{
	for (auto SplineMeshComponent : SplineMeshComponents)
	{
		if (!IsValid(SplineMeshComponent))
			continue;

		GetOwner()->RemoveInstanceComponent(SplineMeshComponent);
	}

	return false;
}

bool USplineMeshBuilderComponent::OnWorldSaved()
{
	SetCollisionEnabled(CollisionEnabled);
	for (auto SplineMeshComponent : SplineMeshComponents)
	{
		GetOwner()->AddInstanceComponent(SplineMeshComponent);
	}

	return true;
}

void USplineMeshBuilderComponent::OnHiddenInGameChanged()
{
	Super::OnHiddenInGameChanged();

	for (auto SplineMeshComponent : SplineMeshComponents)
	{
		SplineMeshComponent->SetHiddenInGame(bHiddenInGame);
	}

	SetCollisionEnabled(bHiddenInGame ? ECollisionEnabled::NoCollision : CollisionEnabled);
}
