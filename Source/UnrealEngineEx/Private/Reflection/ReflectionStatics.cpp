#include "Reflection/ReflectionStatics.h"



bool UReflectionStatics::IsValid(FUObjectPropertyAccessor& Property)
{
	return Property.IsValid();
}

void UReflectionStatics::MakePropertyAccessor(FUObjectPropertyAccessor& Property, UObject* Object, FName Name)
{
	Property = FUObjectPropertyAccessor(Object, Name);
}
