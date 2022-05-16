#pragma once

#include "ExPrologue.h"
#include "Engine/World.h"
#include "ValidEx.h"


DECLARE_CONST_EXTENSION(UWorld)
};

DECLARE_MUTABLE_EXTENSION(UWorld)
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

DECLARE_EXTENSION(UWorld);
