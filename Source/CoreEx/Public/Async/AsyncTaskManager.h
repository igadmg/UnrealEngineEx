#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Async/AsyncTypes.h"
#include "AsyncTaskManager.generated.h"



UCLASS()
class COREEX_API UAsyncTaskManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "AsyncTaskManager", EditAnywhere)
	TSet<class UAsyncTask*> Tasks;

	//UPROPERTY()
	//TSet<FCoreExOnAsyncTaskTickDelegate> TaskTickDelegates;

	TSet<FCoreExOnAsyncTaskTickFunction> TaskTickFunctions;


public:
	UAsyncTaskManager();


protected:
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UAsyncTaskManager, STATGROUP_Tickables); }
	virtual void Tick(float DeltaTime) override;
};
