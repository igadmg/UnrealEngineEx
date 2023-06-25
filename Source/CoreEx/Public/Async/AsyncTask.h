#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncTypes.h"
#include "AsyncTask.generated.h"


UCLASS(blueprintable, BlueprintType)
class COREEX_API UAsyncTask : public UObject
{
	GENERATED_BODY()


public:
	virtual class UWorld* GetWorld() const override;


public:
	UPROPERTY(Category = "AsyncTask", EditAnywhere, BlueprintReadWrite)
	float Timeout = 60;

	UPROPERTY(Category = "AsyncTask", VisibleInstanceOnly, transient)
	float RunTime;

	UPROPERTY(Category = "AsyncTask", EditAnywhere)
	FCoreExOnAsyncTaskFinishedDelegate OnFinishedDelegate;


public:
	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable)
	void Run();

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintCallable)
	void Abort();

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintImplementableEvent)
	void Begin();

	/** Update function returns true if Task is finished. */
	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintImplementableEvent)
	bool Update(float DeltaSeconds);

	UFUNCTION(Category = "CoreEx|AsyncTask", BlueprintImplementableEvent)
	void Finish(EAsyncTaskResult Result);


public:
	UAsyncTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~UAsyncTask();
};
