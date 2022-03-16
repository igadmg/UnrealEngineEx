#include "SynchronizedAnimSequence.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"

#include "UnrealEngineExStatics.h"
#include "UnrealEngineEx.final.h"



void USynchronizedAnimSequenceStatics::Play(USkeletalMeshComponent* SkeletalMeshComponent, FSynchronizedAnimSequence& SynchronizedAnimSequence)
{
	if (!IsValid(SkeletalMeshComponent))
		return;

	if (!IsValid(SynchronizedAnimSequence.Animation))
		return;

	float AbsPlayRate = FMath::Abs(SynchronizedAnimSequence.PlayRate);
#if ENGINE_MAJOR_VERSION >= 5
	float AnimDuration = SynchronizedAnimSequence.Animation->GetPlayLength() * AbsPlayRate;
#else
	float AnimDuration = SynchronizedAnimSequence.Animation->SequenceLength * AbsPlayRate;
#endif
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
#if ENGINE_MAJOR_VERSION >= 5
			SkeletalMeshComponent->SetPosition(SynchronizedAnimSequence.Animation->GetPlayLength(), false);
#else
			SkeletalMeshComponent->SetPosition(SynchronizedAnimSequence.Animation->SequenceLength, false);
#endif
		}
	}
	else
	{
		float PlayAnimDuration = AnimDuration - DeltaTime;
		float PlayRate = PlayAnimDuration / AnimDuration * SynchronizedAnimSequence.PlayRate;
		SkeletalMeshComponent->SetPlayRate(PlayRate);
		if (PlayRate < 0)
		{
#if ENGINE_MAJOR_VERSION >= 5
			SkeletalMeshComponent->SetPosition(SynchronizedAnimSequence.Animation->GetPlayLength());
#else
			SkeletalMeshComponent->SetPosition(SynchronizedAnimSequence.Animation->SequenceLength);
#endif
		}
		SkeletalMeshComponent->Play(false);
	}
}
