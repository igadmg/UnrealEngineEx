#include "Async/AsyncTaskManager.h"

#include "Async/AsyncTask.h"

#include "CoreEx.final.h"


UAsyncTaskManager::UAsyncTaskManager()
	: Super()
{
}

void UAsyncTaskManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UAsyncTask*> TasksToRemove;
	for (UAsyncTask* Task : Tasks)
	{
		if (!IsValid(Task))
		{
			TasksToRemove.Add(Task);
			continue;
		}

		if (Task->Update(DeltaTime))
		{
			Task->Finish(EAsyncTaskResult::Complete);
			Task->OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Complete);
			TasksToRemove.Add(Task);
			Task->ConditionalBeginDestroy();
		}

		Task->RunTime += DeltaTime;
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

#if false
	TArray<FCoreExOnAsyncTaskTickDelegate> TaskTickDelegatesToRemove;
	for (auto& TaskTick : TaskTickDelegates)
	{
		if (TaskTick.IsBound())
		{
			if (TaskTick.Execute(DeltaTime))
				TaskTickDelegatesToRemove.Add(TaskTick);
		}
		else
		{
			TaskTickDelegatesToRemove.Add(TaskTick);
		}
	}

	for (auto& TaskTick : TaskTickDelegatesToRemove)
	{
		TaskTickDelegates.Remove(TaskTick);
	}
#endif

	TArray<FCoreExOnAsyncTaskTickFunction> TaskTickFunctionsToRemove;
	for (auto& TaskTick : TaskTickFunctions)
	{
		TaskTick.GetHandle();
		if (TaskTick.IsBound())
		{
			if (TaskTick.Execute(DeltaTime))
				TaskTickFunctionsToRemove.Add(TaskTick);
		}
		else
		{
			TaskTickFunctionsToRemove.Add(TaskTick);
		}
	}

	for (auto& TaskTick : TaskTickFunctionsToRemove)
	{
		TaskTickFunctions.Remove(TaskTick);
	}
}
