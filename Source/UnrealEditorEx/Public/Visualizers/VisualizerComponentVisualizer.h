#pragma once



class FVisualizerComponentVisualizer : public FComponentVisualizer
{
public:
	FVisualizerComponentVisualizer();
	~FVisualizerComponentVisualizer();



public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;



protected:
	class UPrimitiveDrawInterface* uPDI;
};
