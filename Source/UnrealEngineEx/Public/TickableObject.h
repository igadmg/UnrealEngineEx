#pragma once

#include "TickableObject.generated.h"



UCLASS()
class UNREALENGINEEX_API UTickableObject : public UObject
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "UnrealEngineEx: Tickable", BlueprintNativeEvent)
	void Tick(float DeltaSeconds);



public:
	UTickableObject(const FObjectInitializer& ObjectInitializer);
};
