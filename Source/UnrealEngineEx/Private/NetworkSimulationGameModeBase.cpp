#include "NetworkSimulationGameModeBase.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "NetworkSimulationGameInstance.h"

#include "UnrealEngineEx.final.h"


ANetworkSimulationGameModeBase::ANetworkSimulationGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseSeamlessTravel = true;
}

void ANetworkSimulationGameModeBase::PostInitializeComponents()
{
#if WITH_EDITOR
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		bUseSeamlessTravel = false;
	}
#endif

	Super::PostInitializeComponents();
}

void ANetworkSimulationGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	UNetworkSimulationGameInstance::RestorePlayerState(NewPlayer);

	Super::PostLogin(NewPlayer);
}

void ANetworkSimulationGameModeBase::Logout(AController* Exiting)
{
	//if (!bUseSeamlessTravel)
	{
		if (auto PlayerController = Valid<APlayerController>(Exiting))
		{
			if (auto PlayerState = Valid(PlayerController->PlayerState))
			{
				//UNetworkSimulationGameInstance::SavePlayerState(PlayerController); Commented out because it caused server to crash when client leaves session with ReplicationGraph enabled
			}
		}
	}

	Super::Logout(Exiting);
}
