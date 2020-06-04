#include "Reflection/ReflectionStatics.h"



bool UReflectionStatics::IsValid(FPropertyAccessor& Property)
{
	return Property.IsValid();
}

void UReflectionStatics::MakePropertyAccessor(FPropertyAccessor& Property, UObject* Object, FName Name)
{
	Property = FPropertyAccessor(Object, Name);
}
