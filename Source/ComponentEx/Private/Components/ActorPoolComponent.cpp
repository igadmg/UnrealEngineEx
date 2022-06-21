#include "Components/ActorPoolComponent.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"



UActorPoolComponent::UActorPoolComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

AActor* UActorPoolComponent::SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters)
{
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	return World->SpawnActor(ActorClass, &Transform, SpawnParameters);
}

AActor* UActorPoolComponent::SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters, const TFunction<void(AActor*)> DeferredFn)
{
	auto LocalSpawnParameters = SpawnParameters;
	LocalSpawnParameters.bDeferConstruction = true;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	if (auto Actor = World->SpawnActor(ActorClass, &Transform, LocalSpawnParameters))
	{
		DeferredFn(Actor);

		if (Valid(LocalSpawnParameters.Template))
		{
			FComponentInstanceDataCache InstanceDataCache(LocalSpawnParameters.Template);
			Actor->FinishSpawning(Transform, true, &InstanceDataCache);
		}
		else
			Actor->FinishSpawning(Transform, true);

		return Actor;
	}

	return nullptr;
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
