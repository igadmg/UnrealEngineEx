#pragma once

#include "ExPrologue.h"
#include "GameFramework/PlayerController.h"


DECLARE_CONST_EXTENSION(APlayerController)
	FVector2D GetViewportFoV() const;
	FVector2D GetViewportSize() const;
};

DECLARE_MUTABLE_EXTENSION(APlayerController)
};

DECLARE_EXTENSION(APlayerController);
