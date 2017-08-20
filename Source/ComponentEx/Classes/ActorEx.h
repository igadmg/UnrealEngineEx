#pragma once

#include "GameFramework/Actor.h"



struct FActorEx
{
	FActorEx(AActor* This)
		: This(This)
	{
	}



public:
	COMPONENTEX_API class UCameraComponent* GetCameraComponent() const;



protected:
	AActor* This;
};
