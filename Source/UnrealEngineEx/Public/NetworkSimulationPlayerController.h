#pragma once

#include "GameFramework/PlayerController.h"

#include "NetworkSimulationPlayerController.generated.h"


UCLASS()
class UNREALENGINEEX_API ANetworkSimulationPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ANetworkSimulationPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void OnNetCleanup(class UNetConnection* Connection) override;


public:
	class UPlayer* PlayerCopy;
};
