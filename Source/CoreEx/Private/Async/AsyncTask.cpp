#include "Async/AsyncTask.h"

#include "Async/AsyncTaskManager.h"

#include "CoreEx.final.h"


UAsyncTask::UAsyncTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncTask::~UAsyncTask()
{
}

UWorld* UAsyncTask::GetWorld() const
{
	return (!HasAnyFlags(RF_ClassDefaultObject) && GetOuter()) ? GetOuter()->GetWorld() : nullptr;
}

void UAsyncTask::Run()
{
	if (auto AsyncTaskManager = XX::GetSubsystem<UAsyncTaskManager>(this))
	{
		RunTime = 0;
		AsyncTaskManager->Tasks.Add(this);

		Begin();
	}
}

void UAsyncTask::Abort()
{
	if (auto AsyncTaskManager = XX::GetSubsystem<UAsyncTaskManager>(this))
	{
		Finish(EAsyncTaskResult::Abort);
		OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Abort);

		AsyncTaskManager->Tasks.Remove(this);
	}
}
