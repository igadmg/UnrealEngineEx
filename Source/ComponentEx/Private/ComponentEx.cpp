#include "ComponentExPrivatePCH.h"
#include "ComponentEx.h"



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
	Actor->SetActorHiddenInGame(!bIsEnabled);
	Actor->SetActorEnableCollision(bIsEnabled);
	Actor->SetActorTickEnabled(bIsEnabled);
	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

#if 0
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
