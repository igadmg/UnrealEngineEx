#include "UnrealEngineExPrivatePCH.h"
#include "AsyncTask.h"

#include "AsyncTaskManager.h"



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
	if (!IsValid(UAsyncTaskManager::Instance))
		return;

	RunTime = 0;
	UAsyncTaskManager::Instance->Tasks.Add(this);

	Begin();
}

void UAsyncTask::Abort()
{
	if (!IsValid(UAsyncTaskManager::Instance))
		return;

	Finish(EAsyncTaskResult::Abort);
	OnFinishedDelegate.ExecuteIfBound(EAsyncTaskResult::Abort);
	UAsyncTaskManager::Instance->Tasks.Remove(this);
}
