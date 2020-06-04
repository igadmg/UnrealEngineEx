#include "Visualizers/VisualizerComponentVisualizer.h"

#include "Interfaces/VisualizerInterface.h"
#include "PrimitiveDrawInterface.h"
#include "VisualizerComponent.h"

#include "UnrealEngineEx.h"



FVisualizerComponentVisualizer::FVisualizerComponentVisualizer()
{
	uPDI = NewObject<UPrimitiveDrawInterface>();
	uPDI->AddToRoot();
}

FVisualizerComponentVisualizer::~FVisualizerComponentVisualizer()
{
	uPDI->RemoveFromRoot();
}

void FVisualizerComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	uPDI->PDI = PDI;
	UVisualizerComponent* VisualizerComponent = const_cast<UVisualizerComponent*>(Cast<UVisualizerComponent>(Component));
	FProperty* VisualizedComponentProp = VisualizerComponent->GetClass()->FindPropertyByName("VisualizedComponent");
	bool bDoesImplementInterface = FUnrealEngineEx::DoesImplementInterface<UVisualizerInterface>(VisualizerComponent);

	VisualizerComponent->ForEachVisualizedComponent([this](UActorComponent* VisualizedComponent) {
		IVisualizerInterface::Execute_DrawVisualization(VisualizedComponent, uPDI);
	}, [=](UActorComponent* VisualizedComponent) {
		if (bDoesImplementInterface)
		{
			if (VisualizedComponentProp != nullptr)
			{
				UObject** ValuePtr = VisualizedComponentProp->ContainerPtrToValuePtr<UObject*>(VisualizerComponent);
				if (ValuePtr != nullptr)
				{
					*ValuePtr = VisualizedComponent;
				}
			}
			IVisualizerInterface::Execute_DrawVisualization(VisualizerComponent, uPDI);
		}
	});
}

void FVisualizerComponentVisualizer::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	UVisualizerComponent* VisualizerComponent = const_cast<UVisualizerComponent*>(Cast<UVisualizerComponent>(Component));
	FProperty* VisualizedComponentProp = VisualizerComponent->GetClass()->FindPropertyByName("VisualizedComponent");
	bool bDoesImplementInterface = FUnrealEngineEx::DoesImplementInterface<UVisualizerInterface>(VisualizerComponent);

	VisualizerComponent->ForEachVisualizedComponent([](UActorComponent* VisualizedComponent) {
		IVisualizerInterface::Execute_DrawVisualizationHUD(VisualizedComponent);
	}, [=](UActorComponent* VisualizedComponent) {
		if (bDoesImplementInterface)
		{
			if (VisualizedComponentProp != nullptr)
			{
				UObject** ValuePtr = VisualizedComponentProp->ContainerPtrToValuePtr<UObject*>(VisualizerComponent);
				if (ValuePtr != nullptr)
				{
					*ValuePtr = VisualizedComponent;
				}
			}
			IVisualizerInterface::Execute_DrawVisualizationHUD(VisualizerComponent);
		}
	});
}
