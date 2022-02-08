#include "UnrealEditorEx.h"

#include "Editor/UnrealEdEngine.h"
#include "Core.h"
#include "UnrealEdGlobals.h"



#define LOCTEXT_NAMESPACE "FUnrealEditorExModule"



void FUnrealEditorExModule::StartupModule()
{
#if 0
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
#endif
}

void FUnrealEditorExModule::ShutdownModule()
{
#if 0
	if (!IsValid(GUnrealEd))
		return;

	GUnrealEd->UnregisterComponentVisualizer(UVisualizerComponent::StaticClass()->GetFName());
#endif
}

#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FUnrealEditorExModule, UnrealEditorEx)

//DEFINE_LOG_CATEGORY(LogUnrealEditorEx)
//DEFINE_LOG_CATEGORY(LogUnrealEditorExInit)
//DEFINE_LOG_CATEGORY(LogUnrealEditorExCriticalErrors)
