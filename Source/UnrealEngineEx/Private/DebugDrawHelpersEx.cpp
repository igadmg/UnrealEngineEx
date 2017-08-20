#include "UnrealEngineExPrivatePCH.h"
#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "Components/LineBatchComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"



#if ENABLE_DRAW_DEBUG

UNREALENGINEEX_API void DrawDebugFloatHistoryEx(UWorld const & WorldRef, FDebugFloatHistory const & FloatHistory, FTransform const & DrawTransform, FVector2D const & DrawSize, FColor const & DrawColor, bool const & bPersistent, float const & LifeTime, uint8 const & DepthPriority)
{
	int const NumSamples = FloatHistory.GetNumSamples();
	if (NumSamples >= 2)
	{
		FVector DrawLocation = DrawTransform.GetLocation();
		FVector const AxisX = DrawTransform.GetUnitAxis(EAxis::Y);
		FVector const AxisY = DrawTransform.GetUnitAxis(EAxis::Z);
		FVector const AxisXStep = AxisX * DrawSize.X / float(FloatHistory.MaxSamples - 1);
		FVector const AxisYStep = AxisY * DrawSize.Y / FMath::Max(FloatHistory.GetMinMaxRange(), KINDA_SMALL_NUMBER);

		// Frame
		DrawDebugLine(&WorldRef, DrawLocation, DrawLocation + AxisX * DrawSize.X, DrawColor, bPersistent, LifeTime, DepthPriority);
		DrawDebugLine(&WorldRef, DrawLocation, DrawLocation + AxisY * DrawSize.Y, DrawColor, bPersistent, LifeTime, DepthPriority);
		DrawDebugLine(&WorldRef, DrawLocation + AxisY * DrawSize.Y, DrawLocation + AxisX * DrawSize.X + AxisY * DrawSize.Y, DrawColor, bPersistent, LifeTime, DepthPriority);
		DrawDebugLine(&WorldRef, DrawLocation + AxisX * DrawSize.X, DrawLocation + AxisX * DrawSize.X + AxisY * DrawSize.Y, DrawColor, bPersistent, LifeTime, DepthPriority);

		TArray<float> const & Samples = FloatHistory.GetSamples();

		TArray<FVector> Verts;
		Verts.AddUninitialized(NumSamples * 2);

		TArray<int32> Indices;
		Indices.AddUninitialized((NumSamples - 1) * 6);

		float GraphHeight = FMath::Clamp(Samples[0], FloatHistory.GetMinValue(), FloatHistory.GetMaxValue()) - FloatHistory.GetMinValue();

		Verts[0] = DrawLocation;
		Verts[1] = DrawLocation + AxisYStep * GraphHeight;

		for (int HistoryIndex = 1; HistoryIndex < NumSamples; HistoryIndex++)
		{
			DrawLocation += AxisXStep;

			GraphHeight = FMath::Clamp(Samples[HistoryIndex], FloatHistory.GetMinValue(), FloatHistory.GetMaxValue()) - FloatHistory.GetMinValue();

			int const VertIndex = (HistoryIndex - 1) * 2;
			Verts[VertIndex + 2] = DrawLocation;
			Verts[VertIndex + 3] = DrawLocation + AxisYStep * GraphHeight;

			int const StartIndex = (HistoryIndex - 1) * 6;
			Indices[StartIndex + 0] = VertIndex + 0; Indices[StartIndex + 1] = VertIndex + 1; Indices[StartIndex + 2] = VertIndex + 3;
			Indices[StartIndex + 3] = VertIndex + 0; Indices[StartIndex + 4] = VertIndex + 3; Indices[StartIndex + 5] = VertIndex + 2;
		}

		DrawDebugMesh(&WorldRef, Verts, Indices, DrawColor, bPersistent, LifeTime, DepthPriority);
	}
}

#endif // ENABLE_DRAW_DEBUG
