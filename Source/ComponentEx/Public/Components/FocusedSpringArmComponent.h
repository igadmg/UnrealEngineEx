#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Interpolator.h"

#include "FocusedSpringArmComponent.generated.h"


USTRUCT(BlueprintType)
struct COMPONENTEX_API FFocusedSpringArmConfig
{
	GENERATED_BODY()


	/** Natural length of the spring arm when there are no collisions */
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float TargetArmLength = 0.f;

	/** offset at end of spring arm; use this instead of the relative offset of the attached component to ensure the line trace works as desired */
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	FVector SocketOffset = FVector::ZeroVector;

	/** Offset at start of spring, applied in world space. Use this if you want a world-space offset from the parent component instead of the usual relative-space offset. */
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	FVector TargetOffset = FVector::ZeroVector;

	/** Focal point where camera should be rotated and look at. */
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	FVector FocusOffset = FVector(2000.f, 0.f, 0.f);


	static FFocusedSpringArmConfig Make(float InTargetArmLength, FVector InSocketOffset, FVector InTargetOffset, FVector InFocusOffset)
	{
		FFocusedSpringArmConfig Result;
		Result.TargetArmLength = InTargetArmLength;
		Result.SocketOffset = InSocketOffset;
		Result.TargetOffset = InTargetOffset;
		Result.FocusOffset = InFocusOffset;
		return Result;
	}
	static FFocusedSpringArmConfig Make(class UFocusedSpringArmComponent* ArmComponent);
};

namespace XX
{
	template <> struct TLerp<FFocusedSpringArmConfig>
	{
		static FFocusedSpringArmConfig Lerp(const FFocusedSpringArmConfig& A, const FFocusedSpringArmConfig& B, float V)
		{
			return FFocusedSpringArmConfig::Make(
				XX::Lerp(A.TargetArmLength, B.TargetArmLength, V)
				, XX::Lerp(A.SocketOffset, B.SocketOffset, V)
				, XX::Lerp(A.TargetOffset, B.TargetOffset, V)
				, XX::Lerp(A.FocusOffset, B.FocusOffset, V)
			);
		}
	};
}


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

	UFUNCTION(BlueprintCallable)
	FFocusedSpringArmConfig ChangeCameraConfig(FFocusedSpringArmConfig SpringArmConfig, float Time, TEnumAsByte<EEasingFunc::Type> EasingFunc);


public:
	UFocusedSpringArmComponent(const FObjectInitializer& ObjectInitializer);


public:
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;


protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;


protected:
	FRotator PreviousFocusRotation;
	TLerpInterpolationTimer<FFocusedSpringArmConfig> ChangeCameraConfigInterpolationTimer;
};

inline FFocusedSpringArmConfig FFocusedSpringArmConfig::Make(UFocusedSpringArmComponent* ArmComponent)
{
	return FFocusedSpringArmConfig::Make(
		ArmComponent->TargetArmLength
		, ArmComponent->SocketOffset
		, ArmComponent->TargetOffset
		, ArmComponent->FocusOffset);
}
