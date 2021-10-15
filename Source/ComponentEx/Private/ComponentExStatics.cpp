#include "ComponentExStatics.h"

#include "ComponentEx.h"
#include "ActorEx.h"

#include "Animation/AnimationAsset.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

#include "CoreEx.h"



void UComponentExStatics::SetActorEnabled(AActor* Actor, bool bIsEnabled)
{
	FComponentEx::SetActorEnabled(Actor, bIsEnabled);
}

UCameraComponent* UComponentExStatics::GetCameraComponent(const AActor* Actor)
{
	return FActorEx(Actor).GetCameraComponent();
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
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 23
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
