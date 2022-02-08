#pragma once

#include "Camera/PlayerCameraManager.h"



struct COMPONENTEX_API FPlayerCameraManagerEx
{
	FPlayerCameraManagerEx(APlayerCameraManager* This)
		: This(This)
	{
	}


public:
	template <typename T>
	T* FindCameraModifierByClass()
	{
		return Cast<T>(This->FindCameraModifierByClass(T::StaticClass()));
	}


protected:
	APlayerCameraManager* This;
};
