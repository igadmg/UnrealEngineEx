#include "Components/DiscComponent.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "PrimitiveSceneProxy.h"

#include "ComponentEx.final.h"


UDiscComponent::UDiscComponent()
{
	bWantsInitializeComponent = true;
}

void UDiscComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDiscComponent::OnRegister()
{
	Super::OnRegister();

	SphereCollider = NewObject<USphereComponent>(GetOwner());
	SphereCollider->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &UDiscComponent::OnOverlapSphereBegin);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &UDiscComponent::OnOverlapSphereEnd);

	BoxCollider = NewObject<UBoxComponent>(GetOwner());
	BoxCollider->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &UDiscComponent::OnOverlapBoxBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &UDiscComponent::OnOverlapBoxEnd);

	SphereCollider->RegisterComponent();
	BoxCollider->RegisterComponent();
}

void UDiscComponent::DestroyComponent(bool bPromoteChildren)
{
	SphereCollider->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SphereCollider->DestroyComponent(bPromoteChildren);
	BoxCollider->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	BoxCollider->DestroyComponent(bPromoteChildren);

	SphereCollider = nullptr;
	BoxCollider = nullptr;

	Super::DestroyComponent(bPromoteChildren);
}

FPrimitiveSceneProxy* UDiscComponent::CreateSceneProxy()
{
	class FDiscSceneProxy : public FPrimitiveSceneProxy
	{
	public:
		FDiscSceneProxy(const UDiscComponent* InComponent)
			: FPrimitiveSceneProxy(InComponent)
			, bDrawOnlyIfSelected(InComponent->bDrawOnlyIfSelected)
			, Radius(InComponent->Radius)
			, Height(InComponent->Height)
			, DiscColor(InComponent->ShapeColor)
		{
			bWillEverBeLit = false;
		}

		virtual SIZE_T GetTypeHash() const override
		{
			static size_t UniquePointer;
			return reinterpret_cast<size_t>(&UniquePointer);
		}

		virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
		{
			QUICK_SCOPE_CYCLE_COUNTER(STAT_BoxSceneProxy_GetDynamicMeshElements);

			const FMatrix& LocalToWorld = GetLocalToWorld();

			for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
			{
				if (VisibilityMap & (1 << ViewIndex))
				{
					const FSceneView* View = Views[ViewIndex];

					const FLinearColor DrawColor = GetViewSelectionColor(DiscColor, *View, IsSelected(), IsHovered(), false, IsIndividuallySelected());

					FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
					DrawWireCylinder(PDI, LocalToWorld.GetOrigin(), LocalToWorld.GetScaledAxis(EAxis::X), LocalToWorld.GetScaledAxis(EAxis::Y), LocalToWorld.GetScaledAxis(EAxis::Z), DrawColor, Radius, Height/2.f, 16, SDPG_World);
				}
			}
		}

		virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
		{
			const bool bProxyVisible = !bDrawOnlyIfSelected || IsSelected();

			// Should we draw this because collision drawing is enabled, and we have collision
			const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();

			FPrimitiveViewRelevance Result;
			Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
			Result.bDynamicRelevance = true;
			Result.bShadowRelevance = IsShadowCast(View);
			Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
			return Result;
		}
		virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }
		uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

	private:
		const uint32	bDrawOnlyIfSelected : 1;
		const float		Radius;
		const float		Height;
		const FColor	DiscColor;
	};

	return new FDiscSceneProxy(this);
}

void UDiscComponent::UpdateBodySetup()
{
	if (SphereCollider && BoxCollider)
	{
		SphereCollider->SetSphereRadius(Radius);
		BoxCollider->SetBoxExtent(FVector(Radius, Radius, Height));

		SphereCollider->UpdateBodySetup();
		BoxCollider->UpdateBodySetup();
	}
}

FBoxSphereBounds UDiscComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FBox(-FVector(Radius, Radius, Height), FVector(Radius, Radius, Height))).TransformBy(LocalToWorld);
}

void UDiscComponent::OnOverlapBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || (OtherActor == (AActor*)GetOuter()) || !IsValid(OtherComp))
		return;

	CollisionFlags |= 0x01;
	CheckCollisionEvent(OtherActor, OtherComp, SweepResult);
}

void UDiscComponent::OnOverlapSphereBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || (OtherActor == (AActor*)GetOuter()) || !IsValid(OtherComp))
		return;

	CollisionFlags |= 0x02;
	CheckCollisionEvent(OtherActor, OtherComp, SweepResult);
}

void UDiscComponent::OnOverlapBoxEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || (OtherActor == (AActor*)GetOuter()) || !IsValid(OtherComp))
		return;

	CollisionFlags &= ~0x01;
}

void UDiscComponent::OnOverlapSphereEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || (OtherActor == (AActor*)GetOuter()) || !IsValid(OtherComp))
		return;

	CollisionFlags &= ~0x02;
}

void UDiscComponent::CheckCollisionEvent(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& SweepResult)
{
	if (CollisionFlags == 0x03)
	{
		OnComponentHit.Broadcast(nullptr, OtherActor, OtherComp, FVector::ZeroVector, SweepResult);
	}
}
