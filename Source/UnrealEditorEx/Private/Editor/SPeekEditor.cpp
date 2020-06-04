#include "Editor/SPeekEditor.h"

#include "Editor/PeekEditorCommands.h"
#include "Editor/PeekEditorObject.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxDefs.h"
#include "EditorModeManager.h"
#include "PropertyEditorModule.h"
#include "SlateOptMacros.h"


#define LOCTEXT_NAMESPACE "PeekEditor"

#if 0

void SLandscapeAssetThumbnail::Construct(const FArguments& InArgs, UObject* Asset, TSharedRef<FAssetThumbnailPool> ThumbnailPool)
{
	FIntPoint ThumbnailSize = InArgs._ThumbnailSize;

	AssetThumbnail = MakeShareable(new FAssetThumbnail(Asset, ThumbnailSize.X, ThumbnailSize.Y, ThumbnailPool));

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(ThumbnailSize.X)
		.HeightOverride(ThumbnailSize.Y)
		[
			AssetThumbnail->MakeThumbnailWidget()
		]
	];

	UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(Asset);
	if (MaterialInterface)
	{
		UMaterial::OnMaterialCompilationFinished().AddSP(this, &SLandscapeAssetThumbnail::OnMaterialCompilationFinished);
	}
}

SLandscapeAssetThumbnail::~SLandscapeAssetThumbnail()
{
	UMaterial::OnMaterialCompilationFinished().RemoveAll(this);
}

void SLandscapeAssetThumbnail::OnMaterialCompilationFinished(UMaterialInterface* MaterialInterface)
{
	UMaterialInterface* MaterialAsset = Cast<UMaterialInterface>(AssetThumbnail->GetAsset());
	if (MaterialAsset)
	{
		if (MaterialAsset->IsDependent(MaterialInterface))
		{
			// Refresh thumbnail
			AssetThumbnail->SetAsset(AssetThumbnail->GetAsset());
		}
	}
}

void SLandscapeAssetThumbnail::SetAsset(UObject* Asset)
{
	AssetThumbnail->SetAsset(Asset);
}

#endif

//////////////////////////////////////////////////////////////////////////

void FPeekToolKit::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
}

void FPeekToolKit::UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
}

void FPeekToolKit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	auto NameToCommandMap = FPeekEditorCommands::Get().NameToCommandMap;

	FPeekEdMode* PeekEdMode = GetEditorMode();
	TSharedRef<FUICommandList> CommandList = PeekEdMode->GetUICommandList();

#define MAP_MODE(ModeName) CommandList->MapAction(NameToCommandMap.FindChecked(ModeName), FUIAction(FExecuteAction::CreateSP(this, &FPeekToolKit::OnChangeMode, FName(ModeName)), FCanExecuteAction::CreateSP(this, &FPeekToolKit::IsModeEnabled, FName(ModeName)), FIsActionChecked::CreateSP(this, &FPeekToolKit::IsModeActive, FName(ModeName))));
	MAP_MODE("ToolMode_Place");
	//MAP_MODE("ToolMode_Sculpt");
	//MAP_MODE("ToolMode_Paint");
#undef MAP_MODE

#if 0
#define MAP_TOOL(ToolName) CommandList->MapAction(NameToCommandMap.FindChecked("Tool_" ToolName), FUIAction(FExecuteAction::CreateSP(this, &FLandscapeToolKit::OnChangeTool, FName(ToolName)), FCanExecuteAction::CreateSP(this, &FLandscapeToolKit::IsToolEnabled, FName(ToolName)), FIsActionChecked::CreateSP(this, &FLandscapeToolKit::IsToolActive, FName(ToolName))));
	MAP_TOOL("NewLandscape");
	MAP_TOOL("ResizeLandscape");

	MAP_TOOL("Sculpt");
	MAP_TOOL("Paint");
	MAP_TOOL("Smooth");
	MAP_TOOL("Flatten");
	MAP_TOOL("Ramp");
	MAP_TOOL("Erosion");
	MAP_TOOL("HydraErosion");
	MAP_TOOL("Noise");
	MAP_TOOL("Retopologize");
	MAP_TOOL("Visibility");

	MAP_TOOL("Select");
	MAP_TOOL("AddComponent");
	MAP_TOOL("DeleteComponent");
	MAP_TOOL("MoveToLevel");

	MAP_TOOL("Mask");
	MAP_TOOL("CopyPaste");
	MAP_TOOL("Mirror");

	MAP_TOOL("Splines");
#undef MAP_TOOL
#endif

	PeekEditorWidgets = SNew(SPeekEditor, SharedThis(this));

	FModeToolkit::Init(InitToolkitHost);
}

FName FPeekToolKit::GetToolkitFName() const
{
	return FName("LandscapeEditor");
}

FText FPeekToolKit::GetBaseToolkitName() const
{
	return LOCTEXT("ToolkitName", "Landscape Editor");
}

