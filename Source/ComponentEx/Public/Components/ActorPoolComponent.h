#pragma once

#include "Components/ActorComponent.h"

#include "ActorPoolComponent.generated.h"


UCLASS()
class COMPONENTEX_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters);
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters, const TFunction<void(AActor*)> DeferredFn);

	UFUNCTION(BlueprintCallable)
	bool DestroyActor(AActor* Actor, bool bNetForce = false, bool bShouldModifyLevel = true);


public:
	UActorPoolComponent(const FObjectInitializer& ObjectInitializer);
};
