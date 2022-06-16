#pragma once

#include "Components/ActorComponent.h"

#include "ActorPoolComponent.generated.h"


UCLASS()
class COMPONENTEX_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	bool DestroyActor(AActor* Actor, bool bNetForce = false, bool bShouldModifyLevel = true);


public:
	UActorPoolComponent(const FObjectInitializer& ObjectInitializer);
};