FPeekEdMode* FPeekToolKit::GetEditorMode() const
{
	return (FPeekEdMode*)GLevelEditorModeTools().GetActiveMode(FPeekEdMode::EM_PeekEdMode);
}

TSharedPtr<SWidget> FPeekToolKit::GetInlineContent() const
{
	return PeekEditorWidgets;
}

void FPeekToolKit::NotifyToolChanged()
{
	PeekEditorWidgets->NotifyToolChanged();
}

void FPeekToolKit::OnChangeMode(FName ModeName)
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode)
	{
		PeekEdMode->SetCurrentToolMode(ModeName);
	}
}

bool FPeekToolKit::IsModeEnabled(FName ModeName) const
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode)
	{
		/*
		// Manage is the only mode enabled if we have no landscape
		if (ModeName == "ToolMode_Manage" || PeekEdMode->GetLandscapeList().Num() > 0)
		{
			return true;
		}
		*/
	}

	return true;
}

bool FPeekToolKit::IsModeActive(FName ModeName) const
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode && PeekEdMode->CurrentToolMode)
	{
		return PeekEdMode->CurrentToolMode->ToolModeName == ModeName;
	}

	return false;
}

void FPeekToolKit::OnChangeTool(FName ToolName)
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode != nullptr)
	{
		//PeekEdMode->SetCurrentTool(ToolName);
	}
}

bool FPeekToolKit::IsToolEnabled(FName ToolName) const
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode != nullptr)
	{
		/*
		if (ToolName == "NewLandscape" || PeekEdMode->GetLandscapeList().Num() > 0)
		{
			return true;
		}
		*/
	}

	return false;
}

bool FPeekToolKit::IsToolActive(FName ToolName) const
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	/*
	if (PeekEdMode != nullptr && PeekEdMode->CurrentTool != nullptr)
	{
		const FName CurrentToolName = PeekEdMode->CurrentTool->GetToolName();
		return CurrentToolName == ToolName;
	}
	*/

	return false;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPeekEditor::Construct(const FArguments& InArgs, TSharedRef<FPeekToolKit> InParentToolkit)
{
	TSharedRef<FUICommandList> CommandList = InParentToolkit->GetToolkitCommands();

	// Modes:
	FToolBarBuilder ModeSwitchButtons(CommandList, FMultiBoxCustomization::None);
	{
		ModeSwitchButtons.AddToolBarButton(FPeekEditorCommands::Get().PlaceMode, NAME_None, LOCTEXT("Mode.Place", "Place"), LOCTEXT("Mode.Place.Tooltip", "Contains tools to place objects on surfaces."));
		//ModeSwitchButtons.AddToolBarButton(FLandscapeEditorCommands::Get().SculptMode, NAME_None, LOCTEXT("Mode.Sculpt", "Sculpt"), LOCTEXT("Mode.Sculpt.Tooltip", "Contains tools that modify the shape of a landscape"));
		//ModeSwitchButtons.AddToolBarButton(FLandscapeEditorCommands::Get().PaintMode,  NAME_None, LOCTEXT("Mode.Paint",  "Paint"),  LOCTEXT("Mode.Paint.Tooltip",  "Contains tools that paint materials on to a landscape"));
	}

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false,FDetailsViewArgs::HideNameArea);

	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsPanel->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateSP(this, &SPeekEditor::GetIsPropertyVisible));

	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode)
	{
		DetailsPanel->SetObject(PeekEdMode->UISettings);
	}

	//IIntroTutorials& IntroTutorials = FModuleManager::LoadModuleChecked<IIntroTutorials>(TEXT("IntroTutorials"));

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 5)
		[
			SAssignNew(Error, SErrorText)
		]
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SVerticalBox)
			.IsEnabled(this, &SPeekEditor::GetPeekEditorIsEnabled)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(4, 0, 4, 5)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
					.HAlign(HAlign_Center)
					[

						ModeSwitchButtons.MakeWidget()
					]
				]

				// Tutorial link
