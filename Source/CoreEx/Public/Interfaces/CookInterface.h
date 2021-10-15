#pragma once

#include "UObject/Interface.h"

#include "CookInterface.generated.h"


UINTERFACE()
class COREEX_API UCookInterface : public UInterface
{
	GENERATED_BODY()
};

class COREEX_API ICookInterface
{
	GENERATED_BODY()


public:
	virtual bool OnWorldLoaded() { return false; }
	virtual bool OnWorldSaved() { return false; }
};
