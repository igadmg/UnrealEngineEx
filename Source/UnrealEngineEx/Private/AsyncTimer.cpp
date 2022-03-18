#include "AsyncTimer.h"

#include "Engine/Engine.h"

#include "UnrealEngineEx.final.h"


/*
UAsyncTimer::UAsyncTimer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
*/

FTimerManager& GetWorldTimerManager(UObject* Object)
{
	UWorld* World = GEngine->GetWorldFromContextObject(Object, EGetWorldErrorMode::ReturnNull);
	return World->GetTimerManager();
}