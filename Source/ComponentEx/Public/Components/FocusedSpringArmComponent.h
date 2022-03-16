#pragma once

#include "GameFramework/SpringArmComponent.h"

#include "FocusedSpringArmComponent.generated.h"


UCLASS()
class COMPONENTEX_API UFocusedSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()


public:
	/** Focal point where camera should be rotated and look at. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector FocusOffset;


public:
	UFocusedSpringArmComponent(const FObjectInitializer& ObjectInitializer);


public:
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;
};
