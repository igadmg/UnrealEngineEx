#pragma once

#include "UObject/Interface.h"

#include "CookInterface.generated.h"


UINTERFACE()
class COREEX_API UCookInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 Cook interface provide points for classes to do some cleanup work when levels are saved or loaded.
 Can be used to apply some changes or variable cleanup.
*/
class COREEX_API ICookInterface
{
	GENERATED_BODY()


public:
	virtual bool OnWorldLoaded() { return false; }
	virtual bool OnWorldSaved() { return false; }
};
