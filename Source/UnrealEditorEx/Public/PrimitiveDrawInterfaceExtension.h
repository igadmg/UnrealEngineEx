#pragma once



struct UNREALEDITOREX_API XPrimitiveDrawInterface
{
	static void DrawCross(class FPrimitiveDrawInterface* PDI, const FVector& Position, const FTransform& Transform, const FVector& Size
		, const FLinearColor& Color, uint8 DepthPriorityGroup, float Thickness = 0.0f, float DepthBias = 0.0f, bool bScreenSpace = false);
};
