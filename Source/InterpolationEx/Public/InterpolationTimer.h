#pragma once

#include "InterpolationTimer.generated.h"



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

	UFUNCTION(Category = "Interpolation Timer", BlueprintPure, meta = (DisplayName = "Alpha", CompactNodeTitle = "Alpha"))
	float Alpha() const { return Duration > 0 ? FMath::Clamp(CurrentTime/Duration, 0.f, 1.f) : 1.f; }

	/** Advances timer by DeltaSeconds. Returns ture if timer is finished. */
	UFUNCTION(Category = "Interpolation Timer", BlueprintCallable)
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
