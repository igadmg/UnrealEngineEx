#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "MathExStatics.generated.h"


UENUM(BlueprintType)
enum class ETriangleVertexOrder : uint8
{
	Clockwise,
	CounterClockwise,
};

UCLASS()
class MATHEX_API UMathExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "MathEx", BlueprintPure)
	static bool AreVertexOrdered(FVector A, FVector B, FVector C, ETriangleVertexOrder Order = ETriangleVertexOrder::Clockwise);

	UFUNCTION(Category = "MathEx", BlueprintPure)
	static float AngleBetweenVectors(FVector A, FVector B, FVector O = FVector::ZeroVector, ETriangleVertexOrder Order = ETriangleVertexOrder::Clockwise);

	//UFUNCTION(Category = "MathEx", BlueprintPure)
	//static FVector GetPerpendicularTo(FVector A, FVector B, FVector O);

	UFUNCTION(Category = "MathEx", BlueprintPure)
	static float RandomSameDigitsNumber(float Value);

	UFUNCTION(Category = "MathEx", BlueprintPure)
	static FVector SphereCenterByTwoPointsAndRadiusVector(FVector A, FVector B, FVector R);

	UFUNCTION(Category = "MathEx", BlueprintCallable)
	static bool ProjectWorldToScreenBidirectional(class APlayerController* PlayerController, FVector WorldLocation, FVector& ScreenLocation, bool& OutTargetBehindCamera);

	UFUNCTION(Category = "MathEx", BlueprintCallable)
	static bool ProjectWorldToScreen(class APlayerController* PlayerController, FVector WorldLocation, FVector& ScreenLocation, bool& OutTargetBehindCamera);

	UFUNCTION(Category = "MathEx", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool SuggestProjectileVelocityForTime(const UObject* WorldContextObject, FVector& OutVelocity, FVector Start, FVector End, float Time, float OverrideGravityZ = 0);
};
