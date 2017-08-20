#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"



class FPeekEditorCommands : public TCommands<FPeekEditorCommands>
{
public:
	static FName NAME_ToolMode_Place;



public:
	FPeekEditorCommands() : TCommands<FPeekEditorCommands>
	(
		"PeekEditor", // Context name for fast lookup
		NSLOCTEXT("Contexts", "PeekEditor", "Peek Editor"), // Localized context name for displaying
		NAME_None, //"LevelEditor" // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
	{
	}
	
	/**
	 * Initialize commands
	 */
	virtual void RegisterCommands() override;



public:
	// Mode Switch
	TSharedPtr<FUICommandInfo> PlaceMode;
	TSharedPtr<FUICommandInfo> SculptMode;
	TSharedPtr<FUICommandInfo> PaintMode;

	// Tools
	TSharedPtr<FUICommandInfo> NewLandscape;
	TSharedPtr<FUICommandInfo> ResizeLandscape;

	TSharedPtr<FUICommandInfo> SculptTool;
	TSharedPtr<FUICommandInfo> PaintTool;
	TSharedPtr<FUICommandInfo> SmoothTool;
	TSharedPtr<FUICommandInfo> FlattenTool;
	TSharedPtr<FUICommandInfo> RampTool;
	TSharedPtr<FUICommandInfo> ErosionTool;
	TSharedPtr<FUICommandInfo> HydroErosionTool;
	TSharedPtr<FUICommandInfo> NoiseTool;
	TSharedPtr<FUICommandInfo> RetopologizeTool;
	TSharedPtr<FUICommandInfo> VisibilityTool;

	TSharedPtr<FUICommandInfo> SelectComponentTool;
	TSharedPtr<FUICommandInfo> AddComponentTool;
	TSharedPtr<FUICommandInfo> DeleteComponentTool;
	TSharedPtr<FUICommandInfo> MoveToLevelTool;

	TSharedPtr<FUICommandInfo> RegionSelectTool;
	TSharedPtr<FUICommandInfo> RegionCopyPasteTool;
	TSharedPtr<FUICommandInfo> MirrorTool;

	TSharedPtr<FUICommandInfo> SplineTool;

	// Brushes
	TSharedPtr<FUICommandInfo> CircleBrush;
	TSharedPtr<FUICommandInfo> AlphaBrush;
	TSharedPtr<FUICommandInfo> AlphaBrush_Pattern;
	TSharedPtr<FUICommandInfo> ComponentBrush;
	TSharedPtr<FUICommandInfo> GizmoBrush;

	TSharedPtr<FUICommandInfo> CircleBrush_Smooth;
	TSharedPtr<FUICommandInfo> CircleBrush_Linear;
	TSharedPtr<FUICommandInfo> CircleBrush_Spherical;
	TSharedPtr<FUICommandInfo> CircleBrush_Tip;

	TSharedPtr<FUICommandInfo> ViewModeNormal;
	TSharedPtr<FUICommandInfo> ViewModeLOD;
	TSharedPtr<FUICommandInfo> ViewModeLayerDensity;
	TSharedPtr<FUICommandInfo> ViewModeLayerDebug;
	TSharedPtr<FUICommandInfo> ViewModeWireframeOnTop;
	TSharedPtr<FUICommandInfo> ViewModeLayerUsage;

	// Map
	TMap<FName, TSharedPtr<FUICommandInfo>> NameToCommandMap;
};
