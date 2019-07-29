#include "CoordinateSystemExPrivatePCH.h"
#include "CoordinateSystemExStatics.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

#include "CoordinateFrame.h"



FVector UCoordinateSystemExStatics::GetWorldLocation(class UObject* Object)
{
	if (!IsValid(Object))
		return FVector::ZeroVector;

	AActor* AsActor = Cast<AActor>(Object);
	if (AsActor)
	{
		return MakeCoordinateFrame(AsActor).GetWorldLocation();
	}
	else
	{
		USceneComponent* AsSceneComponent = Cast<USceneComponent>(Object);
		if (AsSceneComponent)
			return MakeCoordinateFrame(AsSceneComponent).GetWorldLocation();

		UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
		if (AsActorComponent)
			return MakeCoordinateFrame(AsActorComponent).GetWorldLocation();
	}

	return FVector::ZeroVector;
}

FTransform UCoordinateSystemExStatics::GetWorldTransform(class UObject* Object)
{
	if (!IsValid(Object))
		return FTransform::Identity;

	AActor* AsActor = Cast<AActor>(Object);
	if (AsActor)
	{
		return MakeCoordinateFrame(AsActor).GetWorldTransform();
	}
	else
	{
		USceneComponent* AsSceneComponent = Cast<USceneComponent>(Object);
		if (AsSceneComponent)
			return MakeCoordinateFrame(AsSceneComponent).GetWorldTransform();

		UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
		if (AsActorComponent)
			return MakeCoordinateFrame(AsActorComponent).GetWorldTransform();
	}

	return FTransform::Identity;
}

void UCoordinateSystemExStatics::SetWorldLocation(class UObject* Object, FVector NewLocation)
{
	if (!IsValid(Object))
		return;

	AActor* AsActor = Cast<AActor>(Object);
	if (AsActor)
	{
		MakeCoordinateFrame(AsActor).SetWorldLocation(NewLocation);
	}
	else
	{
		USceneComponent* AsSceneComponent = Cast<USceneComponent>(Object);
		if (AsSceneComponent)
			MakeCoordinateFrame(AsSceneComponent).SetWorldLocation(NewLocation);

		UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
		if (AsActorComponent)
			MakeCoordinateFrame(AsActorComponent).SetWorldLocation(NewLocation);
	}
}

void UCoordinateSystemExStatics::SetWorldTransform(class UObject* Object, FTransform NewTransform)
{
	if (!IsValid(Object))
		return;

	AActor* AsActor = Cast<AActor>(Object);
	if (AsActor)
	{
		MakeCoordinateFrame(AsActor).SetWorldTransform(NewTransform);
	}
	else
	{
		USceneComponent* AsSceneComponent = Cast<USceneComponent>(Object);
		if (AsSceneComponent)
			MakeCoordinateFrame(AsSceneComponent).SetWorldTransform(NewTransform);

		UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
		if (AsActorComponent)
			MakeCoordinateFrame(AsActorComponent).SetWorldTransform(NewTransform);
	}
}
