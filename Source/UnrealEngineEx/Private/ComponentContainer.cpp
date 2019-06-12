#include "UnrealEngineExPrivatePCH.h"
#include "ComponentContainer.h"

#include "Engine/ActorChannel.h"
#include "GameFramework/Actor.h"

#include "IsValidEx.h"



UComponentContainer::UComponentContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UComponentContainer::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UActorComponent* Component : Components)
	{
		if (IsValid(Component) && Component->GetIsReplicated())
		{
			bool bNetInitial = RepFlags->bNetInitial;
			RepFlags->bNetInitial = Channel->ReplicationMap.Find(Component) == nullptr;

			WroteSomething |= Component->ReplicateSubobjects(Channel, Bunch, RepFlags);
			WroteSomething |= Channel->ReplicateSubobject(Component, *Bunch, *RepFlags);

			RepFlags->bNetInitial = bNetInitial;
		}
	}

	return WroteSomething;
}


UContainedComponent* UComponentContainer::GetComponent(TSubclassOf<UContainedComponent> Class) const
{
	for (auto Component : Components)
	{
		if (Component->IsA(Class))
		{
			return Component;
		}
	}

	return nullptr;
}

UContainedComponent* UComponentContainer::AddComponent(TSubclassOf<UContainedComponent> Class)
{
	if (!IsValid(Class))
		return nullptr;

	UContainedComponent* Component = NewObject<UContainedComponent>(this, Class);
	Components.Add(Component);
	//Component
	UComponentContainerManager::RegisterPendingComponent(this, Component);

	return Component;
}

bool UComponentContainer::RemoveComponent(TSubclassOf<UContainedComponent> Class)
{
	return Components.RemoveAll([Class](UContainedComponent* Component) {
			if (Component->IsA(Class))
			{
				Component->DestroyComponent();
				return true;
			}

			return false;
		}) > 0;
}


UComponentContainerManager* UComponentContainerManager::Instance = nullptr;

UComponentContainerManager::UComponentContainerManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Instance = this;
}

void UComponentContainerManager::RegisterPendingComponent(const UObject* WorldContextObject, UContainedComponent* Component)
{
	if (IsValid(Instance))
	{
		Instance->PendingComponents.Add(Component);
		if (!IsValid(Instance->RegisterPendingComponentsHandle))
		{
			Instance->RegisterPendingComponentsHandle = FWorldDelegates::OnWorldPostActorTick
				.AddUObject(Instance, &UComponentContainerManager::RegisterPendingComponents);
		}
	}
}

void UComponentContainerManager::RegisterPendingComponents(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	for (UContainedComponent* Component : PendingComponents)
	{
		Component->RegisterComponent();
		Component->EnableComponentReplication();
		Component->GetOwner()->ForceNetUpdate();
	}

	PendingComponents.Empty();
	FWorldDelegates::OnWorldPostActorTick.Remove(RegisterPendingComponentsHandle);
	RegisterPendingComponentsHandle.Reset();
}



UContainedComponent::UContainedComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

UComponentContainer* UContainedComponent::GetComponentContainer() const
{
	if (auto Actor = Valid(GetOwner()))
	{
		return Valid<UComponentContainer>(Actor->GetComponentByClass(UComponentContainer::StaticClass()));
	}

	return nullptr;
}

void UContainedComponent::EnableComponentReplication()
{
	// Enable replication but do not register component as Actor's replicated component.
	bReplicates = true;
}

bool UContainedComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, struct FReplicationFlags* RepFlags)
{
	ReplicatedChannels.Add(Channel);

	return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
}

void UContainedComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (auto Container = GetComponentContainer())
	{
		Container->Components.Add(this);
		UComponentContainerManager::Instance->OnComponentAdded.Broadcast(this);
	}
}

void UContainedComponent::UninitializeComponent()
{
	if (auto Container = GetComponentContainer())
	{
		UComponentContainerManager::Instance->OnComponentRemoved.Broadcast(this);
		Container->Components.Remove(this);
	}

	Super::UninitializeComponent();
}
