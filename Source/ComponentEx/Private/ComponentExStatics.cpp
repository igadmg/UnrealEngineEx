#include "ComponentExStatics.h"

#include "Animation/AnimationAsset.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/ActorPoolComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

#include "ComponentEx.final.h"


void UComponentExStatics::SetActorEnabled(AActor* Actor, bool bIsEnabled)
{
	ex(Actor).SetActorEnabled(bIsEnabled);
}

UCameraComponent* UComponentExStatics::GetCameraComponent(const AActor* Actor)
{
	return ex(Actor).GetCameraComponent();
}

UActorPoolComponent* UComponentExStatics::GetActorPool(const UObject* WorldContextObject)
{
	if (auto LevelScriptActor = GetLevelScriptActor(WorldContextObject))
	{
		auto Result = LevelScriptActor->FindComponentByClass<UActorPoolComponent>();
		if (!IsValid(Result))
		{
			Result = Cast<UActorPoolComponent>(LevelScriptActor->AddComponentByClass(UActorPoolComponent::StaticClass(), false, FTransform::Identity, false));
		}

		return Result;
	}

	return nullptr;
}

void UComponentExStatics::Attach(const FAttachmentDescription& Where, AActor* What, AActor* ParentActor, const FAttachmentTransformRules& AttachmentRules)
{
	if (Where.Component == nullptr)
	{
		What->AttachToActor(ParentActor, AttachmentRules, Where.SocketName);
	}
	else
	{
		What->AttachToComponent(Where.Component, AttachmentRules, Where.SocketName);
	}
}

ALevelScriptActor* UComponentExStatics::GetLevelScriptActor(const UObject* WorldContextObject, int32 LevelIndex)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	ULevel* Level = World->GetLevel(LevelIndex);
	if (!IsValid(Level))
		return nullptr;

	ALevelScriptActor* LevelScriptActor = Level->GetLevelScriptActor();
	if (!IsValid(LevelScriptActor))
		return nullptr;

	return LevelScriptActor;
}

ALevelScriptActor* UComponentExStatics::GetLevelScriptActorFromStreamingLevel(const UObject* WorldContextObject, ULevelStreaming* StreamingLevel)
{
	if (!IsValid(StreamingLevel))
		return nullptr;

	if (!StreamingLevel->IsLevelLoaded())
		return nullptr;

	ALevelScriptActor* LevelScriptActor = StreamingLevel->GetLevelScriptActor();
	if (!IsValid(LevelScriptActor))
		return nullptr;

	return LevelScriptActor;
}

FVector UComponentExStatics::GetHitOrEndLocation(const FHitResult& HitResult)
{
	return HitResult.bBlockingHit ? HitResult.Location : HitResult.TraceEnd;
}

