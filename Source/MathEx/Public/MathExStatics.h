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
};
