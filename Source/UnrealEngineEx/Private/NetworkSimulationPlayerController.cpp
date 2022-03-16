#include "NetworkSimulationPlayerController.h"

#include "UnrealEngineEx.final.h"


ANetworkSimulationPlayerController::ANetworkSimulationPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ANetworkSimulationPlayerController::OnNetCleanup(UNetConnection* Connection)
{
	PlayerCopy = Player;

	Super::OnNetCleanup(Connection);
}
