#include "BlueprintTriggerVolume.h"

#include "IsValidEx.h"



ABlueprintTriggerVolume::ABlueprintTriggerVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void ABlueprintTriggerVolume::RebuildBlueprintObject()
{
	if (IsValid(BlueprintObject))
	{
		BlueprintObject->Destroy();
		BlueprintObject = nullptr;
	}

	if (IsValid(BlueprintClass))
	{
		if (BlueprintClass->IsChildOf<AActor>())
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			//SpawnInfo.
			BlueprintObject = GetWorld()->SpawnActor<AActor>(BlueprintClass, SpawnInfo);
			BlueprintObject->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}
}

void ABlueprintTriggerVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static FName NAME_BlueprintClass(TEXT("BlueprintClass"));
	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == NAME_BlueprintClass)
	{
		RebuildBlueprintObject();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif // WITH_EDITOR

void ABlueprintTriggerVolume::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (FUnrealEngineEx::DoesImplementInterface<UBlueprintTriggerVolumeInterface>(BlueprintObject))
	{
		OnActorBeginOverlap.AddDynamic(this, &ABlueprintTriggerVolume::OnActorBeginOverlapVolume);
		OnActorEndOverlap.AddDynamic(this, &ABlueprintTriggerVolume::OnActorEndOverlapVolume);
	}
}

void ABlueprintTriggerVolume::BeginDestroy()
{
	if (IsValid(BlueprintObject))
	{
		BlueprintObject->Destroy();
		BlueprintObject = nullptr;
	}

	Super::BeginDestroy();
}

void ABlueprintTriggerVolume::OnActorBeginOverlapVolume(AActor* OverlappedActor, AActor* OtherActor)
{
	IBlueprintTriggerVolumeInterface::Execute_OnActorBeginOverlapVolume(BlueprintObject, OverlappedActor, OtherActor);
}

void ABlueprintTriggerVolume::OnActorEndOverlapVolume(AActor* OverlappedActor, AActor* OtherActor)
{
	IBlueprintTriggerVolumeInterface::Execute_OnActorEndOverlapVolume(BlueprintObject, OverlappedActor, OtherActor);
}
