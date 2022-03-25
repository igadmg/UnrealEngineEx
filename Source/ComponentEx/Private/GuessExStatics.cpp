#include "GuessExStatics.h"

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"


APlayerController* UGuessExStatics::GetLocalPlayerController(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	return World->GetFirstPlayerController();
}

AHUD* UGuessExStatics::GetLocalPlayerHUD(const UObject* WorldContextObject)
{
	return GetPlayerHUD(GetLocalPlayerController(WorldContextObject));
}

template <typename TResult>
TResult* ForEachOwningActor(UObject* Object, TFunction<TResult* (AActor*)> Function)
{
	if (!IsValid(Object))
		return nullptr;

	if (auto AsActor = Cast<AActor>(Object))
	{
		if (auto Result = Function(AsActor))
			return Result;

		if (auto ParentActor = Valid(AsActor->GetParentActor()))
		{
			return ForEachOwningActor(ParentActor, Function);
		}

		if (auto ActorOwner = AsActor->GetOwner())
		{
			return ForEachOwningActor(ActorOwner, Function);
		}
	}
	else
	{
		if (auto AsActorComponent = Cast<UActorComponent>(Object))
		{
			return ForEachOwningActor(AsActorComponent->GetOwner(), Function);
		}

		if (auto AsUserWidget = Cast<UUserWidget>(Object))
		{
			return ForEachOwningActor(AsUserWidget->GetOwningPlayer(), Function);
		}
	}

	return ForEachOwningActor(Object->GetOuter(), Function);
}

bool UGuessExStatics::IsOwnedBy(const UObject* Object, const UObject* Owner)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::IsOwnedBy"), STAT_GuessExStaticsIsOwnedBy, STATGROUP_GuessEx);

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object), [Owner](auto Actor) { return Actor == Owner ? Actor : nullptr; }) != nullptr;
}

AActor* UGuessExStatics::GetOwningActor(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::GetOwningActor"), STAT_GuessExStaticsGetOwningActor, STATGROUP_GuessEx);

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object), [](auto Actor) { return Actor; });
}

AActor* UGuessExStatics::GetOwningActorByClass(const UObject* Object, TSubclassOf<AActor> ActorClass)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::GetOwningActorByClass"), STAT_GuessExStaticsGetOwningActorByClass, STATGROUP_GuessEx);

	if (!IsValid(ActorClass))
		return nullptr;

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object)
		, [&ActorClass](auto Actor) {
			return Actor->IsA(ActorClass) ? Actor : nullptr;
		});
}

AActor* UGuessExStatics::GetControlledActor(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::GetControlledActor"), STAT_GuessExStaticsGetControlledActor, STATGROUP_GuessEx);

	if (!IsValid(Object))
		return nullptr;

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object)
		, [](auto Actor) -> AActor* {
		if (auto AsController = Cast<AController>(Actor))
		{
			return XX::GetPawnOrSpectator(AsController);
		}

		if (auto AsHud = Cast<AHUD>(Actor))
		{
			return XX::GetPawnOrSpectator(AsHud);
		}
		
		return Actor;
	});
}

AHUD* UGuessExStatics::GetPlayerHUD(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::GetPlayerHUD"), STAT_GuessExStaticsGetPlayerHUD, STATGROUP_GuessEx);

	if (auto AsHUD = Cast<AHUD>(GetOwningActorByClass(Object, AHUD::StaticClass())))
	{
		return AsHUD;
	}
	if (auto AsPlayerController = Cast<APlayerController>(GetController(Object)))
	{
		return AsPlayerController->GetHUD();
	}

	return nullptr;
}

