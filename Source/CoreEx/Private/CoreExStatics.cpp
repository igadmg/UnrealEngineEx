#include "CoreExStatics.h"



UClass* UCoreExStatics::GetSelfClass(const UObject* WorldContextObject)
{
	return WorldContextObject ? WorldContextObject->GetClass() : nullptr;
}

TArray<UObject*> UCoreExStatics::ArrayFilter(TArray<UObject*>& Array, FObjectArrayPredicate Predicate)
{
	return Array.FilterByPredicate([Predicate](UObject* Item) { return Predicate.Execute(Item); });
}
