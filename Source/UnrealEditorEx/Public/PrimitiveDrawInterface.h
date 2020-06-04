#pragma once

#include "PrimitiveDrawInterface.generated.h"



UCLASS()
class UNREALEDITOREX_API UPrimitiveDrawInterface : public UObject
{
	GENERATED_BODY()


public:
	class FPrimitiveDrawInterface* PDI;


public:
	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriorityGroup, float Thickness = 0.0f, float DepthBias = 0.0f, bool bScreenSpace = false);


public:
	UPrimitiveDrawInterface(const FObjectInitializer& ObjectInitializer);
};
