#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Async/AsyncTask.h"
#include "Async/AsyncTypes.h"
#include "AsyncTaskStatics.generated.h"


UCLASS()
class COREEX_API UAsyncTaskStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable, BlueprintInternalUseOnly, meta = (DeterminesOutputType = "AsyncTaskClass", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTask(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass);

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable, meta = (DeterminesOutputType = "AsyncTaskClass", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTaskWithCallback(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass, const FCoreExOnAsyncTaskFinishedDelegate& OnFinished, bool bAutorun = true);

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable, meta = (DeterminesOutputType = "AsyncTaskClass", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTaskNoCallback(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass, bool bAutorun = true);

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static void RunAsyncTaskFunction(const UObject* WorldContextObject, FCoreExOnAsyncTaskTickDelegate TaskFunction);

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static void ClearAsyncTasks(const UObject* WorldContextObject);
};

