#pragma once

#include "ExPrologue.h"
#include "Camera/PlayerCameraManager.h"


DECLARE_CONST_EXTENSION(APlayerCameraManager)
};

DECLARE_MUTABLE_EXTENSION(APlayerCameraManager)
	template <typename T>
	T* FindCameraModifierByClass()
	{
		return Cast<T>(This()->FindCameraModifierByClass(T::StaticClass()));
	}
};

DECLARE_EXTENSION(APlayerCameraManager);
