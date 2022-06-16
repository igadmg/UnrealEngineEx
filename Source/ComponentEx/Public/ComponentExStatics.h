#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Components/ActorPoolComponent.h"
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

	UFUNCTION(Category = "ComponentEx", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class UActorPoolComponent* GetActorPool(const UObject* WorldContextObject);

	//UFUNCTION(Category = "ComponentEx", BlueprintCallable)
	static void Attach(const FAttachmentDescription& Where, class AActor* What, class AActor* ParentActor, const FAttachmentTransformRules& AttachmentRules);


	UFUNCTION(Category = "ComponentEx|Level", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class ALevelScriptActor* GetLevelScriptActor(const UObject* WorldContextObject, int32 LevelIndex = 0);

	UFUNCTION(Category = "ComponentEx|Level", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class ALevelScriptActor* GetLevelScriptActorFromStreamingLevel(const UObject* WorldContextObject, class ULevelStreaming* StreamingLevel);


	UFUNCTION(Category = "ComponentEx|Collision", BlueprintPure)
	static FVector GetHitOrEndLocation(const FHitResult& HitResult);

	UFUNCTION(Category = "ComponentEx|Collision", BlueprintPure)
	static FVector GetTraceDirection(const FHitResult& HitResult);


	UFUNCTION(Category = "ComponentEx|Controller", BlueprintCallable)
	static bool RunBehaviorTree(class AController* Controller, class UBehaviorTree* BehaviorTreeAsset, class UBehaviorTreeComponent* BehaviourTreeComponent = nullptr, class UBlackboardComponent* BlackboardComponent = nullptr);

	static bool InitializeBlackboard(class AController* Controller, class UBlackboardComponent& BlackboardComp, class UBlackboardData& BlackboardAsset);

	UFUNCTION(Category = "ComponentEx|Controller", BlueprintCallable)
	static bool UseBlackboard(class AController* Controller, class UBlackboardData* BlackboardAsset, class UBlackboardComponent*& BlackboardComponent);


	UFUNCTION(Category = "ComponentEx|PostProcessing", BlueprintCallable)
	static class UObject* GetBlendable(class APostProcessVolume* Volume, int Index);

	UFUNCTION(Category = "ComponentEx|PostProcessing", BlueprintCallable)
	static bool UpdateBlendableWeight(class APostProcessVolume* Volume, int Index, float Weight);

	UFUNCTION(Category = "ComponentEx|Spline", BlueprintPure)
	static float FindDistanceClosestToLocation(class USplineComponent* Spline, FVector Location, ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	UFUNCTION(Category = "ComponentEx|Rendering|Texture", BlueprintPure)
	static FVector2D GetTextureSize(class UTexture* Texture);

	UFUNCTION(Category = "ComponentEx|StaticMesh", BlueprintPure)
	static FVector GetMeshSize(class UStaticMesh* Mesh);


	UFUNCTION(Category = "ComponentEx|Animation", BlueprintCallable)
	static void PlayAnimationWithCallback(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimationAsset* NewAnimToPlay, bool bLooping, const FTimerDynamicDelegate& OnFinished);

	UFUNCTION(Category = "ComponentEx|Animation", BlueprintPure)
	static bool GetWorldBoneTransformAtTime(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimSequence* AnimSequence, FName BoneName, float Time, FTransform& OutTransform);

	UFUNCTION(Category = "ComponentEx|Animation", BlueprintPure)
	static bool GetWorldSocketTransformAtTime(class USkeletalMeshComponent* SkeletalMeshComponent, class UAnimSequence* AnimSequence, FName SocketName, float Time, FTransform& OutTransform);


	UFUNCTION(Category = "ComponentEx|Spline", BlueprintCallable)
	static void SetupSplineMeshComponentFromSpline(class USplineMeshComponent* SplineMeshComponent, class USplineComponent* Spline, float StartDistance, float EndDistance, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh = true);

	UFUNCTION(Category = "ComponentEx|Spline", BlueprintCallable)
	static void SetupSplineMeshComponentStartFromSpline(class USplineMeshComponent* SplineMeshComponent, class USplineComponent* Spline, float DistanceAlongSpline, float TangetLength, float SplineMeshRoll, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh = true);

	UFUNCTION(Category = "ComponentEx|Spline", BlueprintCallable)
	static void SetupSplineMeshComponentEndFromSpline(class USplineMeshComponent* SplineMeshComponent, class USplineComponent* Spline, float DistanceAlongSpline, float TangetLength, float SplineMeshRoll, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh = true);
};


namespace XX
{
	static UActorPoolComponent* GetActorPool(const UObject* WorldContextObject)
	{
		return UComponentExStatics::GetActorPool(WorldContextObject);
	}

	static bool DestroyActor(AActor* Actor)
	{
		if (!IsValid(Actor))
			return false;

		if (auto ActorPool = GetActorPool(Actor))
		{
			return ActorPool->DestroyActor(Actor);
		}
		if (auto World = Actor->GetWorld())
		{
			return World->DestroyActor(Actor);
		}

		return false;
	}

	template <typename TActor = ALevelScriptActor>
	TActor* GetLevelScriptActor(const UObject* Object, int32 LevelIndex = 0)
	{
		return TValid<TActor, ALevelScriptActor>::Valid(UComponentExStatics::GetLevelScriptActor(Object, LevelIndex));
	}

	template <typename TActor = ALevelScriptActor>
	TActor* GetLevelScriptActorFromStreamingLevel(const UObject* Object, ULevelStreaming* StreamingLevel)
	{
		return TValid<TActor, ALevelScriptActor>::Valid(UComponentExStatics::GetLevelScriptActorFromStreamingLevel(Object, StreamingLevel));
	}
}
