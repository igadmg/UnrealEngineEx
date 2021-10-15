#pragma once

#include "UObject/Interface.h"

#include "PropertyVisitorInterface.generated.h"


UINTERFACE(Blueprintable)
class UNREALENGINEEX_API UPropertyVisitorInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALENGINEEX_API IPropertyVisitorInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void VisitBoolProperty(class UBoolPropertyAccessor* Property);
};

