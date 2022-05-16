#pragma once

#include "ExPrologue.h"

#include "Math/UnrealMathUtility.h"



DECLARE_CONST_EXTENSION_TEMPLATE_REF(TArray, InElementType, InAllocatorType)
	void Shuffle()
	{
		int32 LastIndex = This().Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				This().Swap(i, Index);
			}
		}
	}

	template <typename U>
	TArray<U> Cast()
	{
		TArray<U> Result;
		Result.SetNumUninitialized(This().Num());

		for (auto Item : This())
		{
			if (auto CastItem = ::Cast<typename std::remove_pointer<U>::type>(Item))
				Result.Add(CastItem);
		}

		return Result;
	}
};

DECLARE_MUTABLE_EXTENSION_TEMPLATE_REF(TArray, InElementType, InAllocatorType)
};

EXTENSION_TEMPLATE_REF(TArray, InElementType, InAllocatorType);
