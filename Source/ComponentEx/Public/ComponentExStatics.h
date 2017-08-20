#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ComponentExStatics.generated.h"



UCLASS()
class COMPONENTEX_API UComponentExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "PostProcessing", BlueprintCallable)
	static class UObject* GetBlendable(class APostProcessVolume* Volume, int Index);

	UFUNCTION(Category = "PostProcessing", BlueprintCallable)
	static bool UpdateBlendableWeight(class APostProcessVolume* Volume, int Index, float Weight);

	UFUNCTION(Category = "Spline", BlueprintPure)
	static float FindDistanceClosestToWorldLocation(class USplineComponent* Target, FVector WorldLocation);
};
