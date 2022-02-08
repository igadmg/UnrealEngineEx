#pragma once

#include "Engine/World.h"


/**
 Throttle execution of function. Don't run it more often as needed.
*/
struct FThrottle
{
	float LastCallTime = -1;

	template <typename FN>
	void Run(UObject *WorldContextObject, float Interval, FN fn)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		if (!IsValid(World))
			return;

		float Time = World->GetRealTimeSeconds();
		if ((Time - LastCallTime) > Interval)
		{
			LastCallTime = Time;
			fn();
		}
	}
};
