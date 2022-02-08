#pragma once

#include "Components/ActorComponent.h"
#include "Components/ShapeComponent.h"
#include "CompositeColliderComponent.generated.h"



UCLASS(ClassGroup = "Collision", hidecategories = (Object, LOD, Lighting, TextureStreaming), editinlinenew, meta = (DisplayName = "Composite Collision", BlueprintSpawnableComponent))
class COMPONENTEX_API UCompositeColliderComponent : public UActorComponent
{
	GENERATED_BODY()



protected:
	UPROPERTY(Category = "Shape", EditAnywhere, BlueprintReadOnly, export)
	TArray<class UShapeComponent*> Colliders;

public:
	UPROPERTY(Category = "Collision", BlueprintAssignable)
	FComponentHitSignature OnComponentHit;



public:
	UFUNCTION(Category = "Collision", BlueprintCallable)
	void InitializeCollider();



public:
	UCompositeColliderComponent();

	virtual void InitializeComponent() override;



protected:
	int32 CollisionFlags;
	FHitResult CollisionSweepResult;


	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
