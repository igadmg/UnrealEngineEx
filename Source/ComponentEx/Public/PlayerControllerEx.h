#pragma once

#include "GameFramework/PlayerController.h"


struct COMPONENTEX_API FPlayerControllerEx
{
	FPlayerControllerEx(APlayerController* This)
		: This(This)
	{
	}


public:
	FVector2D GetViewportFoV() const;
	FVector2D GetViewportSize() const;


protected:
	APlayerController* This;
};

