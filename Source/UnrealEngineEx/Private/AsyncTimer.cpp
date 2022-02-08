#include "AsyncTimer.h"

#include "Engine/Engine.h"



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