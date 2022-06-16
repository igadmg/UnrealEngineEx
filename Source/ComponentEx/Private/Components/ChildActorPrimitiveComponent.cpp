#include "Components/ChildActorPrimitiveComponent.h"
#include "Misc/EngineVersionComparison.h"

#include "ComponentEx.final.h"


UChildActorPrimitiveComponent::UChildActorPrimitiveComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowReregistration = false;
}

void UChildActorPrimitiveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UChildActorPrimitiveComponent, ChildActorClass);
	DOREPLIFETIME(UChildActorPrimitiveComponent, ChildActorPtr);
}

void UChildActorPrimitiveComponent::OnRegister()
{
	Super::OnRegister();

	if (!IsValid(ChildActorPtr))
	{
		CreateChildActor();
	}
	else if (auto ChildActor = GetChildActor())
	{
		if (GetComponentLevel() == ChildActor->GetLevel())
		{
			if (USceneComponent* ChildRoot = ChildActor->GetRootComponent())
			{
				if (auto AttachedParent = ChildRoot->GetAttachParent())
				{
					TGuardValue<TEnumAsByte<EComponentMobility::Type>> MobilityGuard(ChildRoot->Mobility, Mobility);
					ChildRoot->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

					OnChildActorCreated(ChildActor);
				}
			}
		}
		else
		{
			CloneChildActor(ChildActor);
		}
	}
}

void UChildActorPrimitiveComponent::OnUnregister()
{
	if (!bPreventDestroyOnUnregister)
	{
		if (auto Level = Valid(GetComponentLevel()))
		{
			if ((Level->bAreComponentsCurrentlyRegistered || IsCreatedByConstructionScript())
				&& !IsGarbageCollecting()
				/* && !FCoreEx::IsObjectReinst(GetOuter())*/)
				DestroyChildActor();
		}
	}

	Super::OnUnregister();
}

#if WITH_EDITOR
void UChildActorPrimitiveComponent::PreEditChange(FProperty* PropertyThatWillChange)
{
	bPreventDestroyOnUnregister = true;

	Super::PreEditChange(PropertyThatWillChange);

	bPreventDestroyOnUnregister = false;
}

void UChildActorPrimitiveComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if_PropertyChanged(ChildActorClass)
	{
		SetChildActorClass(ChildActorClass, nullptr);
	}
}

void UChildActorPrimitiveComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}

void UChildActorPrimitiveComponent::PostEditImport()
{
	Super::PostEditImport();
}

void UChildActorPrimitiveComponent::PostEditUndo()
{
	Super::PostEditUndo();
}

void UChildActorPrimitiveComponent::PostLoad()
{
	Super::PostLoad();
}
#endif

void UChildActorPrimitiveComponent::SetChildActor(AActor* ChildActor, FName SocketName)
{
	ChildAttachedActors.Empty();
	if (IsValid(ChildActorPtr))
	{
		DestroyChildActor();
	}

	if (IsValid(ChildActor))
	{
		if (USceneComponent* ChildRoot = ChildActor->GetRootComponent())
		{
			TGuardValue<TEnumAsByte<EComponentMobility::Type>> MobilityGuard(ChildRoot->Mobility, Mobility);
			ChildRoot->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}

		ChildActorClass = ChildActor->GetClass();
		ChildActorPtr = ChildActor;
		OnChildActorCreated(ChildActor);
	}
}

