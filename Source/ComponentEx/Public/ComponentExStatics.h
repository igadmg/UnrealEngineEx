#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Components/SplineComponent.h"
#include "AttachmentDescription.h"

#include "ComponentExStatics.generated.h"


UCLASS()
class COMPONENTEX_API UComponentExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "ComponentEx", BlueprintCallable)
	static void SetActorEnabled(class AActor* Actor, bool bIsEnabled);

	UFUNCTION(Category = "ComponentEx", BlueprintPure)
	static class UCameraComponent* GetCameraComponent(const class AActor* Actor);

	//UFUNCTION(Category = "ComponentEx", BlueprintCallable)
	static void Attach(const FAttachmentDescription& Where, class AActor* What, class AActor* ParentActor, const FAttachmentTransformRules& AttachmentRules);


	UFUNCTION(Category = "ComponentEx: Controller", BlueprintCallable)
	static bool RunBehaviorTree(class AController* Controller, class UBehaviorTree* BehaviorTreeAsset, class UBehaviorTreeComponent* BehaviourTreeComponent = nullptr, class UBlackboardComponent* BlackboardComponent = nullptr);

	static bool InitializeBlackboard(class AController* Controller, class UBlackboardComponent& BlackboardComp, class UBlackboardData& BlackboardAsset);

	UFUNCTION(Category = "ComponentEx: Controller", BlueprintCallable)
	static bool UseBlackboard(class AController* Controller, class UBlackboardData* BlackboardAsset, class UBlackboardComponent*& BlackboardComponent);


	UFUNCTION(Category = "ComponentEx: PostProcessing", BlueprintCallable)
	static class UObject* GetBlendable(class APostProcessVolume* Volume, int Index);

	UFUNCTION(Category = "ComponentEx: PostProcessing", BlueprintCallable)
	static bool UpdateBlendableWeight(class APostProcessVolume* Volume, int Index, float Weight);

	UFUNCTION(Category = "ComponentEx: Spline", BlueprintPure)
	static float FindDistanceClosestToLocation(class USplineComponent* Spline, FVector Location, ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	UFUNCTION(Category = "ComponentEx: Rendering|Texture", BlueprintPure)
	static FVector2D GetTextureSize(class UTexture* Texture);

	UFUNCTION(Category = "ComponentEx: StaticMesh", BlueprintPure)
	static FVector GetMeshSize(class UStaticMesh* Mesh);


	UFUNCTION(Category = "ComponentEx: Animation", BlueprintCallable)
	static void PlayAnimationWithCallback(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimationAsset* NewAnimToPlay, bool bLooping, const FTimerDynamicDelegate& OnFinished);

	UFUNCTION(Category = "ComponentEx: Animation", BlueprintPure)
	static bool GetWorldBoneTransformAtTime(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimSequence* AnimSequence, FName BoneName, float Time, FTransform& OutTransform);

	UFUNCTION(Category = "ComponentEx: Animation", BlueprintPure)
	static bool GetWorldSocketTransformAtTime(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimSequence* AnimSequence, FName SocketName, float Time, FTransform& OutTransform);


	UFUNCTION(Category = "ComponentEx: Spline", BlueprintCallable)
	static void SetupSplineMeshComponentFromSpline(class USplineMeshComponent* SplineMeshComponent, class USplineComponent* Spline, float StartDistance, float EndDistance, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh = true);

	UFUNCTION(Category = "ComponentEx: Spline", BlueprintCallable)
	static void SetupSplineMeshComponentStartFromSpline(class USplineMeshComponent* SplineMeshComponent, class USplineComponent* Spline, float DistanceAlongSpline, float TangetLength, float SplineMeshRoll, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh = true);

	UFUNCTION(Category = "ComponentEx: Spline", BlueprintCallable)
	static void SetupSplineMeshComponentEndFromSpline(class USplineMeshComponent* SplineMeshComponent, class USplineComponent* Spline, float DistanceAlongSpline, float TangetLength, float SplineMeshRoll, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh = true);
};
