#pragma once

#include "SynchronizedNetworkClock.generated.h"



DECLARE_DELEGATE_TwoParams(FSynchronizedNetworkClockDelegate, float, float);
DECLARE_DELEGATE_OneParam(FSynchronizedNetworkClockSynchronizedDelegate, float);



struct FNetworkTimeSample
{
	float Offset;
	float Roundtrip;
};

UCLASS(Transient)
class UNREALENGINEEX_API USynchronizedNetworkClock : public UActorComponent
{
	GENERATED_BODY()

	friend class FSynchronizeNetworkClockAction;


public:
	UPROPERTY(Category = "Synchronized Network Clock", EditAnywhere, BlueprintReadWrite)
	float CustomOffset = 0.f;


public:
	UFUNCTION(Category = "UnrealEngineEx|Synchronized Network Clock", BlueprintPure)
	float GetTimeSeconds() const;

	UFUNCTION(Category = "UnrealEngineEx|Synchronized Network Clock", BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo"))
	void SynchronizeClock(int32 NumberOfSamples, FLatentActionInfo LatentInfo);



protected:
	UFUNCTION(Category = "UnrealEditorEx|Synchronized Network Clock", server, unreliable, WithValidation) void SynchronizeTimeServer(int32 InToken);
	UFUNCTION(Category = "UnrealEditorEx|Synchronized Network Clock", client, unreliable) void SynchronizeTimeClient(int32 InToken, float InServerTime);
	void DoSynchronizeTime();

	FSynchronizedNetworkClockDelegate OnSynchronizedNetworkClockCallback;
	void FinishSynchronization(const TArray<FNetworkTimeSample>& Samples);



public:
	USynchronizedNetworkClock(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	FSynchronizedNetworkClockSynchronizedDelegate OnSynchronized;


protected:
	int32 Token;
	float ClientATime;
	float ClientBTime;

	float ClientOffset = 0;
};
