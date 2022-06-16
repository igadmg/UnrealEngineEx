#include "Components/ActorPoolComponent.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"



UActorPoolComponent::UActorPoolComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UActorPoolComponent::DestroyActor(AActor* Actor, bool bNetForce, bool bShouldModifyLevel)
{
	if (auto World = Actor->GetWorld())
	{
		return World->DestroyActor(Actor);
	}

	return false;
}

#undef LOCTEXT_NAMESPACE
