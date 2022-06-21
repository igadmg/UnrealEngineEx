#pragma once

#include "Components/ActorComponent.h"

#include "ActorPoolComponent.generated.h"


USTRUCT()
struct COMPONENTEX_API FActorPoolNode
{
	GENERATED_BODY()


	UPROPERTY()
	TArray<AActor*> PooledActors;
};

UCLASS()
class COMPONENTEX_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, FActorPoolNode> ActorPoolNodes;


public:
	UFUNCTION(BlueprintCallable)
	bool DestroyActor(AActor* Actor, bool bNetForce = false, bool bShouldModifyLevel = true);


public:
	UActorPoolComponent(const FObjectInitializer& ObjectInitializer);


public:
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters);
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters, const TFunction<void(AActor*)> DeferredFn);


protected:
	FActorPoolNode& GetActorPoolNode(TSubclassOf<AActor> ActorClass);
};