void UChildActorPrimitiveComponent::SetChildActorClass(TSubclassOf<AActor> InClass, AActor* NewChildActorTemplate)
{
	if (auto ChildActor = Valid(ChildActorPtr))
	{
		if (ChildActor->GetClass() != InClass)
		{
			ChildActor->GetAttachedActors(ChildAttachedActors, true);
			DestroyChildActor();
		}
	}

	if (!IsValid(ChildActorPtr))
	{
		ChildActorClass = InClass;
		CreateChildActor();
		if (auto ChildActor = Valid(ChildActorPtr))
		{
			for (auto ChildAttachedActor : ChildAttachedActors)
			{
				ChildAttachedActor->AttachToActor(ChildActor, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

void UChildActorPrimitiveComponent::SetPreventDestroyOnUnregister(bool NewValue)
{
	bPreventDestroyOnUnregister = NewValue;
}

void UChildActorPrimitiveComponent::CreateChildActor(AActor* ActorTemplate)
{
	ChildActorPtr = nullptr;
	if (ChildActorClass == nullptr)
		return;

	AActor* MyOwner = Valid(GetOwner());
	if (auto World = Valid(GetWorld()))
	{
		FActorSpawnParameters Params;
		Params.Owner = MyOwner;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.bDeferConstruction = true; // We defer construction so that we set ParentComponent prior to component registration so they appear selected
		Params.bAllowDuringConstructionScript = true;
		Params.OverrideLevel = (MyOwner ? MyOwner->GetLevel() : nullptr);
		Params.Name = /*ChildActorName*/ NAME_None;
		Params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
#if WITH_EDITOR
		Params.OverridePackage = GetOwner()->GetExternalPackage();
		/*Params.OverrideParentComponent = this;*/
		Params.OverrideActorGuid = /*CachedInstanceData ? CachedInstanceData->ChildActorGUID :*/ FGuid();
#endif

		if (ActorTemplate && ActorTemplate->GetClass() == ChildActorClass)
		{
			Params.Template = ActorTemplate;
		}

		Params.ObjectFlags |= (/*RF_TextExportTransient | */RF_NonPIEDuplicateTransient);
		if (!HasAllFlags(RF_Transactional))
		{
			Params.ObjectFlags &= ~RF_Transactional;
		}
		if (HasAllFlags(RF_Transient) || IsEditorOnly() || (MyOwner && (MyOwner->HasAllFlags(RF_Transient) || MyOwner->IsEditorOnly())))
		{
			// If this component or its owner are transient or editor only, set our created actor to transient.
			// We can't programatically set editor only on an actor so this is the best option
			Params.ObjectFlags |= RF_Transient;
		}

		ConditionalUpdateComponentToWorld();
		FVector Location = GetChildActorLocation();
		FRotator Rotation = GetChildActorRotation();

		if (auto ChildActor = World->SpawnActor(ChildActorClass, &Location, &Rotation, Params))
		{
			OnChildActorSpawned(ChildActor);

			if (Valid(ActorTemplate))
			{
				FComponentInstanceDataCache InstanceDataCache(ActorTemplate);
				ChildActor->FinishSpawning(GetChildActorTransform(), false, &InstanceDataCache);
			}
			else
				ChildActor->FinishSpawning(GetChildActorTransform(), false);

			if (USceneComponent* ChildRoot = ChildActor->GetRootComponent())
			{
				TGuardValue<TEnumAsByte<EComponentMobility::Type>> MobilityGuard(ChildRoot->Mobility, Mobility);
				ChildRoot->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			}

			ChildActorPtr = ChildActor;
			OnChildActorCreated(ChildActor);
		}
	}
}

void UChildActorPrimitiveComponent::DestroyChildActor()
{
	if (auto ChildActor = GetChildActor())
	{
		if (ChildActor->HasAuthority()/* && !IsLevelBeingRemoved()*/)
		{
			if (!GExitPurge)
			{
#if !UE_VERSION_OLDER_THAN(5, 0, 0)
				const bool bIsChildActorPendingKillOrUnreachable = !IsValid(ChildActor);
#else
				const bool bIsChildActorPendingKillOrUnreachable = ChildActor->IsPendingKillOrUnreachable();
#endif

				if (auto World = ChildActor->GetWorld())
				{
					UClass* ChildClass = ChildActor->GetClass();

					if (!IsGarbageCollecting())
					{
						const FString ObjectBaseName = FString::Printf(TEXT("DESTROYED_%s_CHILDACTOR"), *ChildClass->GetName());
						ChildActor->Rename(*MakeUniqueObjectName(ChildActor->GetOuter(), ChildClass, *ObjectBaseName).ToString(), nullptr, REN_DoNotDirty | REN_ForceNoResetLoaders);
					}

					if (!bIsChildActorPendingKillOrUnreachable)
					{
						OnChildActorDestroyed(ChildActor);
						XX::DestroyActor(ChildActor);
					}
				}
			}
		}
	}

	ClearChildActor();
}

void UChildActorPrimitiveComponent::ClearChildActor()
{
	ChildActorPtr = nullptr;
}

void UChildActorPrimitiveComponent::CloneChildActor(AActor* SrcActor)
{
	CreateChildActor(SrcActor);
}
