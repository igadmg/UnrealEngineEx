#include "CoreExStatics.h"

#include "CoreEx.h"


UClass* UCoreExStatics::GetSelfClass(const UObject* WorldContextObject)
{
	return WorldContextObject ? WorldContextObject->GetClass() : nullptr;
}
