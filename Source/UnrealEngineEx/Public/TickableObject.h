#pragma once

#include "TickableObject.generated.h"



UCLASS(blueprintable, BlueprintType)
class UNREALENGINEEX_API UTickableObject : public UObject
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "UnrealEngineEx: Tickable", BlueprintCallable, BlueprintNativeEvent)
	void Tick(float DeltaSeconds);



public:
	UTickableObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
