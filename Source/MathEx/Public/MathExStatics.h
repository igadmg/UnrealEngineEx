#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathExStatics.generated.h"



UCLASS()
class MATHEX_API UMathExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "MathEx", BlueprintPure)
	static float RandomSameDigitsNumber(float Value);
};
