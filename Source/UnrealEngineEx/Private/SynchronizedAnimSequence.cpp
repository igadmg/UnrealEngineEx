#include "UnrealEngineExPrivatePCH.h"
#include "SynchronizedAnimSequence.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"

#include "UnrealEngineExStatics.h"



void USynchronizedAnimSequenceStatics::Play(USkeletalMeshComponent* SkeletalMeshComponent, FSynchronizedAnimSequence& SynchronizedAnimSequence)
{
	if (!IsValid(SkeletalMeshComponent))
		return;

	if (!IsValid(SynchronizedAnimSequence.Animation))
		return;

	float AbsPlayRate = FMath::Abs(SynchronizedAnimSequence.PlayRate);
	float AnimDuration = SynchronizedAnimSequence.Animation->SequenceLength * AbsPlayRate;
	float ServerTime = UUnrealEngineExStatics::GetServerWorldTimeSeconds(SkeletalMeshComponent);

	float DeltaTime = ServerTime - SynchronizedAnimSequence.PlayStartServerTime;

	SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SkeletalMeshComponent->SetAnimation(SynchronizedAnimSequence.Animation);
	if (DeltaTime > AnimDuration)
	{
		if (SynchronizedAnimSequence.PlayRate < 0)
		{
			SkeletalMeshComponent->SetPosition(0, false);
		}
		else
		{
			SkeletalMeshComponent->SetPosition(SynchronizedAnimSequence.Animation->SequenceLength, false);
		}
	}
	else
	{
		float PlayAnimDuration = AnimDuration - DeltaTime;
		float PlayRate = PlayAnimDuration / AnimDuration * SynchronizedAnimSequence.PlayRate;
		SkeletalMeshComponent->SetPlayRate(PlayRate);
		if (PlayRate < 0)
		{
			SkeletalMeshComponent->SetPosition(SynchronizedAnimSequence.Animation->SequenceLength);
		}
		SkeletalMeshComponent->Play(false);
	}
}
