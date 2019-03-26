#include "UnrealEditorExPrivatePCH.h"
#include "Editor/PeekEdMode.h"

#include "Editor/PeekEditorCommands.h"
#include "Editor/PeekEditorObject.h"
#include "SPeekEditor.h"

#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

/*
#include "ScopedTransaction.h"
#include "AssetData.h"

#include "Editor/UnrealEd/Public/Toolkits/ToolkitManager.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Editor/LevelEditor/Public/ILevelViewport.h"
#include "Engine/Selection.h"

#include "CustomEdModeToolkit.h"
#include "CustomTool.h"
#include "SCustomEdit.h"
*/


FEditorModeID FPeekEdMode::EM_PeekEdMode(TEXT("EM_PeekEdMode"));

FPeekEdMode::FPeekEdMode()
	: FEdMode()
	, CurrentToolMode(nullptr)
	, CurrentTool(nullptr)
//	, bToolActive(false)
{
	InitializeToolModes();

	UISettings = NewObject<UPeekEditorObject>(GetTransientPackage(), TEXT("UISettings"), RF_Transactional);
	UISettings->SetParent(this);

	UISettings->TraceChannel = ECC_Visibility;
}

FPeekEdMode::~FPeekEdMode()
{

}

void FPeekEdMode::InitializeToolModes()
{
	FPeekToolMode* ToolMode_Place = new(PeekToolModes)FPeekToolMode(FPeekEditorCommands::NAME_ToolMode_Place);
	/*
	ToolMode_Manage->ValidTools.Add(TEXT("NewLandscape"));
	ToolMode_Manage->ValidTools.Add(TEXT("Select"));
	ToolMode_Manage->ValidTools.Add(TEXT("AddComponent"));
	ToolMode_Manage->ValidTools.Add(TEXT("DeleteComponent"));
	ToolMode_Manage->ValidTools.Add(TEXT("MoveToLevel"));
	ToolMode_Manage->ValidTools.Add(TEXT("ResizeLandscape"));
	ToolMode_Manage->ValidTools.Add(TEXT("Splines"));
	ToolMode_Manage->CurrentToolName = TEXT("Select");

	FLandscapeToolMode* ToolMode_Sculpt = new(LandscapeToolModes)FLandscapeToolMode(TEXT("ToolMode_Sculpt"), ELandscapeToolTargetTypeMask::Heightmap | ELandscapeToolTargetTypeMask::Visibility);
	ToolMode_Sculpt->ValidTools.Add(TEXT("Sculpt"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Smooth"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Flatten"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Ramp"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Noise"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Erosion"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("HydraErosion"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Retopologize"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Visibility"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Mask"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("CopyPaste"));
	ToolMode_Sculpt->ValidTools.Add(TEXT("Mirror"));

	FLandscapeToolMode* ToolMode_Paint = new(LandscapeToolModes)FLandscapeToolMode(TEXT("ToolMode_Paint"), ELandscapeToolTargetTypeMask::Weightmap);
	ToolMode_Paint->ValidTools.Add(TEXT("Paint"));
	ToolMode_Paint->ValidTools.Add(TEXT("Smooth"));
	ToolMode_Paint->ValidTools.Add(TEXT("Flatten"));
	ToolMode_Paint->ValidTools.Add(TEXT("Noise"));
	ToolMode_Paint->ValidTools.Add(TEXT("Visibility"));
	*/
}

void FPeekEdMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdMode::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(UISettings);
}

TSharedRef<FUICommandList> FPeekEdMode::GetUICommandList() const
{
	check(Toolkit.IsValid());
	return Toolkit->GetToolkitCommands();
}

void FPeekEdMode::SetCurrentToolMode(FName ToolModeName, bool bRestoreCurrentTool)
{
	if (CurrentToolMode == NULL || ToolModeName != CurrentToolMode->ToolModeName)
	{
		for (int32 i = 0; i < PeekToolModes.Num(); ++i)
		{
			if (PeekToolModes[i].ToolModeName == ToolModeName)
			{
				CurrentToolMode = &PeekToolModes[i];
				if (bRestoreCurrentTool)
				{
					if (CurrentToolMode->CurrentToolName == NAME_None)
					{
						CurrentToolMode->CurrentToolName = CurrentToolMode->ValidTools[0];
					}
					//SetCurrentTool(CurrentToolMode->CurrentToolName);
				}

				break;
			}
		}
	}

}

