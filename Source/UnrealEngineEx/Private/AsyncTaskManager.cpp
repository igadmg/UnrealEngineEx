#include "AsyncTaskManager.h"

#include "AsyncTask.h"

#include "UnrealEngineEx.final.h"


UAsyncTaskManager* UAsyncTaskManager::Instance = nullptr;



UAsyncTaskManager::UAsyncTaskManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		Instance = this;
		TickUpdateHandle = FWorldDelegates::OnWorldTickStart.AddUObject(this, &UAsyncTaskManager::TickUpdate);
	}
}

void UAsyncTaskManager::BeginDestroy()
{
	FWorldDelegates::OnWorldTickStart.Remove(TickUpdateHandle);

	Tasks.Empty();

	Super::BeginDestroy();
}

void UAsyncTaskManager::TickUpdate(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (TickType != LEVELTICK_All)
		return;

	TArray<UAsyncTask*> TasksToRemove;
	for (UAsyncTask* Task : Tasks)
	{
		if (!IsValid(Task))
			continue;

		if (Task->Update(DeltaSeconds))
		{
			Task->Finish(EAsyncTaskResult::Complete);
			Task->OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Complete);
			TasksToRemove.Add(Task);
			Task->ConditionalBeginDestroy();
		}

		Task->RunTime += DeltaSeconds;
		if (Task->Timeout > 0 && Task->RunTime > Task->Timeout)
		{
			Task->Finish(EAsyncTaskResult::Timeout);
			Task->OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Timeout);
			TasksToRemove.Add(Task);
			Task->ConditionalBeginDestroy();
		}
	}

	for (UAsyncTask* Task : TasksToRemove)
	{
		Tasks.Remove(Task);
	}
}
