#include "UnrealEngineExPrivatePCH.h"
#include "BlueprintableVolume.h"



ABlueprintableVolume::ABlueprintableVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bColored = true;
	BrushColor.R = 100;
	BrushColor.G = 255;
	BrushColor.B = 100;
	BrushColor.A = 255;
}

#if WITH_EDITOR
void ABlueprintableVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static FName NAME_BrushBuilder(TEXT("BrushBuilder"));

	// The brush builder that created this volume has changed. Notify listeners
	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == NAME_BrushBuilder)
	{
		if (IsValid(BrushBuilder) && !IsValid(Brush))
		{
			PolyFlags = 0;
			Brush = NewObject<UModel>(this, NAME_None, RF_Transactional);
			Brush->Initialize(nullptr, true);
			Brush->Polys = NewObject<UPolys>(Brush, NAME_None, RF_Transactional);
			GetBrushComponent()->Brush = Brush;

			BrushBuilder->Build(GetWorld(), this);

			//FBSPOps::csgPrepMovingBrush(NewActor);

			// Set the texture on all polys to NULL.  This stops invisible textures
			// dependencies from being formed on volumes.
			if (IsValid(Brush))
			{
				for (int32 poly = 0; poly < Brush->Polys->Element.Num(); ++poly)
				{
					FPoly* Poly = &(Brush->Polys->Element[poly]);
					Poly->Material = nullptr;
				}
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR
