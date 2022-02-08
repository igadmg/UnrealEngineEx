#pragma once

#include "Engine/EngineTypes.h"
#include "Materials/MaterialInterface.h"

#include "PrimitiveDrawInterface.generated.h"



UCLASS(BlueprintType)
class UNREALEDITOREX_API UPrimitiveDrawInterface : public UObject
{
	GENERATED_BODY()


public:
	class FPrimitiveDrawInterface* PDI;
	UPrimitiveDrawInterface* Init(FPrimitiveDrawInterface* InPDI) { PDI = InPDI; return this; }


public:
	UFUNCTION(Category = "HitProxy", BlueprintCallable)
	void SetHitProxy(class UHitProxy* HitPoint);


public:
	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriorityGroup, float Thickness = 0.0f, float DepthBias = 0.0f, bool bScreenSpace = false);


public:
	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawPlane10x10(const FTransform& ObjectToWorld, float Radii, FVector2D UVMin, FVector2D UVMax, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawTriangle(const FVector& A, const FVector& B, const FVector& C, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriorityGroup);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawBox(const FTransform& BoxToWorld, const FVector& Radii, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawSphere(const FVector& Center, const FRotator& Orientation, const FVector& Radii, int32 NumSides, int32 NumRings, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority, bool bDisableBackfaceCulling = false);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawCone(const FTransform& ConeToWorld, float Angle1, float Angle2, int32 NumSides, bool bDrawSideLines, const FLinearColor& SideLineColor, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawCylinder(const FVector& Base, const FVector& XAxis, const FVector& YAxis, const FVector& ZAxis, float Radius, float HalfHeight, int32 Sides, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawFlatArrow(const FVector& Base, const FVector& XAxis, const FVector& YAxis, FColor Color, float Length, int32 Width, UMaterialInterface* MaterialInterface, ESceneDepthPriorityGroup DepthPriority, float Thickness);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawArrowIn3D(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 DepthPriorityGroup, float ArrowTipSizeInPercents = 10.0f, float ArrowTipHalfAngleInDegrees = 30.0f, float Thickness = 0.0f, float DepthBias = 0.0f, bool bScreenSpace = false);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawArc(const FVector Base, const FVector X, const FVector Y, const float MinAngle, const float MaxAngle, const float Radius, const int32 Sections, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority, float Thickness = 0.0f, float DepthBias = 0.0f, bool bScreenSpace = false);
	void DrawArc(FPrimitiveDrawInterface* PDI, const FVector Base, const FVector X, const FVector Y, const float MinAngle, const float MaxAngle, const float Radius, const int32 Sections, const FLinearColor& Color, uint8 DepthPriority, float Thickness, float DepthBias, bool bScreenSpace);

	UFUNCTION(Category = "Draw", BlueprintCallable)
	void DrawArcByTwoPointsAndRadiusVector(const FVector A, const FVector B, const FVector R, const int32 Sections, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority, float Thickness = 0.0f, float DepthBias = 0.0f, bool bScreenSpace = false);

	//void DrawCylinderRotate(const FMatrix& CylToWorld, const FVector& Base, const FVector& XAxis, const FVector& YAxis, const FVector& ZAxis, float Radius, float HalfHeight, uint32 Sides, uint8 DepthPriority);

	//void DrawCylinder(const FVector& Start, const FVector& End, float Radius, int32 Sides, uint8 DepthPriority);
};
