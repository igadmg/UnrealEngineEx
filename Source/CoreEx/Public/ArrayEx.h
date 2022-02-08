#pragma once

#include "ExPrologue.h"

#include "Math/UnrealMathUtility.h"



namespace FArrayEx
{
	template <typename T>
	void Shuffle(TArray<T>& Array)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
}
