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
	Actor->SetActorTransform(Transform, false, nullptr, ETeleportType::ResetPhysics);

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
				Actor->FinishSpawning(Transform, false, &InstanceDataCache);
			}
			else
				Actor->FinishSpawning(Transform, false);

			return Actor;
		}

		return nullptr;
	}

	auto Actor = ActorPoolNode.PooledActors.Pop();
	Actor->SetActorTransform(Transform, false, nullptr, ETeleportType::ResetPhysics);

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

AActor* UActorPoolComponent::SpawnActorDeferred(TSubclassOf<AActor> ActorClass, const FTransform& Transform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator, FActorPoolDeferredCallback& Finish)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Instigator;
	SpawnParameters.bDeferConstruction = true;

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
			Finish.Actor = Actor;
			Finish.Delegate.BindWeakLambda(this, [this, Transform, SpawnParameters](AActor* Actor) {
					if (Valid(SpawnParameters.Template))
					{
						FComponentInstanceDataCache InstanceDataCache(SpawnParameters.Template);
						Actor->FinishSpawning(Transform, false, &InstanceDataCache);
					}
					else
						Actor->FinishSpawning(Transform, false);
				});

			return Actor;
		}

		return nullptr;
	}

	auto Actor = ActorPoolNode.PooledActors.Pop();
	Actor->SetActorTransform(Transform, false, nullptr, ETeleportType::ResetPhysics);

	Finish.Actor = Actor;
	Finish.Delegate.BindWeakLambda(this, [Transform, SpawnParameters](AActor* Actor) {
			ex(Actor).SetActorEnabled(true);

			if (Valid(SpawnParameters.Template))
			{
				FComponentInstanceDataCache InstanceDataCache(SpawnParameters.Template);
				Actor->FinishSpawning(Transform, true, &InstanceDataCache);
			}
			else
				Actor->FinishSpawning(Transform, true);
		});

	return Actor;
}

bool UActorPoolComponent::FinishSpawnActorDeferred(FActorPoolDeferredCallback Callback)
{
	Callback.Call();

	return true;
}

bool UActorPoolComponent::DestroyActor(AActor* Actor, bool bNetForce, bool bShouldModifyLevel)
{
	if (!IsValid(Actor))
		return false;


	auto& ActorPoolNode = GetActorPoolNode(Actor->GetClass());
	ensure(!ActorPoolNode.PooledActors.Contains(Actor));

	ActorPoolNode.PooledActors.Push(Actor);

	ex(Actor).SetActorEnabled(false);
	Actor->Destroyed();

	return true;
}

#undef LOCTEXT_NAMESPACE
