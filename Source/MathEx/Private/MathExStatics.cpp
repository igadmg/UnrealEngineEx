#include "MathExStatics.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "SceneView.h"
#include "StereoRendering.h"
#include "UnrealClient.h"

#include "MathEx.h"



bool UMathExStatics::AreVertexOrdered(FVector A, FVector B, FVector C, ETriangleVertexOrder Order)
{
	float Sum = A.X * B.Y - A.Y * B.X;
	Sum += B.X * C.Y - B.Y * C.X;
	Sum += C.X * A.Y - C.Y * A.X;

	switch (Order)
	{
	case ETriangleVertexOrder::Clockwise:
		return Sum < 0;
	case ETriangleVertexOrder::CounterClockwise:
		return Sum > 0;
	}

	return false;
}

float UMathExStatics::AngleBetweenVectors(FVector A, FVector B, FVector O, ETriangleVertexOrder Order)
{
	bool bIsSameOrder = AreVertexOrdered(A, B, O, Order);
	auto OAn = bIsSameOrder ? (A - O).GetSafeNormal() : (B - O).GetSafeNormal();
	auto OBn = bIsSameOrder ? (B - O).GetSafeNormal() : (A - O).GetSafeNormal();

	float CosA = FVector::DotProduct(OAn, OBn);
	float SinA = FVector::CrossProduct(OAn, OBn).Size();

	return FMath::Atan2(SinA, CosA);
}

float UMathExStatics::RandomSameDigitsNumber(float Value)
{
	return FMathEx::RandomSameDigitsNumber(Value);
}

FVector UMathExStatics::SphereCenterByTwoPointsAndRadiusVector(FVector A, FVector B, FVector R)
{
	// https://math.stackexchange.com/a/1781546

	float RadiusSquared = R.SizeSquared();
	float ABDistanceSquared = (B - A).SizeSquared();

	FVector C = (A + B) * 0.5f;

	if (RadiusSquared <= ABDistanceSquared)
		return C;

	FVector Rn = R.GetSafeNormal();
	FVector CAn = (A - C).GetSafeNormal();
	float b = FMath::Sqrt(RadiusSquared - (C - A).SizeSquared());

	// Renormalize R vector so it is pependicular to AB
	FVector Nn = FVector::CrossProduct(CAn, Rn).GetSafeNormal();
	Rn = FVector::CrossProduct(Nn, CAn).GetSafeNormal();

	return C + Rn * b;
}

bool UMathExStatics::ProjectWorldToScreenBidirectional(APlayerController* PlayerController, FVector WorldLocation, FVector& ScreenLocation, bool& OutTargetBehindCamera)
{
	if (!IsValid(PlayerController))
		return false;

	FVector Projected;
	OutTargetBehindCamera = false;

	ULocalPlayer const* const LP = PlayerController->GetLocalPlayer();
	if (LP && LP->ViewportClient)
	{
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			const FMatrix ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			const FIntRect ViewRectangle = ProjectionData.GetConstrainedViewRect();

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldLocation, 1.f));
			if (Result.W < 0.f) { OutTargetBehindCamera = true; }
			if (Result.W == 0.f) { Result.W = 1.f; } // Prevent Divide By Zero

			const float RHW = 1.f / FMath::Abs(Result.W);
			Projected = FVector(Result.X, Result.Y, Result.Z) * RHW;

			// Normalize to 0..1 UI Space
			const float NormX = (Projected.X / 2.f) + 0.5f;
			const float NormY = 1.f - (Projected.Y / 2.f) - 0.5f;

			Projected.X = (float)ViewRectangle.Min.X + (NormX * (float)ViewRectangle.Width());
			Projected.Y = (float)ViewRectangle.Min.Y + (NormY * (float)ViewRectangle.Height());

			ScreenLocation = Projected;

			return true;
		}
	}

	return false;
}

