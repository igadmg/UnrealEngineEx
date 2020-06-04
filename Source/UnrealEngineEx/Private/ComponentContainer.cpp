#include "ComponentContainer.h"

#include "Engine/ActorChannel.h"
#include "GameFramework/Actor.h"

#include "IsValidEx.h"



UComponentContainer::UComponentContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = true;
	bWantsInitializeComponent = true;
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


void UComponentContainer::InitializeComponent()
{
	Super::InitializeComponent();

	if_HasAuthority(this)
	{
		for (auto ComponentClass : InitialComponents)
		{
			AddComponent(ComponentClass);
		}
	}
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
	if (auto ComponentContainerManager = Valid(UComponentContainerManager::Instance))
	{
		ComponentContainerManager->Components.Add(Component);
		UComponentContainerManager::RegisterPendingComponent(this, Component);
	}

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

UComponentContainerManager::~UComponentContainerManager()
{
	Instance = nullptr;
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
	}

	PendingComponents.Empty();
	FWorldDelegates::OnWorldPostActorTick.Remove(RegisterPendingComponentsHandle);
	RegisterPendingComponentsHandle.Reset();
}



UContainedComponent::UContainedComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = true;
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
}

bool UContainedComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, struct FReplicationFlags* RepFlags)
{
	ReplicatedChannels.Add(Channel);

	return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
}

void UContainedComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetIsReplicated(false);		// Unregister component as Actor's replicated component.

	if (auto Container = GetComponentContainer())
	{
		Container->Components.AddUnique(this);
		if (auto ComponentContainerManager = Valid(UComponentContainerManager::Instance))
		{
			ComponentContainerManager->Components.AddUnique(this);
			ComponentContainerManager->OnComponentAdded.Broadcast(this);
		}
	}
}

void UContainedComponent::UninitializeComponent()
{
	if (auto Container = GetComponentContainer())
	{
		if (auto ComponentContainerManager = Valid(UComponentContainerManager::Instance))
		{
			ComponentContainerManager->OnComponentRemoved.Broadcast(this);
			ComponentContainerManager->Components.Remove(this);
		}
		Container->Components.Remove(this);
	}

	Super::UninitializeComponent();
}

void UContainedComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Not working
	//bReplicates = true;			// Enable replication but do not register component as Actor's replicated component.
	GetOwner()->ForceNetUpdate();
}
