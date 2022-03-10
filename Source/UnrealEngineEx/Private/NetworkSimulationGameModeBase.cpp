#include "NetworkSimulationGameModeBase.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "NetworkSimulationGameInstance.h"

#include "ValidEx.h"


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
				UNetworkSimulationGameInstance::SavePlayerState(PlayerController);
			}
		}
	}

	Super::Logout(Exiting);
}
