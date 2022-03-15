#pragma once

#include "Engine/World.h"
#include "ValidEx.h"


struct FWorldConstEx
{
	FWorldConstEx(const UWorld* This)
		: This_(This)
	{
	}

	const UWorld* This() const { return This_; }


private:
	const UWorld* This_;
};

struct FWorldMutableEx : public FWorldConstEx
{
	FWorldMutableEx(UWorld* This)
		: FWorldConstEx(This)
	{
	}

	UWorld* This() { return const_cast<UWorld*>(FWorldConstEx::This()); }


public:
	template <typename T, typename InitializerFunction>
	T* SpawnActor(const TSubclassOf<T>& Class, const FActorSpawnParameters& SpawnParameters, InitializerFunction Initializer)
	{
		return SpawnActor<T>(Valid(Class), SpawnParameters, Initializer);
	}

	template <typename T, typename InitializerFunction>
	T* SpawnActor(UClass* Class, const FActorSpawnParameters& SpawnParameters, InitializerFunction Initializer)
	{
		if (auto Actor = Valid(This()->SpawnActor<T>(Class, SpawnParameters)))
		{
			Initializer(Actor);
			if (SpawnParameters.bDeferConstruction)
			{
				Actor->FinishSpawning({});
			}

			return Actor;
		}

		return nullptr;
	}
};

static FWorldConstEx FWorldEx(const UWorld* This)
{
	return FWorldConstEx(This);
}

static FWorldMutableEx FWorldEx(UWorld* This)
{
	return FWorldMutableEx(This);
}
