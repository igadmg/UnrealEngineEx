#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SynchronizedAnimSequence.generated.h"



USTRUCT(BlueprintType)
struct UNREALENGINEEX_API FSynchronizedAnimSequence
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) /* Not Replicated */
	class UAnimSequenceBase* Animation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly) /* Replicated */
	float PlayStartServerTime;

	/** Play Rate. Negative means backward paly. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly) /* Replicaterd */
	float PlayRate;


public:
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << PlayStartServerTime;
		Ar << PlayRate;

		bOutSuccess = true;
		return true;
	}


public:
	FORCEINLINE void Play(class USkeletalMeshComponent* SkeletalMeshComponent);
};

template<>
struct TStructOpsTypeTraits<FSynchronizedAnimSequence> : public TStructOpsTypeTraitsBase2<FSynchronizedAnimSequence>
{
	enum
	{
		WithNetSerializer = true,
	};
};


UCLASS()
class UNREALENGINEEX_API USynchronizedAnimSequenceStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	static void Play(class USkeletalMeshComponent* SkeletalMeshComponent, struct FSynchronizedAnimSequence& SynchronizedAnimSequence);
};


void FSynchronizedAnimSequence::Play(class USkeletalMeshComponent* SkeletalMeshComponent)
{
	USynchronizedAnimSequenceStatics::Play(SkeletalMeshComponent, *this);
}
