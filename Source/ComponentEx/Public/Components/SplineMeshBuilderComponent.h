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
	class UMaterialInterface* Material;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	bool bGenerateOverlapEvents = false;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	FBodyInstance CollisionPresets;

	UPROPERTY(Category = "SplineMeshBuilderComponent", EditAnywhere, BlueprintReadWrite)
	float SplineMeshCullDistance = 15000.f;

	UPROPERTY(Category = "Internal", BlueprintReadOnly)
	TArray<class USplineMeshComponent*> SplineMeshComponents;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadOnly)
	bool bDebugKeepComponentInstances = false;
#endif


public:
	UFUNCTION(Category = "UnrealEngineEx: SplineMeshBuilderComponent", BlueprintCallable)
	void BuildSplineMesh();

	UFUNCTION(Category = "UnrealEngineEx: SplineMeshBuilderComponent", BlueprintCallable)
	void SetCollisionEnabled(ECollisionEnabled::Type NewType);


public: // ICookInterface
	virtual bool OnWorldLoaded() override;
	virtual bool OnWorldSaved() override;


public:
	USplineMeshBuilderComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void OnHiddenInGameChanged();
};
