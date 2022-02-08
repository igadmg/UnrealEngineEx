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
	UPROPERTY(Category = "SplineInstanceMeshPlacerComponent", VisibleAnywhere, BlueprintReadWrite)
	class USplineComponent* SplineComponent;

	UPROPERTY(Category = "SplineInstanceMeshPlacerComponent", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;


public:
	UFUNCTION(Category = "UnrealEngineEx: SplineInstanceMeshPlacerComponent", BlueprintCallable)
	void FillSplineMesh(float Interval, FVector InstanceOffset);


public: // ICookInterface
	//virtual bool OnWorldLoaded() override;
	virtual bool OnWorldSaved() override;


public:
	USplineInstanceMeshPlacerComponent(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void OnHiddenInGameChanged();
};

