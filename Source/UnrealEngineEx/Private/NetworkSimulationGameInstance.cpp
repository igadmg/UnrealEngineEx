#include "NetworkSimulationGameInstance.h"

#include "NetworkSimulationPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "UnrealEngineEx.final.h"


UNetworkSimulationGameInstance::UNetworkSimulationGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UNetworkSimulationGameInstance::RestorePlayerState(APlayerController* PlayerController)
{
	if (auto GameInstance = Valid<UNetworkSimulationGameInstance>(UGameplayStatics::GetGameInstance(PlayerController)))
	{
		if (auto PlayerStatePtr = GameInstance->PlayerStateCache.Find(PlayerController->Player))
		{
			(*PlayerStatePtr)->DispatchCopyProperties(PlayerController->PlayerState);
			GameInstance->PlayerStateCache.Remove(PlayerController->Player);
		}
	}
	else
	{
		UE_LOG_EX_WCO(LogUnrealEngineEx, Warning, PlayerController, TEXT("GameInstance should be a subclass of UNetworkSimulationGameInstance"));
	}
}

void UNetworkSimulationGameInstance::SavePlayerState(APlayerController* PlayerController)
{
	if (auto GameInstance = Valid<UNetworkSimulationGameInstance>(UGameplayStatics::GetGameInstance(PlayerController)))
	{
		auto PlayerState = PlayerController->PlayerState;
		auto Player = PlayerController->Player;
		if (auto NetworkSimulationPlayerController = Valid<ANetworkSimulationPlayerController>(PlayerController))
		{
			if (Player == nullptr)
			{
				Player = NetworkSimulationPlayerController->PlayerCopy;
			}
		}
		if (!PlayerState->GetWorld()->bIsTearingDown)
		{
			PlayerState = PlayerState->Duplicate();
		}
		if (auto GameState = UGameplayStatics::GetGameState(PlayerState))
		{
			GameState->RemovePlayerState(PlayerState);
		}

		GameInstance->PlayerStateCache.Emplace(Player, PlayerState);
		PlayerState->Rename(nullptr, GameInstance);
		PlayerState->SetReplicates(false);
	}
	else
	{
		UE_LOG_EX_WCO(LogUnrealEngineEx, Warning, PlayerController, TEXT("GameInstance should be a subclass of UNetworkSimulationGameInstance"));
	}
}
