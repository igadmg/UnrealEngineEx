#include "SynchronizedNetworkClock.h"

#include "LatentActions.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/NetworkObjectList.h"
#include "Net/UnrealNetwork.h"

#include "UnrealEngineEx.final.h"


USynchronizedNetworkClock::USynchronizedNetworkClock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}
/*
void USynchronizedNetworkClock::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USynchronizedNetworkClock, ServerTime);
}
*/
bool USynchronizedNetworkClock::SynchronizeTimeServer_Validate(int32 InToken)
{
	return true;
}

void USynchronizedNetworkClock::SynchronizeTimeServer_Implementation(int32 InToken)
{
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	if (IsValid(NetDriver))
	{
		FNetworkObjectInfo* NetworkObjectInfo = NetDriver->FindNetworkObjectInfo(GetOwner());
		UE_LOG(LogUnrealEngineEx, Log, TEXT("NetworkObjectInfo: %f"), NetworkObjectInfo->NextUpdateTime);
	}

	SynchronizeTimeClient(InToken, UGameplayStatics::GetTimeSeconds(this));
}

void USynchronizedNetworkClock::SynchronizeTimeClient_Implementation(int32 InToken, float InServerTime)
{
	if (Token != InToken)
		return;

	ClientBTime = UGameplayStatics::GetTimeSeconds(this);

	float RoundTrip = ClientBTime - ClientATime;
	float Offset = InServerTime - ClientATime + RoundTrip / 2.f;

	OnSynchronizedNetworkClockCallback.ExecuteIfBound(Offset, RoundTrip);
}

void USynchronizedNetworkClock::DoSynchronizeTime()
{
	Token = FMath::Rand();
	ClientATime = UGameplayStatics::GetTimeSeconds(this);
	ClientBTime = ClientATime;

	SynchronizeTimeServer(Token);
}

void USynchronizedNetworkClock::FinishSynchronization(const TArray<FNetworkTimeSample>& Samples)
{
	float SumOffset = 0;
	float SumRoundtrip = 0;

	for (const FNetworkTimeSample& Sample : Samples)
	{
		SumOffset += Sample.Offset;
		SumRoundtrip += Sample.Roundtrip;
	}

	SumOffset /= Samples.Num();
	SumRoundtrip /= Samples.Num();

	ClientOffset = SumOffset + CustomOffset;

	OnSynchronized.ExecuteIfBound(ClientOffset);
}

// Stream Level Action
class FSynchronizeNetworkClockAction : public FPendingLatentAction
{
public:
	USynchronizedNetworkClock* Clock;
	int32 NumberOfSamples;
	FLatentActionInfo LatentInfo;

	bool bIsBusy = false;
	float RequestTime;
	TArray<FNetworkTimeSample> Samples;



	FSynchronizeNetworkClockAction(USynchronizedNetworkClock* InClock, int32 InNumberOfSamples, const FLatentActionInfo& InLatentInfo)
		: Clock(InClock)
		, NumberOfSamples(InNumberOfSamples)
		, LatentInfo(InLatentInfo)
	{
		Clock->OnSynchronizedNetworkClockCallback.BindRaw(this, &FSynchronizeNetworkClockAction::OnSynchronizedNetworkClockCallback);
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if (!bIsBusy)
		{
			if (Samples.Num() < NumberOfSamples)
			{
				bIsBusy = true;
				RequestTime = UGameplayStatics::GetTimeSeconds(Clock);
				Clock->DoSynchronizeTime();
			}
			else
			{
				Clock->OnSynchronizedNetworkClockCallback.Unbind();
				Clock->FinishSynchronization(Samples);
				Response.FinishAndTriggerIf(true, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
			}
		}
		else if (UGameplayStatics::GetTimeSeconds(Clock) - RequestTime > 5.f)
		{
			bIsBusy = false;
		}
	}

	void OnSynchronizedNetworkClockCallback(float Offset, float Roundtrip)
	{
		Samples.Add(FNetworkTimeSample{ Offset, Roundtrip });
		bIsBusy = false;
	}
};

float USynchronizedNetworkClock::GetTimeSeconds() const
{
	return UGameplayStatics::GetTimeSeconds(this) + ClientOffset;
}

void USynchronizedNetworkClock::SynchronizeClock(int32 NumberOfSamples, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return;

	if (GetOwner()->HasAuthority())
	{
		ClientOffset = 0;
		FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	if (LatentManager.FindExistingAction<FSynchronizeNetworkClockAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
	{
		FSynchronizeNetworkClockAction* NewAction = new FSynchronizeNetworkClockAction(this, NumberOfSamples, LatentInfo);
		LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
	}
}
