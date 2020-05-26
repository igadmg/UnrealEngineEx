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

void ABlueprintableVolume::PostActorCreated()
{
	Super::PostActorCreated();

	if (IsValid(BrushBuilder))
	{
		InitializeBrush();
	}
}

#if WITH_EDITOR
void ABlueprintableVolume::PreEditChange(UProperty* PropertyAboutToChange)
{
	static FName NAME_BrushBuilder(TEXT("BrushBuilder"));

	if (PropertyAboutToChange && PropertyAboutToChange->GetFName() == NAME_BrushBuilder)
	{
		Brush = nullptr;
		GetBrushComponent()->Brush = nullptr;
	}

	Super::PreEditChange(PropertyAboutToChange);
}

void ABlueprintableVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static FName NAME_BrushBuilder(TEXT("BrushBuilder"));

	// The brush builder that created this volume has changed. Notify listeners
	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == NAME_BrushBuilder)
	{
		InitializeBrush();
	}

	if (PropertyChangedEvent.Property == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty: %s"), *GetFName().ToString());
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void ABlueprintableVolume::InitializeBrush()
{
	if (!IsValid(BrushBuilder))
	{
		Brush = nullptr;
		GetBrushComponent()->Brush = nullptr;

		return;
	}

	PolyFlags = 0;
	if (!Brush) Brush = NewObject<UModel>(this, NAME_None, RF_Transactional);
	if (!Brush->Polys)
	{
		Brush->Initialize(this, true);
		BrushBuilder->Build(GetWorld(), this);

		GetBrushComponent()->Brush = Brush;
		GetBrushComponent()->Bounds = Brush->Bounds;

		//FBSPOps::csgPrepMovingBrush(NewActor);

		// Set the texture on all polys to NULL.  This stops invisible textures
		// dependencies from being formed on volumes.
		for (int32 poly = 0; poly < Brush->Polys->Element.Num(); ++poly)
		{
			FPoly* Poly = &(Brush->Polys->Element[poly]);
			Poly->Material = nullptr;
		}
	}
}
