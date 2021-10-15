#include "PrimitiveDrawInterface.h"

#include "HitProxy.h"

#include "Editor/EditorEngine.h"
#include "Materials/Material.h"
#include "Editor.h"
#include "MathExStatics.h"
#include "SceneManagement.h"



#define GET_RENDERPROXY(Mat) (Mat ? Mat->GetRenderProxy() : GEngine->DebugEditorMaterial->GetRenderProxy())


void UPrimitiveDrawInterface::SetHitProxy(UHitProxy* HitProxy)
{
	PDI->SetHitProxy(HitProxy ? HitProxy->ToHHitProxy() : nullptr);
}

void UPrimitiveDrawInterface::DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriorityGroup, float Thickness, float DepthBias, bool bScreenSpace)
{
	PDI->DrawLine(Start, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
}

void UPrimitiveDrawInterface::DrawPlane10x10(const FTransform& ObjectToWorld, float Radii, FVector2D UVMin, FVector2D UVMax, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority)
{
	::DrawPlane10x10(PDI, ObjectToWorld.ToMatrixWithScale(), Radii, UVMin, UVMin, GET_RENDERPROXY(MaterialInterface), DepthPriority);
}

void UPrimitiveDrawInterface::DrawTriangle(const FVector& A, const FVector& B, const FVector& C, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriorityGroup)
{
	::DrawTriangle(PDI, A, B, C, GET_RENDERPROXY(MaterialInterface), DepthPriorityGroup);
}

void UPrimitiveDrawInterface::DrawBox(const FTransform& BoxToWorld, const FVector& Radii, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority)
{
	::DrawBox(PDI, BoxToWorld.ToMatrixWithScale(), Radii, GET_RENDERPROXY(MaterialInterface), DepthPriority);
}

void UPrimitiveDrawInterface::DrawSphere(const FVector& Center, const FRotator& Orientation, const FVector& Radii, int32 NumSides, int32 NumRings, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority, bool bDisableBackfaceCulling)
{
	::DrawSphere(PDI, Center, Orientation, Radii, NumSides, NumRings, GET_RENDERPROXY(MaterialInterface), DepthPriority, bDisableBackfaceCulling);
}

void UPrimitiveDrawInterface::DrawCone(const FTransform& ConeToWorld, float Angle1, float Angle2, int32 NumSides, bool bDrawSideLines, const FLinearColor& SideLineColor, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority)
{
	::DrawCone(PDI, ConeToWorld.ToMatrixWithScale(), Angle1, Angle2, NumSides, bDrawSideLines, SideLineColor, GET_RENDERPROXY(MaterialInterface), DepthPriority);
}

void UPrimitiveDrawInterface::DrawCylinder(const FVector& Base, const FVector& XAxis, const FVector& YAxis, const FVector& ZAxis, float Radius, float HalfHeight, int32 Sides, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority)
{
	::DrawCylinder(PDI, Base, XAxis, YAxis, ZAxis, Radius, HalfHeight, Sides, GET_RENDERPROXY(MaterialInterface), DepthPriority);
}

void UPrimitiveDrawInterface::DrawFlatArrow(const FVector& Base, const FVector& XAxis, const FVector& YAxis, FColor Color, float Length, int32 Width, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority, float Thickness)
{
	::DrawFlatArrow(PDI, Base, XAxis, YAxis, Color, Length, Width, GET_RENDERPROXY(MaterialInterface), DepthPriority, Thickness);
}

void UPrimitiveDrawInterface::DrawArc(const FVector Base, const FVector X, const FVector Y, const float MinAngle, const float MaxAngle, const float Radius, const int32 Sections, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority, float Thickness, float DepthBias, bool bScreenSpace)
{
	::DrawArc(PDI, Base, X, Y, MinAngle, MaxAngle, Radius, Sections, Color, DepthPriority, Thickness, DepthBias, bScreenSpace);
}

void UPrimitiveDrawInterface::DrawArcByTwoPointsAndRadiusVector(const FVector A, const FVector B, const FVector R, const int32 Sections, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority, float Thickness, float DepthBias, bool bScreenSpace)
{
	auto C = UMathExStatics::SphereCenterByTwoPointsAndRadiusVector(A, B, R);
	float Angle = UMathExStatics::AngleBetweenVectors(A, B, C);
	auto X = A - C;
	//float Y =

	//::DrawArc(PDI, C, X, Y, MinAngle, MaxAngle, Radius, Sections, Color, DepthPriority, Thickness, DepthBias, bScreenSpace);
}