//				+ SOverlay::Slot()
//				.HAlign(HAlign_Right)
//				.VAlign(VAlign_Bottom)
//				.Padding(4)
//				[
//					IntroTutorials.CreateTutorialsWidget(TEXT("LandscapeMode"))
//				]
			]
			+ SVerticalBox::Slot()
			.Padding(0)
			[
				DetailsPanel.ToSharedRef()
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FPeekEdMode* SPeekEditor::GetEditorMode() const
{
	return (FPeekEdMode*)GLevelEditorModeTools().GetActiveMode(FPeekEdMode::EM_PeekEdMode);
}

FText SPeekEditor::GetErrorText() const
{
	const FPeekEdMode* PeekEdMode = GetEditorMode();
#if 0
	ELandscapeEditingState EditState = PeekEdMode->GetEditingState();
	switch (EditState)
	{
		case ELandscapeEditingState::SIEWorld:
		{

			if (LandscapeEdMode->NewLandscapePreviewMode != ENewLandscapePreviewMode::None)
			{
				return LOCTEXT("IsSimulatingError_create", "Can't create landscape while simulating!");
			}
			else
			{
				return LOCTEXT("IsSimulatingError_edit", "Can't edit landscape while simulating!");
			}
			break;
		}
		case ELandscapeEditingState::PIEWorld:
		{
			if (LandscapeEdMode->NewLandscapePreviewMode != ENewLandscapePreviewMode::None)
			{
				return LOCTEXT("IsPIEError_create", "Can't create landscape in PIE!");
			}
			else
			{
				return LOCTEXT("IsPIEError_edit", "Can't edit landscape in PIE!");
			}
			break;
		}
		case ELandscapeEditingState::BadFeatureLevel:
		{
			if (LandscapeEdMode->NewLandscapePreviewMode != ENewLandscapePreviewMode::None)
			{
				return LOCTEXT("IsFLError_create", "Can't create landscape with a feature level less than SM4!");
			}
			else
			{
				return LOCTEXT("IsFLError_edit", "Can't edit landscape with a feature level less than SM4!");
			}
			break;
		}
		case ELandscapeEditingState::NoLandscape:
		{
			return LOCTEXT("NoLandscapeError", "No Landscape!");
		}
		case ELandscapeEditingState::Enabled:
		{
			return FText::GetEmpty();
		}
		default:
			checkNoEntry();
	}
#endif

	return FText::GetEmpty();
}

bool SPeekEditor::GetPeekEditorIsEnabled() const
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode)
	{
		//Error->SetError(GetErrorText());
		//return PeekEdMode->GetEditingState() == ELandscapeEditingState::Enabled;
	}
	return true;
}

bool SPeekEditor::GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const
{
	const FProperty& Property = PropertyAndParent.Property;

	FPeekEdMode* PeekEdMode = GetEditorMode();
#if 0
	if (PeekEdMode != nullptr && PeekEdMode->CurrentTool != nullptr)
	{
		if (Property.HasMetaData("ShowForMask"))
		{
			const bool bMaskEnabled = LandscapeEdMode->CurrentTool &&
				LandscapeEdMode->CurrentTool->SupportsMask() &&
				LandscapeEdMode->CurrentToolTarget.LandscapeInfo.IsValid() &&
				LandscapeEdMode->CurrentToolTarget.LandscapeInfo->SelectedRegion.Num() > 0;

			if (bMaskEnabled)
			{
				return true;
			}
		}
		if (Property.HasMetaData("ShowForTools"))
		{
			const FName CurrentToolName = LandscapeEdMode->CurrentTool->GetToolName();

			TArray<FString> ShowForTools;
			Property.GetMetaData("ShowForTools").ParseIntoArray(ShowForTools, TEXT(","), true);
			if (!ShowForTools.Contains(CurrentToolName.ToString()))
			{
				return false;
			}
		}
		if (Property.HasMetaData("ShowForBrushes"))
		{
			const FName CurrentBrushSetName = LandscapeEdMode->LandscapeBrushSets[LandscapeEdMode->CurrentBrushSetIndex].BrushSetName;
			// const FName CurrentBrushName = LandscapeEdMode->CurrentBrush->GetBrushName();

			TArray<FString> ShowForBrushes;
			Property.GetMetaData("ShowForBrushes").ParseIntoArray(ShowForBrushes, TEXT(","), true);
			if (!ShowForBrushes.Contains(CurrentBrushSetName.ToString()))
				//&& !ShowForBrushes.Contains(CurrentBrushName.ToString())
			{
				return false;
			}
		}
		if (Property.HasMetaData("ShowForTargetTypes"))
		{
			static const TCHAR* TargetTypeNames[] = { TEXT("Heightmap"), TEXT("Weightmap"), TEXT("Visibility") };

			TArray<FString> ShowForTargetTypes;
			Property.GetMetaData("ShowForTargetTypes").ParseIntoArray(ShowForTargetTypes, TEXT(","), true);

			const ELandscapeToolTargetType::Type CurrentTargetType = LandscapeEdMode->CurrentToolTarget.TargetType;
			if (CurrentTargetType == ELandscapeToolTargetType::Invalid ||
				ShowForTargetTypes.FindByKey(TargetTypeNames[CurrentTargetType]) == nullptr)
			{
				return false;
			}
		}

		return true;
	}
#endif

	return true;
}

void SPeekEditor::NotifyToolChanged()
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode)
	{
		// Refresh details panel
		//DetailsPanel->SetObject(PeekEdMode->UISettings, true);
	}
}

void SPeekEditor::NotifyBrushChanged()
{
	FPeekEdMode* PeekEdMode = GetEditorMode();
	if (PeekEdMode)
	{
		// Refresh details panel
		//DetailsPanel->SetObject(LandscapeEdMode->UISettings, true);
	}
}

#undef LOCTEXT_NAMESPACE
