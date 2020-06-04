#include "ComponentEx.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"



#define LOCTEXT_NAMESPACE "FComponentExModule"



void FComponentExModule::StartupModule()
{
}

void FComponentExModule::ShutdownModule()
{
}



#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FComponentExModule, ComponentEx)

//DEFINE_LOG_CATEGORY(LogCoordinateSystemEx);

void FComponentEx::SetActorEnabled(AActor* Actor, bool bIsEnabled)
{
	if (!IsValid(Actor))
		return;

	Actor->SetActorHiddenInGame(!bIsEnabled);
	Actor->SetActorEnableCollision(bIsEnabled);
	Actor->SetActorTickEnabled(bIsEnabled);
	for (auto Component : Actor->GetComponents())
	{
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
