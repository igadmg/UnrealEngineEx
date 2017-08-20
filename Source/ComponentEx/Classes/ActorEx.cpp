#include "ComponentExPrivatePCH.h"
#include "ActorEx.h"



UCameraComponent* FActorEx::GetCameraComponent() const
{
	if (!IsValid(This))
		return nullptr;


	if (This->bFindCameraComponentWhenViewTarget)
	{
		// Look for the first active camera component and use that for the view
		TInlineComponentArray<UCameraComponent*> Cameras;
		This->GetComponents<UCameraComponent>(/*out*/ Cameras);

		for (UCameraComponent* CameraComponent : Cameras)
		{
			if (CameraComponent->bIsActive)
			{
				return CameraComponent;
			}
		}
	}

	return nullptr;
}
