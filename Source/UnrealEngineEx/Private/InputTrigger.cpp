#include "UnrealEngineExPrivatePCH.h"
#include "InputTrigger.h"
#include "Kismet/GameplayStatics.h"



UInputTrigger::UInputTrigger(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Reset();
}

UWorld* UInputTrigger::GetWorld() const
{
	return (!HasAnyFlags(RF_ClassDefaultObject) && GetOuter()) ? GetOuter()->GetWorld() : nullptr;
}

bool UInputTrigger::Trigger(float Value, float& OutputValue)
{
	bool bThresholdPassed = FMath::Abs(Value) > Threshold;

	OutputValue = 0;

	if (bThresholdPassed)
	{
		if (LastPressedState)
		{
			LastPressedTime += UGameplayStatics::GetWorldDeltaSeconds(this);

			float CheckTime = (LastPressedCount > 1) ? NextRepeatTime : FirstRepeatTime;
			if (LastPressedTime > CheckTime)
			{
				LastPressedCount++;
				LastPressedTime -= CheckTime;

				OutputValue = FMath::Sign(Value);
				return true;
			}

			return false;
		}
		else
		{
			LastPressedState = true;
			LastPressedCount = 1;
			LastPressedTime = 0;

			OutputValue = FMath::Sign(Value);
			return true;
		}
	}
	else
	{
		LastPressedState = false;
		LastPressedCount = 0;
		LastPressedTime = 0;
	}

	return false;
}

void UInputTrigger::Reset()
{
	LastPressedState = false;
	LastPressedCount = 0;
	LastPressedTime = 0;
}