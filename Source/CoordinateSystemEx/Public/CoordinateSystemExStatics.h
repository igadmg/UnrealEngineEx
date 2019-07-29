#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoordinateSystemExStatics.generated.h"



UCLASS()
class COORDINATESYSTEMEX_API UCoordinateSystemExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	//static FVector GetRelativeLocation(class UActorComponent* Object);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	//static FRotator GetRelativeRotation(class UActorComponent* Object);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	//static FVector GetRelativeScale(class UActorComponent* Object);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	//static FTransform GetRelativeTransform(class UActorComponent* Object);
	UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static FVector GetWorldLocation(class UObject* Object);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	//static FRotator GetWorldRotation(class UActorComponent* Object);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	//static FVector GetWorldScale(class UActorComponent* Object);
	UFUNCTION(Category = "CoordinateSystemEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static FTransform GetWorldTransform(class UObject* Object);

	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetRelativeLocation(class UActorComponent* Object, FVector NewLocation);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetRelativeRotation(class UActorComponent* Object, FRotator NewRotation);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetRelativeRotation(class UActorComponent* Object, FQuat NewRotation);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetRelativeScale(class UActorComponent* Object, FVector NewScale);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetRelativeTransform(class UActorComponent* Object, FTransform NewTransform);
	UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	static void SetWorldLocation(class UObject* Object, FVector NewLocation);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetWorldRotation(class UActorComponent* Object, FRotator NewRotation);
	//UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	//static void SetWorldScale(class UActorComponent* Object, FVector NewScale);
	UFUNCTION(Category = "CoordinateSystemEx", BlueprintCallable, meta = (DefaultToSelf = "Object"))
	static void SetWorldTransform(class UObject* Object, FTransform NewTransform);
};