FVector UComponentExStatics::GetTraceDirection(const FHitResult& HitResult)
{
	return (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
}

bool UComponentExStatics::RunBehaviorTree(AController* Controller, UBehaviorTree* BehaviorTreeAsset, UBehaviorTreeComponent* BehaviourTreeComponent, UBlackboardComponent* BlackboardComponent)
{
	if (!IsValid(BehaviorTreeAsset) || !IsValid(BehaviorTreeAsset->BlackboardAsset))
	{
		//UE_VLOG(this, LogBehaviorTree, Warning, TEXT("RunBehaviorTree: Unable to run NULL behavior tree"));
		return false;
	}

	// see if need a blackboard component at all
	if (BlackboardComponent == nullptr || !BlackboardComponent->IsCompatibleWith(BehaviorTreeAsset->BlackboardAsset))
	{
		if (!UseBlackboard(Controller, BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
			return false;
	}

	if (!IsValid(BlackboardComponent))
		return false;

	if (!IsValid(BehaviourTreeComponent))
	{
		BehaviourTreeComponent = Controller->FindComponentByClass<UBehaviorTreeComponent>();
		if (!IsValid(BehaviourTreeComponent))
		{
			//UE_VLOG(this, LogBehaviorTree, Log, TEXT("RunBehaviorTree: spawning BehaviorTreeComponent.."));

			BehaviourTreeComponent = NewObject<UBehaviorTreeComponent>(Controller, TEXT("BehaviourTreeComponent"));
			BehaviourTreeComponent->RegisterComponent();
		}
	}

	BehaviourTreeComponent->StartTree(*BehaviorTreeAsset, EBTExecutionMode::Looped);

	return true;
}

bool UComponentExStatics::InitializeBlackboard(AController* Controller, UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset)
{
	check(BlackboardComp.GetOwner() == Controller);

	if (!BlackboardComp.InitializeBlackboard(BlackboardAsset))
		return false;

	// find the "self" key and set it to our pawn
	const FBlackboard::FKey SelfKey = BlackboardAsset.GetKeyID(FBlackboard::KeySelf);
	if (SelfKey != FBlackboard::InvalidKey)
	{
		BlackboardComp.SetValue<UBlackboardKeyType_Object>(SelfKey, Controller->GetPawn());
	}

	return true;
}

bool UComponentExStatics::UseBlackboard(AController* Controller, UBlackboardData* BlackboardAsset, UBlackboardComponent*& BlackboardComponent)
{
	if (!IsValid(BlackboardAsset))
	{
		//UE_VLOG(this, LogBehaviorTree, Log, TEXT("UseBlackboard: trying to use NULL Blackboard asset. Ignoring"));
		return false;
	}

	if (!IsValid(BlackboardComponent))
	{
		BlackboardComponent = Controller->FindComponentByClass<UBlackboardComponent>();

		if (!IsValid(BlackboardComponent))
		{
			BlackboardComponent = NewObject<UBlackboardComponent>(Controller, TEXT("BlackboardComponent"));
			BlackboardComponent->RegisterComponent();
		}
	}

	if (BlackboardComponent->GetBlackboardAsset() == nullptr)
	{
		InitializeBlackboard(Controller, *BlackboardComponent, *BlackboardAsset);
	}
	else if (BlackboardComponent->GetBlackboardAsset() != BlackboardAsset)
	{
		// @todo this behavior should be opt-out-able.
		//UE_VLOG(this, LogBehaviorTree, Log, TEXT("UseBlackboard: requested blackboard %s while already has %s instantiated. Forcing new BB.")
		//	, *GetNameSafe(BlackboardAsset), *GetNameSafe(Blackboard->GetBlackboardAsset()));
		InitializeBlackboard(Controller, *BlackboardComponent, *BlackboardAsset);
	}

	return true;
}

UObject* UComponentExStatics::GetBlendable(APostProcessVolume* Volume, int Index)
{
	if (!IsValid(Volume))
		return nullptr;

	if (Volume->Settings.WeightedBlendables.Array.Num() < Index)
		return nullptr;

	return Volume->Settings.WeightedBlendables.Array[Index].Object;
}

bool UComponentExStatics::UpdateBlendableWeight(APostProcessVolume* Volume, int Index, float Weight)
{
	if (!IsValid(Volume))
		return false;

	if (Volume->Settings.WeightedBlendables.Array.Num() < Index)
		return false;

	Volume->Settings.WeightedBlendables.Array[Index].Weight = Weight;

	return true;
}

float UComponentExStatics::FindDistanceClosestToLocation(USplineComponent* Spline, FVector Location, ESplineCoordinateSpace::Type CoordinateSpace)
{
	const int DistanceSolverIterations = 16;

	const float ClosestInputKey = Spline->FindInputKeyClosestToWorldLocation(Location);
	const int32 PreviousPoint = FMath::TruncToInt(ClosestInputKey);

	// Lerp between the previous and the next spline points
	float Distance = Spline->GetDistanceAlongSplineAtSplinePoint(PreviousPoint);
	Distance += (ClosestInputKey - PreviousPoint) * (Spline->GetDistanceAlongSplineAtSplinePoint(PreviousPoint + 1) - Distance);

	// The linear approximation is not enough. So here is a kinda numerical approximation.
	for (int32 i = 0; i < DistanceSolverIterations; ++i)
	{
		const float InputKeyAtDistance = Spline->SplineCurves.ReparamTable.Eval(Distance, 0.0f);
		// The euclidean distance between the current calculated distance and the real closest point
		const float Delta = (Spline->GetLocationAtSplineInputKey(InputKeyAtDistance, CoordinateSpace) - Spline->GetLocationAtSplineInputKey(ClosestInputKey, CoordinateSpace)).Size();
		if (InputKeyAtDistance < ClosestInputKey)
		{
			Distance += Delta;
		}
		else if (InputKeyAtDistance > ClosestInputKey)
		{
			Distance -= Delta;
		}
		else
		{
			break;
		}
	}

	return FMath::Clamp(Distance, 0.0f, Spline->GetSplineLength());
}

FVector2D UComponentExStatics::GetTextureSize(UTexture* Texture)
{
	if (!IsValid(Texture))
		return FVector2D::ZeroVector;

	return FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
}

FVector UComponentExStatics::GetMeshSize(UStaticMesh* Mesh)
{
	if (!IsValid(Mesh))
		return FVector::ZeroVector;

	auto Bounds = Mesh->GetBounds();
	return Bounds.BoxExtent * 2.0f;
}

void UComponentExStatics::PlayAnimationWithCallback(USkeletalMeshComponent* SkeletalMeshComponent, UAnimationAsset* NewAnimToPlay, bool bLooping, const FTimerDynamicDelegate& OnFinished)
{
	if (!IsValid(SkeletalMeshComponent))
	{
		OnFinished.ExecuteIfBound();
		return;
	}

	if (!IsValid(NewAnimToPlay))
	{
		OnFinished.ExecuteIfBound();
		return;
	}

	float AnimationTime = 0;
	if (auto Montage = Valid<UAnimSequenceBase>(NewAnimToPlay))
	{
		AnimationTime = Montage->GetPlayLength() / Montage->RateScale;
	}

	SkeletalMeshComponent->PlayAnimation(NewAnimToPlay, bLooping);

	if (AnimationTime > 0)
	{
		FTimerHandle TimerHandle;
		SkeletalMeshComponent->GetWorld()->GetTimerManager().SetTimer(TimerHandle, OnFinished, AnimationTime, false);
	}
	else
	{
		OnFinished.ExecuteIfBound();
	}
}

bool UComponentExStatics::GetWorldBoneTransformAtTime(USkeletalMeshComponent* SkeletalMeshComponent, UAnimSequence* AnimSequence, FName BoneName, float Time, FTransform& OutTransform)
{
	if (!IsValid(SkeletalMeshComponent))
		return false;

	if (!IsValid(AnimSequence))
		return false;

	if (SkeletalMeshComponent->GetBoneIndex(BoneName) == INDEX_NONE)
		return false;

	USkeleton* Skeleton = AnimSequence->GetSkeleton();
	if (!IsValid(Skeleton))
		return false;

	TArray<FName> BoneStack;
	BoneStack.Add(BoneName);
	while (true)
	{
		FName ParentBoneName = SkeletalMeshComponent->GetParentBone(BoneStack.Last());
		if (ParentBoneName == NAME_None)
			break;
		BoneStack.Add(ParentBoneName);
	}

	for (const FName& LocalBoneName : BoneStack)
	{
		int32 TrackIndex =
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 23 || ENGINE_MAJOR_VERSION >= 5
			Skeleton->GetRawAnimationTrackIndex(SkeletalMeshComponent->GetBoneIndex(LocalBoneName), AnimSequence);
#elif ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 0
			Skeleton->GetAnimationTrackIndex(SkeletalMeshComponent->GetBoneIndex(LocalBoneName), AnimSequence, true);
#endif

		if (TrackIndex == INDEX_NONE)
			return false;

		FTransform BoneTransform;
		AnimSequence->GetBoneTransform(BoneTransform, TrackIndex, Time, true);
		OutTransform = OutTransform * BoneTransform;
	}

	return true;
}

bool UComponentExStatics::GetWorldSocketTransformAtTime(USkeletalMeshComponent* SkeletalMeshComponent, UAnimSequence* AnimSequence, FName SocketName, float Time, FTransform& OutTransform)
{
	if (!IsValid(SkeletalMeshComponent))
		return false;

	if (!IsValid(AnimSequence))
		return false;

	return GetWorldBoneTransformAtTime(SkeletalMeshComponent, AnimSequence, SkeletalMeshComponent->GetSocketBoneName(SocketName), Time, OutTransform);
}

void UComponentExStatics::SetupSplineMeshComponentFromSpline(USplineMeshComponent* SplineMeshComponent, USplineComponent* Spline, float StartDistance, float EndDistance, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh)
{
	if (!IsValid(SplineMeshComponent) || !IsValid(Spline))
		return;

	float UpVectorDistance = (StartDistance + EndDistance) / 2;
	auto UpDir = Spline->GetUpVectorAtDistanceAlongSpline(UpVectorDistance, CoordinateSpace);
	SplineMeshComponent->SetSplineUpDir(UpDir, false);

	auto IntervalDistance = (EndDistance - StartDistance);
	SetupSplineMeshComponentStartFromSpline(SplineMeshComponent, Spline, StartDistance, IntervalDistance, 0, CoordinateSpace, false);
	SetupSplineMeshComponentEndFromSpline(SplineMeshComponent, Spline, EndDistance, IntervalDistance, 0, CoordinateSpace, false);

	auto InverseUpVectorTransform = Spline->GetTransformAtDistanceAlongSpline(UpVectorDistance, CoordinateSpace).Inverse();
	auto StartRotation = Spline->GetRotationAtDistanceAlongSpline(StartDistance, CoordinateSpace);
	auto StartRoll = InverseUpVectorTransform.TransformRotation(StartRotation.Quaternion()).Rotator().Roll;
	SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(StartRoll), false);

	auto EndRotation = Spline->GetRotationAtDistanceAlongSpline(EndDistance, CoordinateSpace);
	auto EndRoll = InverseUpVectorTransform.TransformRotation(EndRotation.Quaternion()).Rotator().Roll;
	SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(EndRoll), false);

	if (bUpdateMesh)
	{
		SplineMeshComponent->UpdateMesh();
	}
}

void UComponentExStatics::SetupSplineMeshComponentStartFromSpline(USplineMeshComponent* SplineMeshComponent, USplineComponent* Spline, float DistanceAlongSpline, float TangetLength, float SplineMeshRoll, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh)
{
	if (!IsValid(SplineMeshComponent) || !IsValid(Spline))
		return;

	//auto StartRotation = Spline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, CoordinateSpace);
	//auto SplineTransform = SplineMeshComponent->GetUpVector();
	//SplineMeshComponent->SetStartRoll(FMath::DegreesToRadians(FRotator::NormalizeAxis(StartRotation.Roll - SplineMeshRoll)), false);

	auto StartPos = Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, CoordinateSpace);
	auto StartTangent = Spline->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, CoordinateSpace);

	StartTangent.Normalize();
	SplineMeshComponent->SetStartPosition(StartPos, false);
	SplineMeshComponent->SetStartTangent(StartTangent * TangetLength, false);

	if (bUpdateMesh)
	{
		SplineMeshComponent->UpdateMesh();
	}
}

void UComponentExStatics::SetupSplineMeshComponentEndFromSpline(USplineMeshComponent* SplineMeshComponent, USplineComponent* Spline, float DistanceAlongSpline, float TangetLength, float SplineMeshRoll, ESplineCoordinateSpace::Type CoordinateSpace, bool bUpdateMesh)
{
	if (!IsValid(SplineMeshComponent) || !IsValid(Spline))
		return;

	//auto EndRotation = Spline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, CoordinateSpace);
	//SplineMeshComponent->SetEndRoll(FMath::DegreesToRadians(FRotator::NormalizeAxis(EndRotation.Roll - SplineMeshRoll)), false);

	auto EndPos = Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, CoordinateSpace);
	auto EndTangent = Spline->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, CoordinateSpace);

	EndTangent.Normalize();
	SplineMeshComponent->SetEndPosition(EndPos, false);
	SplineMeshComponent->SetEndTangent(EndTangent * TangetLength, false);

	if (bUpdateMesh)
	{
		SplineMeshComponent->UpdateMesh();
	}
}
