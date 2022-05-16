#pragma once

#include "ExPrologue.h"
#include "Components/ArrowComponent.h"


DECLARE_CONST_EXTENSION(UArrowComponent)
	FVector GetArrowVector() const
	{
		return This()->GetForwardVector() * This()->ArrowLength;
	}
};

DECLARE_MUTABLE_EXTENSION(UArrowComponent)
};

DECLARE_EXTENSION(UArrowComponent);