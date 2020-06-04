#pragma once

#include "AsyncTaskManager.generated.h"



UCLASS()
class UNREALENGINEEX_API UAsyncTaskManager : public UObject
{
	GENERATED_BODY()



public:
	static UAsyncTaskManager* Instance;



public:
	UPROPERTY()
	TSet<class UAsyncTask*> Tasks;



public:
	UAsyncTaskManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;



protected:
	FDelegateHandle TickUpdateHandle;
	void TickUpdate(class UWorld* World, ELevelTick TickType, float DeltaSeconds);
};
