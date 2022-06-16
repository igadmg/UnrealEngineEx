#include "Extensions/ActorEx.h"

#include "Camera/CameraComponent.h"

#include "ComponentEx.final.h"


UCameraComponent* FAActorConstEx::GetCameraComponent() const
{
	if (!IsValid(This()))
		return nullptr;


	if (This()->bFindCameraComponentWhenViewTarget)
	{
		// Look for the first active camera component and use that for the view
		TInlineComponentArray<UCameraComponent*> Cameras;
		This()->GetComponents<UCameraComponent>(/*out*/ Cameras);

		for (UCameraComponent* CameraComponent : Cameras)
		{
			if (CameraComponent->IsActive())
			{
				return CameraComponent;
			}
		}
	}

	return nullptr;
}

void FAActorMutableEx::SetActorEnabled(bool bIsEnabled)
{
	if (!IsValid(This()))
		return;

	This()->SetActorHiddenInGame(!bIsEnabled);
	This()->SetActorEnableCollision(bIsEnabled);
	This()->SetActorTickEnabled(bIsEnabled);
	for (auto Component : This()->GetComponents())
	{
		Component->SetActive(bIsEnabled);
		Component->SetComponentTickEnabled(bIsEnabled);
	}

#if 0
	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	if (MeshComponent != nullptr)
	{
		if (bIsEnabled)
		{
			FNavigationSystem::OnComponentRegistered(*MeshComponent);
		}
		else
		{

			FNavigationSystem::OnComponentUnregistered(*MeshComponent);
		}
	}
#endif
}
