#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Reflection/PropertyAccessor.h"
#include "ReflectionStatics.generated.h"



UCLASS()
class UNREALENGINEEX_API UReflectionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "UnrealEngineEx|Reflection")
	static bool IsValid(UPARAM(ref) FPropertyAccessor& Property);

    UFUNCTION(BlueprintCallable, Category = "UnrealEngineEx|Reflection")
	static void MakePropertyAccessor(UPARAM(ref) FPropertyAccessor& Property, UObject* Object, FName Name);
};
