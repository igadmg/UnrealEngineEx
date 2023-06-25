#pragma once

#include "InputTrigger.generated.h"



UCLASS(BlueprintType)
class UNREALENGINEEX_API UInputTriggerObject : public UObject
{
	GENERATED_BODY()



public:
	virtual class UWorld* GetWorld() const override;



public:
	UPROPERTY(Category = "Input Trigger", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	float Threshold = 0.7f;

	UPROPERTY(Category = "Input Trigger", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	float FirstRepeatTime = 0.3f;

	UPROPERTY(Category = "Input Trigger", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	float NextRepeatTime = 0.1f;



public:
	UFUNCTION(Category = "UnrealEngineEx: Input Trigger", BlueprintCallable)
	bool Trigger(float Value, float& OutputValue);

	UFUNCTION(Category = "UnrealEngineEx: Input Trigger", BlueprintCallable)
	void Reset();



public:
	UInputTriggerObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());



protected:
	bool LastPressedState;
	int32 LastPressedCount;
	float LastPressedTime;
};
