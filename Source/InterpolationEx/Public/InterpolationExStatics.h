#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterpolationExStatics.generated.h"



UCLASS()
class INTERPOLATIONEX_API UInterpolationExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static bool IsFinished(class UInterpolationTimer* Timer);


	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolationTimer* MakeInterpolationTimer(float Duration, bool Cyclic = false);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorFloat* MakeFloatLinearInterpolator(float Start, float End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorVector2D* MakeVector2DLinearInterpolator(FVector2D Start, FVector2D End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorVector* MakeVectorLinearInterpolator(FVector Start, FVector End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorRotator* MakeRotatorLinearInterpolator(FRotator Start, FRotator End, bool bShortestPath);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorColor* MakeColorLinearInterpolator(FLinearColor Start, FLinearColor End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorPostProcessSettings* MakePostProcessSettingsLinearInterpolator(FPostProcessSettings Start, FPostProcessSettings End);


	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolationTimer* ReuseInterpolationTimer(UInterpolationTimer* Object, float Duration, bool Cyclic = false);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorFloat* ReuseFloatLinearInterpolator(UInterpolatorFloat* Object, float Start, float End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorVector2D* ReuseVector2DLinearInterpolator(UInterpolatorVector2D* Object, FVector2D Start, FVector2D End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorVector* ReuseVectorLinearInterpolator(UInterpolatorVector* Object, FVector Start, FVector End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorRotator* ReuseRotatorLinearInterpolator(UInterpolatorRotator* Object, FRotator Start, FRotator End, bool bShortestPath);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorColor* ReuseColorLinearInterpolator(UInterpolatorColor* Object, FLinearColor Start, FLinearColor End);

	UFUNCTION(Category = "Interpolation", BlueprintPure)
	static class UInterpolatorPostProcessSettings* ReusePostProcessSettingsLinearInterpolator(UInterpolatorPostProcessSettings* Object, FPostProcessSettings Start, FPostProcessSettings End);
};
