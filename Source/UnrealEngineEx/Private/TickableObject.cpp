#include "TickableObject.h"

#include "UnrealEngineEx.final.h"



UTickableObject::UTickableObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTickableObject::Tick_Implementation(float DeltaSeconds)
{
}
