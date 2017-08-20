#pragma once

#include "VisualizerInterface.generated.h"



UINTERFACE(Blueprintable)
class UNREALEDITOREX_API UVisualizerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class UNREALEDITOREX_API IVisualizerInterface
{
	GENERATED_IINTERFACE_BODY()



public:
	UFUNCTION(Category = "Visualizer", BlueprintNativeEvent)
	void DrawVisualization(class UPrimitiveDrawInterface* PDI) const;

	UFUNCTION(Category = "Visualizer", BlueprintNativeEvent)
	void DrawVisualizationHUD() const;
};
