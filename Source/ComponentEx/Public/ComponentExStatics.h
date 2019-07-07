#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ComponentExStatics.generated.h"



UCLASS()
class COMPONENTEX_API UComponentExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "ComponentEx", BlueprintCallable)
	static void SetActorEnabled(class AActor* Actor, bool bIsEnabled);
 

	UFUNCTION(Category = "PostProcessing", BlueprintCallable)
	static class UObject* GetBlendable(class APostProcessVolume* Volume, int Index);

	UFUNCTION(Category = "PostProcessing", BlueprintCallable)
	static bool UpdateBlendableWeight(class APostProcessVolume* Volume, int Index, float Weight);

	UFUNCTION(Category = "Spline", BlueprintPure)
	static float FindDistanceClosestToWorldLocation(class USplineComponent* Target, FVector WorldLocation);

	UFUNCTION(Category = "Rendering|Texture", BlueprintPure)
	static FVector2D GetSize(class UTexture* Texture);


	UFUNCTION(Category = "Animation", BlueprintPure)
	static bool GetWorldBoneTransformAtTime(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimSequence* AnimSequence, FName BoneName, float Time, FTransform& OutTransform);

	UFUNCTION(Category = "Animation", BlueprintPure)
	static bool GetWorldSocketTransformAtTime(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimSequence* AnimSequence, FName SocketName, float Time, FTransform& OutTransform);
};
