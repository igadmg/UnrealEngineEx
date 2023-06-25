#pragma once

#include "Components/ActorComponent.h"
#include "Engine/World.h"

#include "ActorPoolComponent.generated.h"


DECLARE_DELEGATE_OneParam(FActorPoolDeferredDelegate, AActor*);

USTRUCT()
struct COMPONENTEX_API FActorPoolNode
{
	GENERATED_BODY()


	UPROPERTY()
	TArray<AActor*> PooledActors;
};

USTRUCT(BlueprintType)
struct COMPONENTEX_API FActorPoolDeferredCallback
{
	GENERATED_BODY()


	UPROPERTY()
	AActor *Actor = nullptr;

	FActorPoolDeferredDelegate Delegate/* = FActorPoolDeferredDelegate()*/;
	void Call() { Delegate.ExecuteIfBound(Actor); };
};


UCLASS()
class COMPONENTEX_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	static FName TagPooled;


protected:
	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, FActorPoolNode> ActorPoolNodes;


public:
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ActorClass"))
	AActor* SpawnActor(TSubclassOf<AActor> Class, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ActorClass"))
	AActor* SpawnActorDeferred(TSubclassOf<AActor> Class, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator, FActorPoolDeferredCallback& Finish);

	UFUNCTION(BlueprintCallable)
	bool FinishSpawnActorDeferred(FActorPoolDeferredCallback Callback);

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Actor"))
	bool DestroyActor(AActor* Actor, bool bNetForce = false, bool bShouldModifyLevel = true);


public:
	UActorPoolComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters);
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters, const TFunction<void(AActor*)> DeferredFn);


protected:
	FActorPoolNode& GetActorPoolNode(TSubclassOf<AActor> ActorClass);
};
