#include "PendingContainer.h"

#include "UnrealEngineEx.final.h"


UPendingContainer::UPendingContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPendingContainer::AddObject(UObject* Object)
{
	if (OnAddObjectFp)
		OnAddObjectFp(Object);
	else
		PendingObjects.Add(Object);
}

void UPendingContainer::RemoveObject(UObject* Object)
{
	if (OnRemoveObjectFp)
		OnRemoveObjectFp(Object);
	else
		PendingObjects.Remove(Object);
}

void UPendingContainer::Consume(FPendingContainerOnObjectCallback OnObjectAdd, FPendingContainerOnObjectCallback OnObjectRemove)
{
	PendingObjects.Empty();
	OnAddObject = OnObjectAdd;
	OnRemoveObject = OnObjectRemove;

	Consume<UObject>(
		[this](UObject* Object) { OnAddObject.Execute(Object); },
		[this](UObject* Object) { OnRemoveObject.Execute(Object); }
	);
}

void UPendingContainer::ConsumePendingObjects()
{
	for (UObject* Object : PendingObjects)
	{
		OnAddObjectFp(Object);
	}

	PendingObjects.Empty();
}
