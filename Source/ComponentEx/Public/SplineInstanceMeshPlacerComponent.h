#pragma once

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Interfaces/CookInterface.h"

#include "SplineInstanceMeshPlacerComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent = true))
class COMPONENTEX_API USplineInstanceMeshPlacerComponent
	: public UHierarchicalInstancedStaticMeshComponent
	, public ICookInterface
{
	GENERATED_BODY()


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;


public:
	UFUNCTION(BlueprintCallable)
	void FillSplineMesh(float Interval, FVector InstanceOffset);


public: // ICookInterface
	//virtual bool OnWorldLoaded() override;
	virtual bool OnWorldSaved() override;


public:
	USplineInstanceMeshPlacerComponent(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void OnHiddenInGameChanged();
};

