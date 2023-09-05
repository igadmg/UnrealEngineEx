#pragma once

#include "GameFramework/GameModeBase.h"

#include "NetworkSimulationGameModeBase.generated.h"


UCLASS()
class UNREALENGINEEX_API ANetworkSimulationGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	ANetworkSimulationGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void PostInitializeComponents() override;

	virtual void PostLogin(class APlayerController* NewPlayer) override;
	virtual void Logout(class AController* Exiting) override;
};
