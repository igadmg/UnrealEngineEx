#include "ComponentExPrivatePCH.h"
#include "ComponentExStatics.h"

#include "ComponentEx.h"
#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/Texture.h"



void UComponentExStatics::SetActorEnabled(AActor* Actor, bool bIsEnabled)
{
	FComponentEx::SetActorEnabled(Actor, bIsEnabled);
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

float UComponentExStatics::FindDistanceClosestToWorldLocation(USplineComponent* Target, FVector WorldLocation)
{
	return 0;
}

FVector2D UComponentExStatics::GetSize(UTexture* Texture)
{
	if (!IsValid(Texture))
		return FVector2D::ZeroVector;

	return FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
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