bool FPeekEdMode::UsesToolkits() const
{
	return true;
}

/** FEdMode: Called when the mode is entered */
void FPeekEdMode::Enter()
{
	FEdMode::Enter();

	// Clear any selection 
	GEditor->SelectNone(false, true);

	// Force real-time viewports.  We'll back up the current viewport state so we can restore it when the
	// user exits this mode.
	//const bool bWantRealTime = true;
	//const bool bRememberCurrentState = true;
	//ForceRealTimeViewports(bWantRealTime, bRememberCurrentState);

	if (CurrentToolMode == nullptr)
	{
		SetCurrentToolMode(FPeekEditorCommands::NAME_ToolMode_Place, false);
		//SetCurrentTool("Sculpt");
	}
	else
	{
		//SetCurrentTool(CurrentToolMode->CurrentToolName);
	}

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FPeekToolKit);
		Toolkit->Init(Owner->GetToolkitHost());
		/*
		TSharedPtr<FCustomEdModeToolkit> ToolkitLoc = MakeShareable(new FCustomEdModeToolkit);
		Toolkit = ToolkitLoc;
		Toolkit->Init(Owner->GetToolkitHost());
		EditPanel = ToolkitLoc->GetWiget().Get();
		*/
	}
}

void FPeekEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	/*
	// Restore real-time viewport state if we changed it
	const bool bWantRealTime = false;
	const bool bRememberCurrentState = false;
	ForceRealTimeViewports(bWantRealTime, bRememberCurrentState);

	ActiveTool->Destroy();
	ActiveTool = nullptr;


	EditorModel = nullptr;
	*/
	
	FEdMode::Exit();
}

void FPeekEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	//if (bToolActive)
	//{
	//	ApplyBrush(ViewportClient);
	//}
	FEdMode::Tick(ViewportClient, DeltaTime);
}

/** FEdMode: Called when a key is pressed */
bool FPeekEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Key == EKeys::LeftMouseButton)
	{
//		bToolActive = (Event == EInputEvent::IE_Pressed);
	}

	bool bKeyProcessed = false;
//	if (ActiveTool.IsValid())
	{
//		bKeyProcessed |= ActiveTool->InputKey(SharedThis(this), ViewportClient, Viewport, Key, Event);
	}

	return bKeyProcessed;
}

bool FPeekEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click)
{
	if (Click.GetKey() == EKeys::LeftMouseButton)
	{
		// Cache a copy of the world pointer	
		UWorld* World = InViewportClient->GetWorld();

		// Compute a world space ray from the screen space mouse coordinates
		FSceneViewFamilyContext ViewFamily(
			FSceneViewFamilyContext::ConstructionValues(
				InViewportClient->Viewport, InViewportClient->GetScene(), InViewportClient->EngineShowFlags)
			.SetRealtimeUpdate(InViewportClient->IsRealtime()));

		FSceneView* View = InViewportClient->CalcSceneView(&ViewFamily);
		FViewportCursorLocation MouseViewportRay(View, InViewportClient, Click.GetClickPos().X, Click.GetClickPos().Y);
		FVector BrushTraceDirection = MouseViewportRay.GetDirection();

		FVector Start = MouseViewportRay.GetOrigin();
		if (InViewportClient->IsOrtho())
		{
			Start += -WORLD_MAX * BrushTraceDirection;
		}

		FVector End = Start + WORLD_MAX * BrushTraceDirection;

		FHitResult OutHit;
		if (World->LineTraceSingleByChannel(OutHit, Start, End, UISettings->TraceChannel))
		{
			UISettings->HitPont.SetLocation(OutHit.Location);
			UISettings->HitPont.SetRotation(FRotationMatrix::MakeFromX(OutHit.Normal).ToQuat());

			if (UISettings->ActorToMove.IsValid())
			{
				AActor* Actor = UISettings->ActorToMove.Get();
				Actor->Modify();
				Actor->SetActorTransform(UISettings->HitPont);
			}
		}

		return true;
	}

	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}


void FPeekEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FLinearColor SelectedColor = FLinearColor::White;
	SelectedColor.A = .6f;
	/** Call parent implementation */
	FEdMode::Render(View, Viewport, PDI);

	/*
	if (!IsRunningCommandlet()) {
		// Finds the cursor position in grid coordinates
		UpdateGridCursorPosition(View, Viewport);
		if (EditorModel)
		{
			DrawPrevewGird(PDI);
			DrawBlankCells(PDI);

			if (ActiveTool.IsValid())
			{
				ActiveTool->Render(View, Viewport, PDI, EditorModel, OverlayMaterial);
			}
		}
	}
	*/
}

#if 0
void FPeekEdMode::SetActiveTool(TSharedPtr<FCustomToolBase> NewTool)
{
	if (ActiveTool.IsValid())
	{
		ActiveTool->Destroy();
	}
	ActiveTool = NewTool;
	ActiveTool->SetGridCursorPosition(GridCursorPosition);
}

/** Forces real-time perspective viewports */
void FPeekEdMode::ForceRealTimeViewports(const bool bEnable, const bool bStoreCurrentState)
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr< ILevelViewport > ViewportWindow = LevelEditorModule.GetFirstActiveViewport();
	if (ViewportWindow.IsValid())
	{
		FEditorViewportClient &Viewport = ViewportWindow->GetLevelViewportClient();
		if (Viewport.IsPerspective())
		{
			if (bEnable)
			{
				Viewport.SetRealtime(bEnable, bStoreCurrentState);
			}
			else
			{
				const bool bAllowDisable = true;
				Viewport.RestoreRealtime(bAllowDisable);
			}

		}
	}
}


/** FEdMode: Check to see if an actor can be selected in this mode - no side effects */
bool FPeekEdMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	if (ActiveTool.IsValid())
	{
		if (ActiveTool->GetToolType() == EToolType::Select)
		{
			return true;
		}
	}
	if (InActor->IsA(ATestAct::StaticClass()))
	{
		return true;
	}
	return false;
}

/** FEdMode: Called when the currently selected actor has changed */
void FPeekEdMode::ActorSelectionChangeNotify()
{
	
	for (FSelectionIterator SelectionIt(*Owner->GetSelectedActors()); SelectionIt; ++SelectionIt)
	{
		AActor* CurActor = CastChecked<AActor>(*SelectionIt);

		if (CurActor->bHidden || !CurActor->IsSelected())
		{
			continue;
		}

		if (CurActor->IsA(ATestAct::StaticClass()))
		{
			ATestAct* ActiveActor = Cast<ATestAct>(CurActor);
			EditorModel = ActiveActor;
			if (EditorModel->ModeSettings == NULL)
			{
				EditorModel->ModeSettings = NewObject<UModeSettings>(EditorModel);
				EditorModel->ModeSettings->SetActorName(EditorModel);
			}
			EditPanel->SetDetailPanelProperty(EditorModel->ModeSettings); // Set property panel
		}
	}
		
}




void FPeekEdMode::DrawPrevewGird(FPrimitiveDrawInterface* PDI)
{
	FVector2D Size;
	FVector2D Begin;
	FVector P0, P1;
	P0.Z = 0;
	P1.Z = 0;
	const FColor GirdColor = FColor::Black;
	const int32 GirdThickness = 5;

	Size.X = fabs((EditorModel->ModeSettings->Point_1.X - EditorModel->ModeSettings->Point_2.X));
	Size.Y = fabs((EditorModel->ModeSettings->Point_1.Y - EditorModel->ModeSettings->Point_2.Y));

	if (EditorModel->ModeSettings->Point_1.X < EditorModel->ModeSettings->Point_2.X)
	{
		Begin.X = EditorModel->ModeSettings->Point_1.X;
	}
	else
	{
		Begin.X = EditorModel->ModeSettings->Point_2.X;
	}
	if (EditorModel->ModeSettings->Point_1.Y < EditorModel->ModeSettings->Point_2.Y)
	{
		Begin.Y = EditorModel->ModeSettings->Point_1.Y;
	}
	else
	{
		Begin.Y = EditorModel->ModeSettings->Point_2.Y;
	}

	for (int i = 0; i <= Size.X; i++)
	{
		P0.X = (Begin.X + i) * EditorModel->GridCellSize.X;
		P1.X = P0.X;
		P0.Y = Begin.Y * EditorModel->GridCellSize.Y;
		P1.Y = (Begin.Y + Size.Y) * EditorModel->GridCellSize.Y;
		PDI->DrawLine(P0, P1, GirdColor, SDPG_Foreground, GirdThickness, 0, true);
	}
	for (int i = 0; i <= Size.Y; i++)
	{
		P0.Y = (Begin.Y + i) * EditorModel->GridCellSize.Y;
		P1.Y = P0.Y;
		P0.X = Begin.X * EditorModel->GridCellSize.X;
		P1.X = (Begin.X + Size.X) * EditorModel->GridCellSize.X;
		PDI->DrawLine(P0, P1, GirdColor, SDPG_Foreground, GirdThickness, 0, true);
	}
}