bool UMathExStatics::ProjectWorldToScreen(APlayerController* PlayerController, FVector WorldLocation, FVector& ScreenLocation, bool& OutTargetBehindCamera)
{
	if (!IsValid(PlayerController) || !IsValid(PlayerController->GetPawn()))
		return false;

	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
	FVector2D ScreenLocation2D;

	auto PlayerPawn = PlayerController->GetPawn();
	FVector Forward = PlayerPawn->GetViewRotation().Vector();
	FVector ObjectDirection = (WorldLocation - PlayerPawn->GetActorLocation()).GetSafeNormal();

	float DotProduct = FVector::DotProduct(Forward, ObjectDirection);
	OutTargetBehindCamera = (DotProduct < 0);

	// object is behind the camera
	if (OutTargetBehindCamera)
	{
		FVector SideDirection = FVector::CrossProduct(Forward, FVector(0.0f, 0.0f, 1.0f));
		float SideProduct = FVector::DotProduct(ObjectDirection, SideDirection);

		ScreenLocation2D.X = (SideProduct > 0.0f) ? 0.0f : ViewportSize.X;
		ScreenLocation2D.Y = ViewportCenter.Y + FMath::Lerp(ViewportSize.Y / 2.0f, 0.0f, FMath::Abs(SideProduct));
		ScreenLocation = FVector(ScreenLocation2D.X, ScreenLocation2D.Y, 0.0f);
		return true;
	}

	PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation2D);

	// object is inside the screen
	if (ScreenLocation2D.X >= 0.0f && ScreenLocation2D.X <= ViewportSize.X &&
		ScreenLocation2D.Y >= 0.0f && ScreenLocation2D.Y <= ViewportSize.Y)
	{
		ScreenLocation = FVector(ScreenLocation2D.X, ScreenLocation2D.Y, 0.0f);
		return true;
	}

	ScreenLocation2D -= ViewportCenter;

	float AngleRadians = FMath::Atan2(ScreenLocation2D.Y, ScreenLocation2D.X);
	AngleRadians -= FMath::DegreesToRadians(90.0f);

	float Cos = FMath::Cos(AngleRadians);
	float Sin = -FMath::Sin(AngleRadians);
	float Cot = Cos / Sin;
	ScreenLocation2D = FVector2D(ViewportCenter.X + Sin * 150.f, ViewportCenter.Y + Cos * 150.f);

	if (Cos > 0)
	{
		ScreenLocation2D = FVector2D(ViewportCenter.Y / Cot, ViewportCenter.Y);
	}
	else
	{
		ScreenLocation2D = FVector2D(-ViewportCenter.Y / Cot, -ViewportCenter.Y);
	}

	if (ScreenLocation2D.X > ViewportCenter.X)
	{
		ScreenLocation2D = FVector2D(ViewportCenter.X, ViewportCenter.X * Cot);
	}
	else if (ScreenLocation2D.X < -ViewportCenter.X)
	{
		ScreenLocation2D = FVector2D(-ViewportCenter.X, -ViewportCenter.X * Cot);
	}

	ScreenLocation2D += ViewportCenter;
	ScreenLocation = FVector(ScreenLocation2D.X, ScreenLocation2D.Y, 0.0f);

	return true;
}

bool UMathExStatics::SuggestProjectileVelocityForTime(const UObject* WorldContextObject, FVector& OutVelocity, FVector Start, FVector End, float Time, float OverrideGravityZ)
{
	// https://www.forrestthewoods.com/blog/solving_ballistic_trajectories/

	const UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
		return false;

	const float GravityZ = FMath::IsNearlyEqual(OverrideGravityZ, 0.0f) ? -World->GetGravityZ() : -OverrideGravityZ;

	// g = -4*(Start.Z - 2*Zpeak + End.Z)/Time^2
	float Zpeak = (Start.Z + End.Z) / 2 + GravityZ * FMath::Square(Time) / 8;
	float Vz = -(3 * Start.Z - 4 * Zpeak + End.Z) / Time;
	FVector2d Vxy = FVector2d(End - Start) / Time;

	OutVelocity = FVector(Vxy, Vz);

	return true;
}
