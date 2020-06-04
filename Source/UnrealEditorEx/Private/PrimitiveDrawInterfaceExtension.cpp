#include "PrimitiveDrawInterfaceExtension.h"

#include "SceneManagement.h"



void XPrimitiveDrawInterface::DrawCross(FPrimitiveDrawInterface* PDI, const FVector& Position, const FTransform& Transform, const FVector& Size
	, const FLinearColor& Color, uint8 DepthPriorityGroup, float Thickness, float DepthBias, bool bScreenSpace)
{
	FVector ForwardVector = Transform.TransformVectorNoScale(FVector::ForwardVector) * Size.X;
	FVector RightVector = Transform.TransformVectorNoScale(FVector::RightVector) * Size.Y;
	FVector UpVector = Transform.TransformVectorNoScale(FVector::UpVector) * Size.Z;

	PDI->DrawLine(Position - ForwardVector, Position + ForwardVector, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
	PDI->DrawLine(Position - RightVector, Position + RightVector, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
	PDI->DrawLine(Position - UpVector, Position + UpVector, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
}
