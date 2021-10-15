#pragma once

#include "Camera/PlayerCameraManager.h"



struct FPlayerCameraManagerEx
{
	FPlayerCameraManagerEx(APlayerCameraManager* This)
		: This(This)
	{
	}


	template <typename T>
	T* FindCameraModifierByClass()
	{
		return Cast<T>(This->FindCameraModifierByClass(T::StaticClass()));
	}


protected:
	APlayerCameraManager* This;
};
