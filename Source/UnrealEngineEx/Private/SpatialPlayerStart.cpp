#include "SpatialPlayerStart.h"

#include "UnrealEngineEx.final.h"


ASpatialPlayerStart::ASpatialPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FTransform ASpatialPlayerStart::GetStartTransform_Implementation() const
{
	return GetActorTransform();
}
