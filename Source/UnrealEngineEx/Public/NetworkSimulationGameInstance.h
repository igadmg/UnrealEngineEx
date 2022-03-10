#pragma once

#include "Engine/GameInstance.h"

#include "NetworkSimulationGameInstance.generated.h"


UCLASS()
class UNREALENGINEEX_API UNetworkSimulationGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TMap<class UPlayer*, class APlayerState*> PlayerStateCache;


public:
	UNetworkSimulationGameInstance(const FObjectInitializer& ObjectInitializer);


public:
	static void RestorePlayerState(class APlayerController* PlayerController);
	static void SavePlayerState(class APlayerController* PlayerController);
};
