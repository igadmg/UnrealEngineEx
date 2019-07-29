#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimerManager.h"
#include "AsyncTimer.generated.h"



UCLASS()
class UNREALENGINEEX_API UAsyncTimer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
};

UNREALENGINEEX_API FTimerManager& GetWorldTimerManager(UObject* Object);
