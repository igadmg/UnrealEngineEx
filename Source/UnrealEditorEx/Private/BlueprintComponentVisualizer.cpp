#include "BlueprintComponentVisualizer.h"

#include "HitProxies/ComponentVisProxy.h"

#include "Editor/EditorEngine.h"
#include "Editor.h"



UWorld* UBlueprintComponentVisualizer::GetWorld() const
{
	return GEditor ? GEditor->GetEditorWorldContext(false).World() : nullptr;
}


bool UBlueprintComponentVisualizer::DrawVisualization_Implementation(const UActorComponent* Component, UPrimitiveDrawInterface* PDI)
{
	return false;
}

bool UBlueprintComponentVisualizer::DrawVisualizationHUD_Implementation(const UActorComponent* Component, /*const class FViewport* Viewport, */class UCanvas* Canvas)
{
	return false;
}

bool UBlueprintComponentVisualizer::GetComponentLocation_Implementation(UEditorViewportClientProxy* ViewportClient, FVector& OutLocation)
{
	OutLocation = FVector::ZeroVector;
	return false;
}

bool UBlueprintComponentVisualizer::GetCustomInputCoordinateSystem_Implementation(UEditorViewportClientProxy* ViewportClient, FTransform& OutTransform)
{
	OutTransform = FTransform::Identity;
	return false;
}

bool UBlueprintComponentVisualizer::VisProxyHandleClick_Implementation(UEditorViewportClientProxy* ViewportClient, UComponentVisProxy* VisProxy, class UViewportClick* Click)
{
	return false;
}

bool UBlueprintComponentVisualizer::HandleInputDelta_Implementation(UEditorViewportClientProxy* ViewportClient, FVector DeltaTranslate, FRotator DeltaRotate, FVector DeltaScale)
{
	return false;
}

bool UBlueprintComponentVisualizer::HandleInputKey_Implementation(UEditorViewportClientProxy* ViewportClient, FKey Key, EInputEvent Event)
{
	return false;
}

UActorComponent* UBlueprintComponentVisualizer::GetEditedComponent_Implementation()
{
	if (RawComponentVisualizer && !bIsRecursiveCall_GetEditedComponent)
	{
		bIsRecursiveCall_GetEditedComponent = true;
		auto Result = RawComponentVisualizer->GetEditedComponent();
		bIsRecursiveCall_GetEditedComponent = false;
		return Result;
	}

	return nullptr;
}

bool UBlueprintComponentVisualizer::GenerateContextMenu_Implementation(class UMenuBuilder* MenuBuilder)
{
	return false;
}
