#pragma once

#include "GameFramework/SpringArmComponent.h"

#include "FocusedSpringArmComponent.generated.h"


UCLASS()
class COMPONENTEX_API UFocusedSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()


public:
	/** Focal point where camera should be rotated and look at. */
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	FVector FocusOffset = FVector(2000.f, 0.f, 0.f);


public:
	UFUNCTION(BlueprintPure)
	FVector GetFocusLocation() const;

	UFUNCTION(BlueprintPure)
	FRotator GetFocusRotation() const;


public:
	UFocusedSpringArmComponent(const FObjectInitializer& ObjectInitializer);


public:
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;


protected:
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;


protected:
	FRotator PreviousFocusRotation;
};
