#pragma once

#include "Components\ActorComponent.h"
#include "UnrealEngineEx.h"
#include <functional>
#include "ComponentContainer.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComponentContainerOnActorComponent, class UContainedComponent*, Component);


UCLASS(BlueprintType)
class UNREALENGINEEX_API UComponentContainer : public UActorComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class UContainedComponent>> InitialComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UContainedComponent*> Components;


public:
	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	class UContainedComponent* GetComponent(TSubclassOf<class UContainedComponent> Class) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	class UContainedComponent* AddComponent(TSubclassOf<class UContainedComponent> Class);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool RemoveComponent(TSubclassOf<class UContainedComponent> Class);


public:
	UComponentContainer(const FObjectInitializer& ObjectInitializer);


public:
	template <typename T>
	T* GetComponent() const { return Cast<T>(GetComponent(T::StaticClass())); }

	template <typename T>
	T* AddComponent(UClass* Class = T::StaticClass()) { return Cast<T>(AddComponent(Class)); }

	template <typename T>
	bool RemoveComponent() { return RemoveComponent(T::StaticClass()); }

	template <typename T, typename TComponent = UContainedComponent>
	void ForeachComponent(std::function<void (TComponent*)> Callback) const
	{
		for (class UContainedComponent* Component : Components)
		{
			if (auto TC = Cast<TComponent>(Component))
			{
				if_ImplementsT(T, IComponent, TC)
				{
					Callback(IComponent);
				}
			}
		}
	}


protected:
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, struct FReplicationFlags *RepFlags) override;
	virtual void InitializeComponent() override;
};


UCLASS()
class UNREALENGINEEX_API UComponentContainerManager : public UObject
{
	GENERATED_BODY()


public:
	static UComponentContainerManager* Instance;


public:
	static void RegisterPendingComponent(const UObject* WorldContextObject, class UContainedComponent* Component);

	template <typename T>
	static UComponentContainer* GetAssociatedContainer(const UObject* Object)
	{
		if (auto AssociatedObject = FUnrealEngineEx::GetAssociatedObject<typename T::TAssociatedType>(Object))
		{
			return AssociatedObject->ComponentContainer;
		}

		return nullptr;
	}

	template <typename T>
	static T* GetComponent(const UObject* Object)
	{
		if (auto AssociatedContainer = UComponentContainerManager::GetAssociatedContainer<T>(Object))
		{
			return Cast<T>(AssociatedContainer->GetComponent(T::StaticClass()));
		}

		return nullptr;
	}

	template <typename T>
	static T* AddComponent(const UObject* Object, UClass* Class = T::StaticClass())
	{
		if (auto AssociatedContainer = UComponentContainerManager::GetAssociatedContainer<T>(Object))
		{
			return Cast<T>(AssociatedContainer->AddComponent(Class));
		}

		return nullptr;
	}

	template <typename T>
	static bool RemoveComponent(const UObject* Object)
	{
		if (auto AssociatedContainer = UComponentContainerManager::GetAssociatedContainer<T>(Object))
		{
			return AssociatedContainer->RemoveComponent(T::StaticClass());
		}

		return false;
	}


public:
	UPROPERTY()
	TArray<class UContainedComponent*> Components;

	/* client side cosmetic event */
	UPROPERTY(BlueprintAssignable)
	FComponentContainerOnActorComponent OnComponentAdded;

	/* client side cosmetic event */
	UPROPERTY(BlueprintAssignable)
	FComponentContainerOnActorComponent OnComponentRemoved;


protected:
	UPROPERTY()
	TArray<class UContainedComponent*> PendingComponents;

	FDelegateHandle RegisterPendingComponentsHandle;
	void RegisterPendingComponents(UWorld* World, ELevelTick TickType, float DeltaSeconds);


public:
	UComponentContainerManager(const FObjectInitializer& ObjectInitializer);
	virtual ~UComponentContainerManager();
};


UCLASS(Abstract)
class UNREALENGINEEX_API UContainedComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	int32 GetReplicatedNumberOfPlayers() const { return ReplicatedChannels.Num(); }


public:
	UContainedComponent(const FObjectInitializer& ObjectInitializer);


public:
	FORCEINLINE UComponentContainer* GetComponentContainer() const;
	void EnableComponentReplication();


protected:
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, struct FReplicationFlags* RepFlags) override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void BeginPlay() override;


protected:
	TSet<class UActorChannel*> ReplicatedChannels;
};
