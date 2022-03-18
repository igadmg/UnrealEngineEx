#include "Components/TraceComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "ComponentEx.final.h"


#define LOCTEXT_NAMESPACE "ComponentEx"

UTraceComponent::UTraceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraceComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if 0
	auto TraceStart = GetComponentLocation();
	auto TraceEnd = TraceStart + GetForwardVector() * TraceDistance;
	
	FHitResult OutSingleHit;
	HitResults.Empty();
	switch (TaceType)
	{
	case ETaceType::LineSingle:
		if (UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, TraceChannel, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, OutSingleHit, bIgnoreSelf))
		{
			HitResults.Add(OutSingleHit);
		}
		break;
	case ETaceType::LineMulti:
		UKismetSystemLibrary::LineTraceMulti(this, TraceStart, TraceEnd, TraceChannel, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, HitResults, bIgnoreSelf);
		break;
	case ETaceType::BoxSingle:
		break;
	case ETaceType::BoxMulti:
		break;
	case ETaceType::SphereSingle:
		break;
	case ETaceType::SphereMulti:
		break;
	case ETaceType::CapsuleSingle:
		break;
	case ETaceType::CapsuleMulti:
		break;
	default:
		break;
	}
#endif
}

bool UTraceComponent::LineTraceSingle(TEnumAsByte<ETraceTypeQuery> TraceChannel, TArray<AActor*> ActorsToIgnore, FHitResult& HitResult)
{
	auto TraceStart = GetComponentLocation();
	auto TraceEnd = TraceStart + GetForwardVector() * TraceDistance;

	if (auto Pawn = XX::GetPlayerPawn(this))
	{
		ActorsToIgnore.Add(Pawn);
		Pawn->GetAttachedActors(ActorsToIgnore, false, true);
	}

	return UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, TraceChannel, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, HitResult, bIgnoreSelf);
}

#undef LOCTEXT_NAMESPACE
