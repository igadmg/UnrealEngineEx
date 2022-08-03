#pragma once

#include "UObject/Object.h"
#include "InterpolationTimer.generated.h"



struct FInterpolationTimer
{
	float Duration;
	float CurrentTime;
	bool bIsCyclic;

	FInterpolationTimer()
		: Duration(-1)
		, CurrentTime(0.f)
		, bIsCyclic(false)
	{
	}

	FInterpolationTimer(float Duration, bool bIsCyclic = false)
		: Duration(Duration)
		, CurrentTime(0.f)
		, bIsCyclic(bIsCyclic)
	{
	}

	bool IsFinished() const { return CurrentTime >= Duration; }
	float Alpha() const { return Duration > 0 ? FMath::Clamp(CurrentTime / Duration, 0.f, 1.f) : 1.f; }
	bool Advance(float DeltaSeconds)
	{
		CurrentTime += DeltaSeconds;
		bool bIsFinished = IsFinished();
		if (bIsCyclic && bIsFinished)
		{
			if (Duration > 0) while (CurrentTime > Duration) CurrentTime -= Duration;
		}
		return bIsFinished;
	}

	void Reset(float NewDuration, float NewCurrentTime = 0.0f, bool bNewIsCyclic = false)
	{
		Duration = NewDuration;
		CurrentTime = NewCurrentTime;
		bIsCyclic = bNewIsCyclic;
	}
};

UCLASS(BlueprintType)
class INTERPOLATIONEX_API UInterpolationTimer : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Interpolation Timer", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool bIsCyclic;

	UPROPERTY(Category = "Interpolation Timer", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float Duration;

	UPROPERTY(Category = "Interpolation Timer", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float CurrentTime;



public:
	bool IsFinished() const { return CurrentTime >= Duration; }

	UFUNCTION(Category = "UnrealEngineEx: Interpolation Timer", BlueprintPure, meta = (DisplayName = "Alpha", CompactNodeTitle = "Alpha"))
	float Alpha() const { return Duration > 0 ? FMath::Clamp(CurrentTime/Duration, 0.f, 1.f) : 1.f; }

	/** Advances timer by DeltaSeconds. Returns ture if timer is finished. */
	UFUNCTION(Category = "UnrealEngineEx: Interpolation Timer", BlueprintCallable)
	bool Advance(float DeltaSeconds)
	{
		CurrentTime += DeltaSeconds;
		bool bIsFinished = IsFinished();
		if (bIsCyclic && bIsFinished)
		{
			if (Duration > 0) while (CurrentTime > Duration) CurrentTime -= Duration;
		}
		return bIsFinished;
	}
};
