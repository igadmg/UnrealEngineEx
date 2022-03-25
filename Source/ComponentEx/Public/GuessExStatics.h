#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValidEx.h"
#include "GuessExStatics.generated.h"


UCLASS()
class COMPONENTEX_API UGuessExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class APlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class AHUD* GetLocalPlayerHUD(const UObject* WorldContextObject);


	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Owner"))
	static bool IsOwnedBy(const UObject* Object, const UObject* Owner);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class AActor* GetOwningActor(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object", DeterminesOutputType = "ActorClass"))
	static class AActor* GetOwningActorByClass(const UObject* Object, TSubclassOf<class AActor> ActorClass);

	/** Return and actor being controlled by Object (if it is owned by Controller, HUD etc.)
		Or retrun Actor itself.
		Some form of GetOwnerActor but it prefer to fallback to GetPawnOrSpectator and skip Controllers and Huds.
	*/
	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class AActor* GetControlledActor(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, BlueprintCosmetic, meta = (DefaultToSelf = "Object"))
	static class AHUD* GetPlayerHUD(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APlayerState* GetPlayerState(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static float GetPlayerScore(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APawn* GetPawnOrSpectator(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APawn* GetPlayerPawn(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, BlueprintCosmetic, meta = (DefaultToSelf = "Object"))
	static class ASpectatorPawn* GetSpectatorPawn(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class AController* GetController(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APlayerController* GetPlayerController(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APlayerCameraManager* GetPlayerCameraManager(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class UCameraComponent* GetPlayerActiveCamera(const UObject* Object);

	UFUNCTION(Category = "GuessEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class UCharacterMovementComponent* GetCharacterMovementComponent(const UObject* Object);
};

class AActor;
class AController;
class AGameModeBase;
class AHUD;
class APawn;
class APlayerState;
class ASpectatorPawn;

namespace XX
{	
	template <typename TActor = AActor>
	TActor* GetOwningActor(const UObject* Object)
	{
		return Cast<TActor>(UGuessExStatics::GetOwningActor(Object));
	}

	template <typename TActor = AActor>
	const TActor* GetOwningActor(const AActor* Object)
	{
		return Cast<TActor>(Object);
	}

	template <typename TActor = AActor>
	TActor* GetOwningActor(const UActorComponent* Object)
	{
		return Cast<TActor>(Object->GetOwner());
	}

	template <typename TGameMode = AGameModeBase>
	TGameMode* GetGameMode(const UObject* Object)
	{
		return TValid<TGameMode, AGameModeBase>::Valid(UGameplayStatics::GetGameMode(Object));
	}

	template <typename THUD = AHUD>
	THUD* GetPlayerHUD(const UObject* Object)
	{
		return TValid<THUD, AHUD>::Valid(UGuessExStatics::GetPlayerHUD(Object));
	}

	template <typename TPlayerState = APlayerState>
	TPlayerState* GetPlayerState(const UObject* Object)
	{
		return TValid<TPlayerState, APlayerState>::Valid(UGuessExStatics::GetPlayerState(Object));
	}

	template <typename TPawn = APawn>
	TPawn* GetPawnOrSpectator(const UObject* Object)
	{
		return TValid<TPawn, APawn>::Valid(UGuessExStatics::GetPawnOrSpectator(Object));
	}

	template <typename TPawn = APawn>
	TPawn* GetPlayerPawn(const UObject* Object)
	{
		return TValid<TPawn, APawn>::Valid(UGuessExStatics::GetPlayerPawn(Object));
	}

	template <typename TSpectatorPawn = ASpectatorPawn>
	TSpectatorPawn* GetSpectatorPawn(const UObject* Object)
	{
		return TValid<TSpectatorPawn, ASpectatorPawn>::Valid(UGuessExStatics::GetSpectatorPawn(Object));
	}

	template <typename TController = AController>
	TController* GetController(const UObject* Object)
	{
		return TValid<TController, AController>::Valid(UGuessExStatics::GetController(Object));
	}

	template <typename TController = AController>
	TController* GetLocalPlayerController(const UObject* Object)
	{
		return TValid<TController, AController>::Valid(UGuessExStatics::GetLocalPlayerController(Object));
	}

	template <typename TPlayerCameraManager = APlayerCameraManager>
	TPlayerCameraManager* GetPlayerCameraManager(const UObject* Object)
	{
		return TValid<TPlayerCameraManager, APlayerCameraManager>::Valid(UGuessExStatics::GetPlayerCameraManager(Object));
	}

	template <typename TCameraComponent = UCameraComponent>
	TCameraComponent* GetPlayerActiveCamera(const UObject* Object)
	{
		return TValid<TCameraComponent, UCameraComponent>::Valid(UGuessExStatics::GetPlayerActiveCamera(Object));
	}

	template <typename TCharacterMovementComponent = UCharacterMovementComponent>
	TCharacterMovementComponent* GetCharacterMovementComponent(const UObject* Object)
	{
		return TValid<TCharacterMovementComponent, UCharacterMovementComponent>::Valid(UGuessExStatics::GetCharacterMovementComponent(Object));
	}
}
