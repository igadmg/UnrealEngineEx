#include "UnrealEditorExPrivatePCH.h"
#include "PrimitiveDrawInterface.h"



UPrimitiveDrawInterface::UPrimitiveDrawInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPrimitiveDrawInterface::DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriorityGroup, float Thickness, float DepthBias, bool bScreenSpace)
{
	PDI->DrawLine(Start, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
}
