#include "CoordinateSystemExStatics.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

#include "CoordinateFrame.h"


#define RETURN_CF_CALL(Object, Call)\
if (auto AsActor = Cast<AActor>(Object))\
{\
	return cf(AsActor).Call;\
}\
else if (auto AsActorComponent = Cast<UActorComponent>(Object))\
{\
	auto AsSceneComponent = Cast<USceneComponent>(AsActorComponent);\
	if (AsSceneComponent)\
		return cf(AsSceneComponent).Call;\
\
	return cf(AsActorComponent).Call;\
}

#define SET_CF_CALL(Object, Call)\
if (auto AsActor = Cast<AActor>(Object))\
{\
	cf(AsActor).Call;\
}\
else if (auto AsActorComponent = Cast<UActorComponent>(Object))\
{\
	if (auto AsSceneComponent = Cast<USceneComponent>(AsActorComponent))\
		cf(AsSceneComponent).Call;\
	else\
		cf(AsActorComponent).Call;\
}

FVector UCoordinateSystemExStatics::GetWorldLocation(class UObject* Object)
{
	if (!IsValid(Object))
		return FVector::ZeroVector;

	RETURN_CF_CALL(Object, GetWorldLocation());

	return FVector::ZeroVector;
}

FTransform UCoordinateSystemExStatics::GetWorldTransform(class UObject* Object)
{
	if (!IsValid(Object))
		return FTransform::Identity;

	RETURN_CF_CALL(Object, GetWorldTransform());

	return FTransform::Identity;
}

void UCoordinateSystemExStatics::SetWorldLocation(class UObject* Object, FVector NewLocation)
{
	if (!IsValid(Object))
		return;

	SET_CF_CALL(Object, SetWorldLocation(NewLocation));
}

void UCoordinateSystemExStatics::SetWorldTransform(class UObject* Object, FTransform NewTransform)
{
	if (!IsValid(Object))
		return;

	SET_CF_CALL(Object, SetWorldTransform(NewTransform));
}

#undef RETURN_CF_CALL
#undef SET_CF_CALL
