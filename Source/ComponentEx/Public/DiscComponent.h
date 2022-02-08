#pragma once

#include "Components/ShapeComponent.h"
#include "DiscComponent.generated.h"



UCLASS(ClassGroup = "Collision", hidecategories = (Object, LOD, Lighting, TextureStreaming), editinlinenew, meta = (DisplayName = "Disc Collision", BlueprintSpawnableComponent))
class COMPONENTEX_API UDiscComponent : public UShapeComponent
{
	GENERATED_BODY()



protected:
	UPROPERTY()
	class USphereComponent* SphereCollider;

	UPROPERTY()
	class UBoxComponent* BoxCollider;


protected:
	/** Radius of the disc **/
	UPROPERTY(Category = "Shape", EditAnywhere, BlueprintReadOnly, export)
	float Radius;

	/** Height of the disc **/
	UPROPERTY(Category = "Shape", EditAnywhere, BlueprintReadOnly, export)
	float Height;



public:
	UDiscComponent();

	virtual void InitializeComponent() override;
	virtual void OnRegister() override;
	virtual void DestroyComponent(bool bPromoteChildren) override;

	void SetRadiusAndHeight(float Radius, float Height);



public:
	//~ Begin USceneComponent Interface
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ End USceneComponent Interface

	//~ Begin UShapeComponent Interface
	virtual void UpdateBodySetup() override;
	//~ End UShapeComponent Interface



protected:
	int32 CollisionFlags;
	FHitResult CollisionSweepResult;


	UFUNCTION()
	void OnOverlapBoxBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapSphereBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapBoxEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapSphereEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void CheckCollisionEvent(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, const FHitResult& SweepResult);
};
