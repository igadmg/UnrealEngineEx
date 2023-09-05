#pragma once

#include "K2Node_ConstructObjectFromClass.h"

#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_ConstructObjectFromPool.generated.h"


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_ConstructObjectFromPool : public UK2Node_ConstructObjectFromClass
{
	GENERATED_BODY()


public:
	UK2Node_ConstructObjectFromPool(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

