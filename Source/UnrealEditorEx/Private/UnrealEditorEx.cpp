#include "UnrealEditorExPrivatePCH.h"

#include "Core.h"
#include "ModuleManager.h"

#include "Editor/PeekEdMode.h"
#include "Editor/PeekEditorCommands.h"
#include "Visualizers/VisualizerComponentVisualizer.h"
#include "VisualizerComponent.h"



#define LOCTEXT_NAMESPACE "FUnrealEditorExModule"



void FUnrealEditorExModule::StartupModule()
{
	FPeekEditorCommands::Register();

	FEditorModeRegistry::Get().RegisterMode<FPeekEdMode>(
		FPeekEdMode::EM_PeekEdMode,
		FText::FromString("Peek"),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.ViewOptions", "LevelEditor.ViewOptions.Small"),
		true, 400
	);

	//FTestActDetailCustomization::RegestrateCostumization();

	if (IsValid(GUnrealEd))
	{
		TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FVisualizerComponentVisualizer());

		if (Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UVisualizerComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}
}

void FUnrealEditorExModule::ShutdownModule()
{
	if (!IsValid(GUnrealEd))
		return;

	GUnrealEd->UnregisterComponentVisualizer(UVisualizerComponent::StaticClass()->GetFName());
}



#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FUnrealEditorExModule, UnrealEditorEx)

DEFINE_LOG_CATEGORY(LogUnrealEditorEx);
//DEFINE_LOG_CATEGORY(LogUnrealEditorExInit);
//DEFINE_LOG_CATEGORY(LogUnrealEditorExCriticalErrors);
