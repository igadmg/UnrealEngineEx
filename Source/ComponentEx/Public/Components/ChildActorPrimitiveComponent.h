#pragma once

#include "Components/PrimitiveComponent.h"
#include "ChildActorPrimitiveComponent.generated.h"


UCLASS(ShowCategories = "Mobility")
class COMPONENTEX_API UChildActorPrimitiveComponent : public UPrimitiveComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY(Category = "ChildActorPrimitiveComponent", EditAnywhere, BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "true", OnlyPlaceable))
	TSubclassOf<AActor> ChildActorClass;

	UPROPERTY(Category = "ChildActorPrimitiveComponent", VisibleAnywhere, BlueprintReadOnly, Replicated)
	TSoftObjectPtr<AActor> ChildActorPtr;

	/** Temprory storage of Actors attached to Child Actor. They are reattached if actor is recreated. */
	UPROPERTY()
	TArray<class AActor*> ChildAttachedActors;


public:
	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintPure)
	AActor* GetChildActor() const { return ChildActorPtr.Get(); }

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintCallable)
	void SetChildActor(AActor* ChildActor, FName SocketName = NAME_None);

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintPure)
	AActor* GetChildActorTemplate() const { return ChildActorClass ? ChildActorClass->GetDefaultObject<AActor>() : nullptr; }

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintCallable)
	void SetChildActorClass(TSubclassOf<AActor> InClass)
	{
		SetChildActorClass(InClass, nullptr);
	}

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintCallable)
	void CreateChildActor(AActor* ActorTemplate = nullptr);

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintCallable)
	void DestroyChildActor();

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintCallable)
	void ClearChildActor();

	UFUNCTION(Category = "UnrealEngineEx: ChildActorPrimitiveComponent", BlueprintCallable)
	void CloneChildActor(AActor* SrcActor);


public:
	UChildActorPrimitiveComponent(const FObjectInitializer& ObjectInitializer);


public:
	void SetChildActorClass(TSubclassOf<AActor> InClass, AActor* NewChildActorTemplate);

	void SetPreventDestroyOnUnregister(bool NewValue);


protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyThatWillChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual void PostEditImport() override;
	virtual void PostEditUndo() override;
	virtual void PostLoad() override;
#endif

	virtual FVector GetChildActorLocation() const { return GetChildActorTransform().GetLocation(); }
	virtual FRotator GetChildActorRotation() const { return GetChildActorTransform().Rotator(); }
	virtual FTransform GetChildActorTransform() const { return GetComponentTransform(); }

	virtual void OnChildActorSpawned(AActor* NewChildActor) {}
	virtual void OnChildActorCreated(AActor* NewChildActor) {}
	virtual void OnChildActorDestroyed(AActor* OldChildActor) {}


private:
	bool bPreventDestroyOnUnregister = false;
};

