#include "InterpolationExStatics.h"

#include "InterpolationEx.h"

#include "Interpolator.h"
#include "InterpolationTimer.h"

#include "Runtime/Launch/Resources/Version.h"



bool UInterpolationExStatics::IsFinished(UInterpolationTimer* Timer)
{
	if (!IsValid(Timer))
		return true;

	return Timer->IsFinished();
}

UInterpolationTimer* UInterpolationExStatics::MakeInterpolationTimer(float Duration, bool Cyclic)
{
	return ReuseInterpolationTimer(nullptr, Duration, Cyclic);
}

UInterpolatorFloat* UInterpolationExStatics::MakeFloatLinearInterpolator(float Start, float End)
{
	return ReuseFloatLinearInterpolator(nullptr, Start, End);
}

UInterpolatorVector2D* UInterpolationExStatics::MakeVector2DLinearInterpolator(FVector2D Start, FVector2D End)
{
	return ReuseVector2DLinearInterpolator(nullptr, Start, End);
}

UInterpolatorVector* UInterpolationExStatics::MakeVectorLinearInterpolator(FVector Start, FVector End)
{
	return ReuseVectorLinearInterpolator(nullptr, Start, End);
}

UInterpolatorRotator* UInterpolationExStatics::MakeRotatorLinearInterpolator(FRotator Start, FRotator End, bool bShortestPath)
{
	return ReuseRotatorLinearInterpolator(nullptr, Start, End, bShortestPath);
}

UInterpolatorColor* UInterpolationExStatics::MakeColorLinearInterpolator(FLinearColor Start, FLinearColor End)
{
	return ReuseColorLinearInterpolator(nullptr, Start, End);
}

UInterpolatorPostProcessSettings* UInterpolationExStatics::MakePostProcessSettingsLinearInterpolator(FPostProcessSettings Start, FPostProcessSettings End)
{
	return ReusePostProcessSettingsLinearInterpolator(nullptr, Start, End);
}



UInterpolationTimer* UInterpolationExStatics::ReuseInterpolationTimer(UInterpolationTimer* Object, float Duration, bool Cyclic)
{
	UInterpolationTimer* Result = Object ? Object : NewObject<UInterpolationTimer>();

	Result->bIsCyclic = Cyclic;
	Result->Duration = Duration;
	Result->CurrentTime = 0;

	return Result;
}

UInterpolatorFloat* UInterpolationExStatics::ReuseFloatLinearInterpolator(UInterpolatorFloat* Object, float Start, float End)
{
	UInterpolatorFloat* Result = Object ? Object : NewObject<UInterpolatorFloat>();

	Result->Start.Reset();
	Result->End.Reset();
	Result->Start.Add(Start);
	Result->End.Add(End);
	Result->CurveType = EInterpolatorType::IT_None;
	Result->CurveDegree = EInterpolatorDegree::ID_Linear;

	return Result;
}

UInterpolatorVector2D* UInterpolationExStatics::ReuseVector2DLinearInterpolator(UInterpolatorVector2D* Object, FVector2D Start, FVector2D End)
{
	UInterpolatorVector2D* Result = Object ? Object : NewObject<UInterpolatorVector2D>();

	Result->Start.Reset();
	Result->End.Reset();
	Result->Start.Add(Start);
	Result->End.Add(End);
	Result->CurveType = EInterpolatorType::IT_None;
	Result->CurveDegree = EInterpolatorDegree::ID_Linear;

	return Result;
}

UInterpolatorVector* UInterpolationExStatics::ReuseVectorLinearInterpolator(UInterpolatorVector* Object, FVector Start, FVector End)
{
	UInterpolatorVector* Result = Object ? Object : NewObject<UInterpolatorVector>();

	Result->Start.Reset();
	Result->End.Reset();
	Result->Start.Add(Start);
	Result->End.Add(End);
	Result->CurveType = EInterpolatorType::IT_None;
	Result->CurveDegree = EInterpolatorDegree::ID_Linear;

	return Result;
}

UInterpolatorRotator* UInterpolationExStatics::ReuseRotatorLinearInterpolator(UInterpolatorRotator* Object, FRotator Start, FRotator End, bool bShortestPath)
{
	UInterpolatorRotator* Result = Object ? Object : NewObject<UInterpolatorRotator>();

	Result->Start.Reset();
	Result->End.Reset();
	Result->Start.Add(Start);
	Result->End.Add(End);
	Result->bShortestPath = bShortestPath;
	Result->CurveType = EInterpolatorType::IT_None;
	Result->CurveDegree = EInterpolatorDegree::ID_Linear;

	return Result;
}

UInterpolatorColor* UInterpolationExStatics::ReuseColorLinearInterpolator(UInterpolatorColor* Object, FLinearColor Start, FLinearColor End)
{
	UInterpolatorColor* Result = Object ? Object : NewObject<UInterpolatorColor>();

	Result->Start.Reset();
	Result->End.Reset();
	Result->Start.Add(Start);
	Result->End.Add(End);
	Result->CurveType = EInterpolatorType::IT_None;
	Result->CurveDegree = EInterpolatorDegree::ID_Linear;

	return Result;
}

#define NORMALIZE_ZERO(Field, A, B) do { if (A.bOverride_##Field != B.bOverride_##Field) { if (A.bOverride_##Field) B.Field = 0; else A.Field = 0; } } while(false)

UInterpolatorPostProcessSettings* UInterpolationExStatics::ReusePostProcessSettingsLinearInterpolator(UInterpolatorPostProcessSettings* Object, FPostProcessSettings Start, FPostProcessSettings End)
{
	UInterpolatorPostProcessSettings* Result = Object ? Object : NewObject<UInterpolatorPostProcessSettings>();

	NORMALIZE_ZERO(DepthOfFieldFstop, Start, End);
	NORMALIZE_ZERO(DepthOfFieldSensorWidth, Start, End);
	NORMALIZE_ZERO(DepthOfFieldFocalDistance, Start, End);
	NORMALIZE_ZERO(DepthOfFieldDepthBlurAmount, Start, End);
	NORMALIZE_ZERO(DepthOfFieldDepthBlurRadius, Start, End);
	NORMALIZE_ZERO(DepthOfFieldFocalRegion, Start, End);
	NORMALIZE_ZERO(DepthOfFieldNearTransitionRegion, Start, End);
	NORMALIZE_ZERO(DepthOfFieldFarTransitionRegion, Start, End);
	NORMALIZE_ZERO(DepthOfFieldScale, Start, End);
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 21 || ENGINE_MAJOR_VERSION >= 5
#else
	NORMALIZE_ZERO(DepthOfFieldMaxBokehSize, Start, End);
#endif
	NORMALIZE_ZERO(DepthOfFieldNearBlurSize, Start, End);
	NORMALIZE_ZERO(DepthOfFieldFarBlurSize, Start, End);
	NORMALIZE_ZERO(DepthOfFieldOcclusion, Start, End);
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 21 || ENGINE_MAJOR_VERSION >= 5
#else
	NORMALIZE_ZERO(DepthOfFieldColorThreshold, Start, End);
	NORMALIZE_ZERO(DepthOfFieldSizeThreshold, Start, End);
#endif
	NORMALIZE_ZERO(DepthOfFieldSkyFocusDistance, Start, End);
	NORMALIZE_ZERO(DepthOfFieldVignetteSize, Start, End);

	Result->Start.Reset();
	Result->End.Reset();
	Result->Start.Add(Start);
	Result->End.Add(End);
	Result->CurveType = EInterpolatorType::IT_None;
	Result->CurveDegree = EInterpolatorDegree::ID_Linear;

	return Result;
}
