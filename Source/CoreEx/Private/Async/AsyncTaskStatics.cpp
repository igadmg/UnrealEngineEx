#include "Async/AsyncTaskStatics.h"

#include "Async/AsyncTaskManager.h"
#include "Async/AsyncTask.h"

#include "CoreEx.final.h"


UAsyncTask* UAsyncTaskStatics::CreateAsyncTask(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass)
{
	return NewObject<UAsyncTask>(const_cast<UObject*>(WorldContextObject), AsyncTaskClass);
}

UAsyncTask* UAsyncTaskStatics::CreateAsyncTaskWithCallback(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass, const FCoreExOnAsyncTaskFinishedDelegate& OnFinished, bool bAutorun)
{
	if (auto AsyncTask = CreateAsyncTask(WorldContextObject, AsyncTaskClass))
	{
		AsyncTask->OnFinishedDelegate = OnFinished;
		if (bAutorun)
		{
			AsyncTask->Run();
		}

		return AsyncTask;
	}

	return nullptr;
}

UAsyncTask* UAsyncTaskStatics::CreateAsyncTaskNoCallback(const UObject* WorldContextObject, TSubclassOf<class UAsyncTask> AsyncTaskClass, bool bAutorun)
{
	return CreateAsyncTaskWithCallback(WorldContextObject, AsyncTaskClass, FCoreExOnAsyncTaskFinishedDelegate(), bAutorun);
}

void UAsyncTaskStatics::RunAsyncTaskFunction(const UObject* WorldContextObject, FCoreExOnAsyncTaskTickDelegate TaskFunction)
{
#if false
	if (auto AsyncTaskManager = XX::GetSubsystem<UAsyncTaskManager>(WorldContextObject))
	{
		if (!AsyncTaskManager->TaskTickDelegates.Contains(TaskFunction))
			AsyncTaskManager->TaskTickDelegates.Add(TaskFunction);
	}
#endif
}

void UAsyncTaskStatics::ClearAsyncTasks(const UObject* WorldContextObject)
{
	if (auto AsyncTaskManager = XX::GetSubsystem<UAsyncTaskManager>(WorldContextObject))
	{
		for (UAsyncTask* AsyncTask : AsyncTaskManager->Tasks)
		{
			if (IsValid(AsyncTask))
			{
				AsyncTask->Abort();
			}
		}

		AsyncTaskManager->Tasks.Empty();
		AsyncTaskManager->TaskTickFunctions.Empty();
	}
}
