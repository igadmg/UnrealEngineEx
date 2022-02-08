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

void UPrimitiveDrawInterface::DrawArrowIn3D(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriorityGroup, float ArrowTipSizeInPercents, float ArrowTipHalfAngleInDegrees, float Thickness, float DepthBias, bool bScreenSpace)
{
	DrawLine(Start, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);

	float ArrowLength = (End - Start).Size();
	FVector TipProjectionOnArrow = Start + (1.f - ArrowTipSizeInPercents / 100.f) * ArrowLength * (End - Start).GetSafeNormal();
	float TipProjectionLength = ArrowTipSizeInPercents / 100.f * ArrowLength * FMath::Tan(PI / (180.f) * ArrowTipHalfAngleInDegrees);

	FVector TipProjectionDirection1 = FRotationMatrix((End - Start).ToOrientationRotator()).GetScaledAxis(EAxis::Z);
	FVector TipProjectionDirection2 = TipProjectionDirection1.RotateAngleAxis(90.f, (End - Start).GetSafeNormal());

	FVector Tip1_StartLoc = TipProjectionOnArrow + TipProjectionLength * TipProjectionDirection1;
	FVector Tip2_StartLoc = TipProjectionOnArrow + TipProjectionLength * TipProjectionDirection2;
	FVector Tip3_StartLoc = TipProjectionOnArrow + TipProjectionLength * TipProjectionDirection1 * -1.f;
	FVector Tip4_StartLoc = TipProjectionOnArrow + TipProjectionLength * TipProjectionDirection2 * -1.f;

	DrawLine(Tip1_StartLoc, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
	DrawLine(Tip2_StartLoc, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
	DrawLine(Tip3_StartLoc, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
	DrawLine(Tip4_StartLoc, End, Color, DepthPriorityGroup, Thickness, DepthBias, bScreenSpace);
}

void UPrimitiveDrawInterface::DrawArc(const FVector Base, const FVector X, const FVector Y, const float MinAngle, const float MaxAngle, const float Radius, const int32 Sections, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority, float Thickness, float DepthBias, bool bScreenSpace)
{
	DrawArc(PDI, Base, X, Y, MinAngle, MaxAngle, Radius, Sections, Color, DepthPriority, Thickness, DepthBias, bScreenSpace);
}

void UPrimitiveDrawInterface::DrawArc(FPrimitiveDrawInterface* InPDI, const FVector Base, const FVector X, const FVector Y, const float MinAngle, const float MaxAngle, const float Radius, const int32 Sections, const FLinearColor& Color, uint8 DepthPriority, float Thickness, float DepthBias, bool bScreenSpace)
{
	float AngleStep = (MaxAngle - MinAngle) / ((float)(Sections));
	float CurrentAngle = MinAngle;

	FVector LastVertex = Base + Radius * (FMath::Cos(CurrentAngle * (PI / 180.0f)) * X + FMath::Sin(CurrentAngle * (PI / 180.0f)) * Y);
	CurrentAngle += AngleStep;

	for (int32 i = 0; i < Sections; i++)
	{
		FVector ThisVertex = Base + Radius * (FMath::Cos(CurrentAngle * (PI / 180.0f)) * X + FMath::Sin(CurrentAngle * (PI / 180.0f)) * Y);
		InPDI->DrawLine(LastVertex, ThisVertex, Color, DepthPriority, Thickness, DepthBias, bScreenSpace);
		LastVertex = ThisVertex;
		CurrentAngle += AngleStep;
	}
}

void UPrimitiveDrawInterface::DrawArcByTwoPointsAndRadiusVector(const FVector A, const FVector B, const FVector R, const int32 Sections, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority, float Thickness, float DepthBias, bool bScreenSpace)
{
	auto C = UMathExStatics::SphereCenterByTwoPointsAndRadiusVector(A, B, R);
	float Angle = UMathExStatics::AngleBetweenVectors(A, B, C);
	auto X = A - C;
	//float Y =

	//::DrawArc(PDI, C, X, Y, MinAngle, MaxAngle, Radius, Sections, Color, DepthPriority, Thickness, DepthBias, bScreenSpace);
}
