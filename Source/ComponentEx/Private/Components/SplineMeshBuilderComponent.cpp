#include "Components/SplineMeshBuilderComponent.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "ComponentExStatics.h"
#include "ComponentPool.h"

#include "ComponentEx.final.h"



USplineMeshBuilderComponent::USplineMeshBuilderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USplineMeshBuilderComponent::BuildSplineMesh()
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("USplineMeshBuilderComponent::BuildSplineMesh"), STAT_SplineMeshBuilderComponentBuildSplineMesh, STATGROUP_ComponentEx);

	auto SplineMeshComponentsPool = TComponentPool<USplineMeshComponent>(SplineMeshComponents, [this](auto Component) {
			GetOwner()->RemoveInstanceComponent(Component);
			Component->DestroyBodySetup();
		});
	SplineMeshComponents.Empty();

	if (IsValid(SplineComponent) && IsValid(Mesh))
	{
		auto SplineLength = SplineComponent->GetSplineLength();
		auto MeshSize = UComponentExStatics::GetMeshSize(Mesh);
		auto MeshLength = MeshSize.X;

		float IntervalStart = 0;
		while (IntervalStart < SplineLength)
		{
			float IntervalEnd = FMath::Clamp(IntervalStart + MeshLength, 0.f, SplineLength);

			auto SplineMeshComponent = SplineMeshComponentsPool.Take();
			if (!IsValid(SplineMeshComponent))
			{
				SplineMeshComponent = NewObject<USplineMeshComponent>(GetOuter(), NAME_None, RF_Transactional);

				SplineMeshComponent->SetMobility(Mobility);
				SplineMeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
				SplineMeshComponent->RegisterComponent();
			}
			else
			{
				SplineMeshComponent->SetMobility(Mobility);
			}

			SplineMeshComponent->SetStaticMesh(Mesh);
			if (IsValid(Material))
			{
				SplineMeshComponent->SetMaterial(0, Material);
			}

			SplineMeshComponent->SetCullDistance(SplineMeshCullDistance);
			SplineMeshComponent->SetRelativeTransform(cf(SplineComponent).GetRelativeTransform());
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SplineMeshComponent->SetCollisionObjectType(CollisionPresets.GetObjectType());

			UComponentExStatics::SetupSplineMeshComponentFromSpline(SplineMeshComponent, SplineComponent
				, IntervalStart, IntervalEnd, ESplineCoordinateSpace::Local);

			SplineMeshComponents.Add(SplineMeshComponent);
#if WITH_EDITOR
			if (bDebugKeepComponentInstances)
			{
				GetOwner()->AddInstanceComponent(SplineMeshComponent);
			}
#endif

			IntervalStart = IntervalEnd;
		}
	}
}

void USplineMeshBuilderComponent::SetCollisionEnabled(ECollisionEnabled::Type NewType)
{
	for (auto CollisionComponent : SplineMeshComponents)
	{
		bool bIsEnabled = NewType != ECollisionEnabled::NoCollision;
		CollisionComponent->SetCollisionEnabled(NewType);
		CollisionComponent->SetCollisionProfileName(CollisionPresets.GetCollisionProfileName(), true);
		CollisionComponent->SetNotifyRigidBodyCollision(bIsEnabled);
		CollisionComponent->SetGenerateOverlapEvents(bGenerateOverlapEvents);
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
	SetCollisionEnabled(CollisionPresets.GetCollisionEnabled());
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

	SetCollisionEnabled(bHiddenInGame ? ECollisionEnabled::NoCollision : CollisionPresets.GetCollisionEnabled());
}
