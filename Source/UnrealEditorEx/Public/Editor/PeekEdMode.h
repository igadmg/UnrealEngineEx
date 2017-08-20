#pragma once

#include "EdMode.h"



struct FPeekToolMode
{
	const FName	ToolModeName;

	TArray<FName> ValidTools;
	FName CurrentToolName;

	FPeekToolMode(FName InToolModeName)
		: ToolModeName(InToolModeName)
	{
	}
};



class UNREALEDITOREX_API FPeekEdMode : public FEdMode
{
public:
	static FEditorModeID EM_PeekEdMode;


public:
	FPeekToolMode* CurrentToolMode;
	void* CurrentTool;

	class UPeekEditorObject* UISettings;

	TArray<FPeekToolMode> PeekToolModes;



public:
	FPeekEdMode();
	~FPeekEdMode();

	void InitializeToolModes();

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;



public:
	TSharedRef<FUICommandList> GetUICommandList() const;

	void SetCurrentToolMode(FName ToolModeName, bool bRestoreCurrentTool = true);



public:
	virtual void Tick(FEditorViewportClient* ViewportClient,float DeltaTime) override;

	// FEdMode interface
	virtual bool UsesToolkits() const override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual bool InputKey(FEditorViewportClient* InViewportClient, FViewport* InViewport, FKey InKey, EInputEvent InEvent) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;

	//virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override; //Check to see if an actor can be selected in this mode - no side effects
	//virtual void ActorSelectionChangeNotify() override; //Called when the currently selected actor has changed
	//virtual bool ShouldDrawWidget() const override;
	//virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;
	// End of FEdMode interface

	//Render
	//void DrawPrevewGird(FPrimitiveDrawInterface* PDI);
	//void DrawBlankCells(FPrimitiveDrawInterface* PDI);

	//class ATestAct* EditorModel;
	//void ForceRealTimeViewports(const bool bEnable, const bool bStoreCurrentState);
	//TSharedPtr<class FCustomToolBase> GetActiveTool() const { return ActiveTool; }
	//void SetActiveTool(TSharedPtr<FCustomToolBase> ActiveTool);
	//void ApplyBrush(FEditorViewportClient* ViewportClient);

private:
	//UMaterial* OverlayMaterial;
	//void UpdateGridCursorPosition(const FSceneView* View, FViewport* Viewport);
	//ATestAct* FindTestActorOnScen();
	//TSharedPtr<class FCustomToolBase> ActiveTool;
	//FIntVector GridCursorPosition; 
	//bool bToolActive;
	//class SCustomEdit* EditPanel;
};
