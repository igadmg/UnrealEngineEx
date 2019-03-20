#include "ComponentExPrivatePCH.h"
#include "ComponentExStatics.h"

#include "ComponentEx.h"
#include "Components/SplineComponent.h"
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