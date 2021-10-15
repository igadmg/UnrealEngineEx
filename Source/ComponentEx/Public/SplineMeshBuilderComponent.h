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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SplineMeshCullDistance = 15000.f;

	UPROPERTY(Category = "Internal", BlueprintReadOnly)
	TArray<class USplineMeshComponent*> SplineMeshComponents;


public:
	UFUNCTION(BlueprintCallable)
	void BuildSplineMesh();

	UFUNCTION(BlueprintCallable)
	void SetCollisionEnabled(ECollisionEnabled::Type NewType);


public: // ICookInterface
	virtual bool OnWorldLoaded() override;
	virtual bool OnWorldSaved() override;


public:
	USplineMeshBuilderComponent(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void OnHiddenInGameChanged();
};
