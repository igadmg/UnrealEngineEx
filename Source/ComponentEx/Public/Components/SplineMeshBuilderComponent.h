#pragma once

#include "Components/SceneComponent.h"
#include "Interfaces/CookInterface.h"

#include "SplineMeshBuilderComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent = true))
class COMPONENTEX_API USplineMeshBuilderComponent
	: public USceneComponent
	, public ICookInterface
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "SplineMeshBuilderComponent", VisibleAnywhere, BlueprintReadWrite)
	class USplineComponent* SplineComponent;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* Mesh;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	class UMaterial* Material;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	bool bGenerateOverlapEvents = false;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	FBodyInstance CollisionPresets;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	float SplineMeshCullDistance = 15000.f;

	UPROPERTY(Category = "Internal", BlueprintReadOnly)
	TArray<class USplineMeshComponent*> SplineMeshComponents;


public:
	UFUNCTION(Category = "UnrealEngineEx: SplineMeshBuilderComponent", BlueprintCallable)
	void BuildSplineMesh();

	UFUNCTION(Category = "UnrealEngineEx: SplineMeshBuilderComponent", BlueprintCallable)
	void SetCollisionEnabled(ECollisionEnabled::Type NewType);


public: // ICookInterface
	virtual bool OnWorldLoaded() override;
	virtual bool OnWorldSaved() override;


public:
	USplineMeshBuilderComponent(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void OnHiddenInGameChanged();
};
