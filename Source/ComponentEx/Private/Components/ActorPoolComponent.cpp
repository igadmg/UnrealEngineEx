#include "Components/ActorPoolComponent.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"



UActorPoolComponent::UActorPoolComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FActorPoolNode& UActorPoolComponent::GetActorPoolNode(TSubclassOf<AActor> ActorClass)
{
	return ex(ActorPoolNodes).FindOrAdd(ActorClass, [](auto& Key) { return FActorPoolNode(); });
}

AActor* UActorPoolComponent::SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters)
{
	auto& ActorPoolNode = GetActorPoolNode(ActorClass);
	if (ActorPoolNode.PooledActors.Num() == 0)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
		if (!IsValid(World))
			return nullptr;

		return World->SpawnActor(ActorClass, &Transform, SpawnParameters);
	}

	auto Actor = ActorPoolNode.PooledActors.Pop();

	ex(Actor).SetActorEnabled(true);

	return Actor;
}

AActor* UActorPoolComponent::SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters, const TFunction<void(AActor*)> DeferredFn)
{
	auto& ActorPoolNode = GetActorPoolNode(ActorClass);
	if (ActorPoolNode.PooledActors.Num() == 0)
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

	auto Actor = ActorPoolNode.PooledActors.Pop();
	DeferredFn(Actor);

	ex(Actor).SetActorEnabled(true);

	return Actor;
}

AActor* UActorPoolComponent::SpawnActor(TSubclassOf<AActor> Class, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Instigator;

	return SpawnActor(Class, SpawnTransform, SpawnParameters);
}

bool UActorPoolComponent::DestroyActor(AActor* Actor, bool bNetForce, bool bShouldModifyLevel)
{
	if (!IsValid(Actor))
		return false;

	auto& ActorPoolNode = GetActorPoolNode(Actor->GetClass());
	ActorPoolNode.PooledActors.Push(Actor);

	ex(Actor).SetActorEnabled(false);

	return true;
}

#undef LOCTEXT_NAMESPACE
