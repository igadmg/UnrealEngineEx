#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SynchronizedAnimSequence.generated.h"


/**
* This struct is used to store AnimSequence reference which later can be uase to Play it on a SkeletalMeshComponent
* It should be replicated to remote host and played from the same time - as there will be some lag the sequence would be
* revinded forward for the lag value, thus skipping some start.
*/
USTRUCT(BlueprintType)
struct UNREALENGINEEX_API FSynchronizedAnimSequence
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "SynchronizedAnimSequence", EditAnywhere, BlueprintReadOnly) /* Not Replicated */
	class UAnimSequenceBase* Animation = nullptr;

	UPROPERTY(Category = "SynchronizedAnimSequence", VisibleInstanceOnly, BlueprintReadOnly) /* Replicated */
	float PlayStartServerTime = 0.f;

	/** Play Rate. Negative means backward play. */
	UPROPERTY(Category = "SynchronizedAnimSequence", VisibleInstanceOnly, BlueprintReadOnly) /* Replicaterd */
	float PlayRate = 0.f;


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
	UFUNCTION(Category = "UnrealEditorEx|SynchronizedAnimSequence", BlueprintCallable)
	static void Play(class USkeletalMeshComponent* SkeletalMeshComponent, struct FSynchronizedAnimSequence& SynchronizedAnimSequence);
};


void FSynchronizedAnimSequence::Play(class USkeletalMeshComponent* SkeletalMeshComponent)
{
	USynchronizedAnimSequenceStatics::Play(SkeletalMeshComponent, *this);
}
