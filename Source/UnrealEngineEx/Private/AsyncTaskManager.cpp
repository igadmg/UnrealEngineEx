#include "AsyncTaskManager.h"

#include "AsyncTask.h"



UAsyncTaskManager* UAsyncTaskManager::Instance = nullptr;



UAsyncTaskManager::UAsyncTaskManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Instance = this;
	TickUpdateHandle = FWorldDelegates::OnWorldTickStart.AddUObject(this, &UAsyncTaskManager::TickUpdate);
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

	for (UAsyncTask* Task : Tasks)
	{
		if (!IsValid(Task))
			continue;

		if (Task->Update(DeltaSeconds))
		{
			Task->Finish(EAsyncTaskResult::Complete);
			Task->OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Complete);
			Tasks.Remove(Task);
			Task->ConditionalBeginDestroy();
		}

		Task->RunTime += DeltaSeconds;
		if (Task->Timeout > 0 && Task->RunTime > Task->Timeout)
		{
			Task->Finish(EAsyncTaskResult::Timeout);
			Task->OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Timeout);
			Tasks.Remove(Task);
			Task->ConditionalBeginDestroy();
		}
	}
}
