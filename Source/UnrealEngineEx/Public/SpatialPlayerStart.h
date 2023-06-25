#pragma once

#include "GameFramework/PlayerStart.h"

#include "SpatialPlayerStart.generated.h"


/**
* Player Start that provide custom start location and rotation provided by GetStartTransform function.
*/
UCLASS()
class UNREALENGINEEX_API ASpatialPlayerStart : public APlayerStart
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	int SpawnRetryCount = 10;


public:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FTransform GetStartTransform() const;


public:
	ASpatialPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