APlayerState* UGuessExStatics::GetPlayerState(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::GetPlayerState"), STAT_GuessExStaticsGetPlayerState, STATGROUP_GuessEx);

	return ForEachOwningActor<APlayerState>(const_cast<UObject*>(Object)
		, [](auto Actor) -> APlayerState* {
			if (auto AsPlayerState = Cast<APlayerState>(Actor))
			{
				return AsPlayerState;
			}

			if (auto AsPawn = Cast<APawn>(Actor))
			{
				return AsPawn->GetPlayerState();
			}

			if (auto AsController = Cast<AController>(Actor))
			{
				return AsController->PlayerState;
			}

			return nullptr;
		});


	return GetPlayerState(Object->GetOuter());
}

float UGuessExStatics::GetPlayerScore(const UObject* Object)
{
	if (auto PlayerState = UGuessExStatics::GetPlayerState(Object))
	{
		return PlayerState->GetScore();
	}

	return 0;
}

APawn* UGuessExStatics::GetPawnOrSpectator(const UObject* Object)
{
	if (auto Pawn = GetPlayerPawn(Object))
	{
		return Pawn;
	}

	return GetSpectatorPawn(Object);
}

APawn* UGuessExStatics::GetPlayerPawn(const UObject* Object)
{
	if (auto AsPawn = Cast<APawn>(GetOwningActorByClass(Object, APawn::StaticClass())))
	{
		return AsPawn;
	}

	if (auto Controller = GetController(Object))
	{
		return Controller->GetPawn();
	}

	if (auto PlayerState = GetPlayerState(Object))
	{
		return PlayerState->GetPawn();
	}

	return nullptr;
}

ASpectatorPawn* UGuessExStatics::GetSpectatorPawn(const UObject* Object)
{
	if (auto AsSpectatorPawn = Cast<ASpectatorPawn>(GetOwningActorByClass(Object, ASpectatorPawn::StaticClass())))
	{
		return AsSpectatorPawn;
	}

	if (auto Controller = Cast<APlayerController>(GetController(Object)))
	{
		return Controller->GetSpectatorPawn();
	}

	return nullptr;
}

AController* UGuessExStatics::GetController(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UGuessExStatics::GetController"), STAT_GuessExStaticsGetController, STATGROUP_GuessEx);

	if (auto AsGameInstance = Cast<UGameInstance>(Object))
	{
		return GetLocalPlayerController(AsGameInstance);
	}

	return ForEachOwningActor<AController>(const_cast<UObject*>(Object)
		, [](auto Actor) -> AController* {
			if (auto AsController = Cast<AController>(Actor))
			{
				return const_cast<AController*>(AsController);
			}

			if (auto AsPawn = Cast<APawn>(Actor))
			{
				return AsPawn->GetController();
			}

			return nullptr;
		});
}

APlayerController* UGuessExStatics::GetPlayerController(const UObject* Object)
{
	return Valid<APlayerController>(GetController(Object));
}

APlayerCameraManager* UGuessExStatics::GetPlayerCameraManager(const UObject* Object)
{
	const APlayerController* Controller = Cast<APlayerController>(GetController(Object));
	if (!IsValid(Controller))
		return nullptr;

	return Controller->PlayerCameraManager;
}

UCameraComponent* UGuessExStatics::GetPlayerActiveCamera(const UObject* Object)
{
	const APlayerController* Controller = Cast<APlayerController>(GetController(Object));
	if (!IsValid(Controller))
		return nullptr;

	if (!IsValid(Controller->PlayerCameraManager))
		return nullptr;

	// TODO: implement
	return nullptr;// Controller->PlayerCameraManager->GetCameraRotation;
}

UCharacterMovementComponent* UGuessExStatics::GetCharacterMovementComponent(const UObject* Object)
{
	if (!IsValid(Object))
		return nullptr;

	auto AsCharacter = Cast<ACharacter>(Object);
	if (AsCharacter)
	{
		return AsCharacter->GetCharacterMovement();
	}

	auto AsActorComponent = Cast<UActorComponent>(Object);
	if (AsActorComponent)
	{
		return GetCharacterMovementComponent(AsActorComponent->GetOwner());
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