void FPeekEdMode::DrawBlankCells(FPrimitiveDrawInterface* PDI)
{
	FDynamicMeshBuilder MeshBuilder;
	FDynamicColoredMaterialRenderProxy* SelectedColorInstance = new FDynamicColoredMaterialRenderProxy(OverlayMaterial->GetRenderProxy(false), FLinearColor::White);
	PDI->RegisterDynamicResource(SelectedColorInstance);

	
	//UE_LOG(LevelDrawEditor, Warning, TEXT("BeginLoc X = %f y = %f "), BeginLoc.X, BeginLoc.Y);
	//UE_LOG(LevelDrawEditor, Warning, TEXT("EndLoc X = %f y = %f "), EndLoc.X, EndLoc.Y);
	for (int i = -EditorModel->LevelMap.Begin.X; i <= EditorModel->LevelMap.End.X; i++)
	{
		for (int j = -EditorModel->LevelMap.Begin.Y; j <= EditorModel->LevelMap.End.Y; j++)
		{
			
			if (EditorModel->LevelMap.GetCell(i, j))
			{
				ActiveTool->AddQuad(MeshBuilder, FVector2D(i, j), FVector2D(1, 1), EditorModel->GridCellSize, FColor::Red);
			}
			else
			{
				ActiveTool->AddQuad(MeshBuilder, FVector2D(i, j), FVector2D(1, 1), EditorModel->GridCellSize, FColor::Green);
			}
		}
	}

	MeshBuilder.Draw(PDI, FMatrix::Identity, SelectedColorInstance, SDPG_Foreground, true, false);
}




bool FPeekEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	if (ActiveTool->GetToolType() != EToolType::Select)
	{
		return true;
	}

	return FEdMode::InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
}





void FPeekEdMode::ApplyBrush(FEditorViewportClient* ViewportClient)
{
	if (ActiveTool.IsValid())
	{
		ActiveTool->ApplyBrush(ViewportClient);
	}
}

void FPeekEdMode::UpdateGridCursorPosition(const FSceneView* View, FViewport* Viewport)
{
	if (!EditorModel) return;
	const FVector& GridCellSize = EditorModel->GridCellSize;

	FVector2D MousePosition(Viewport->GetMouseX(), Viewport->GetMouseY());
	FVector WorldOrigin, WorldDirection;
	View->DeprojectFVector2D(MousePosition, WorldOrigin, WorldDirection);
	FVector LineStart = WorldOrigin;
	FVector LineEnd = WorldOrigin + WorldDirection * 10000;
	FVector PlaneOrigin(0, 0, 0);
	FVector IntersectionPoint = FMath::LinePlaneIntersection(LineStart, LineEnd, PlaneOrigin, FVector(0, 0, 1));

	// Round this off
	int32 GridX = FMath::FloorToInt(IntersectionPoint.X / GridCellSize.X);
	int32 GridY = FMath::FloorToInt(IntersectionPoint.Y / GridCellSize.Y);
	int32 GridZ = 0; 

	GridCursorPosition = FIntVector(GridX, GridY, GridZ);

	if (ActiveTool.IsValid())
	{
		ActiveTool->SetGridCursorPosition(GridCursorPosition);
	}
}



bool FPeekEdMode::ShouldDrawWidget() const
{
	if (ActiveTool.IsValid())
	{
		if (ActiveTool->GetToolType() == EToolType::Select)
		{
			return true;
		}
	}
	return false;
}

#endif
