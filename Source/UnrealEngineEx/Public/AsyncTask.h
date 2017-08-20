#pragma once

#include "UnrealEngineExTypes.h"
#include "AsyncTask.generated.h"



UCLASS(blueprintable, BlueprintType)
class UNREALENGINEEX_API UAsyncTask : public UObject
{
	GENERATED_BODY()



public:
	virtual class UWorld* GetWorld() const override;



public:
	UPROPERTY(Category = "AsyncTask", EditAnywhere, BlueprintReadWrite)
	float Timeout = 60;

	UPROPERTY(Category = "AsyncTask", VisibleInstanceOnly, transient)
	float RunTime;

	UPROPERTY()
	FUnrealEngineExOnAsyncTaskFinishedDelegate OnFinishedDelegate;



public:
	UFUNCTION(Category = "UnrealEngineEx: AsyncTask", BlueprintCallable)
	void Run();

	UFUNCTION(Category = "UnrealEngineEx: AsyncTask", BlueprintCallable)
	void Abort();

	UFUNCTION(Category = "UnrealEngineEx: AsyncTask", BlueprintImplementableEvent)
	void Begin();

	/** Update function returns true if Task is finished. */
	UFUNCTION(Category = "UnrealEngineEx: AsyncTask", BlueprintImplementableEvent)
	bool Update(float DeltaSeconds);

	UFUNCTION(Category = "UnrealEngineEx: AsyncTask", BlueprintImplementableEvent)
	void Finish(EAsyncTaskResult Result);



public:
	UAsyncTask(const FObjectInitializer& ObjectInitializer);
	~UAsyncTask();
};
