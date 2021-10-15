#include "UnrealEditorExStatics.h"

#include "Engine/Selection.h"
#include "Components/ActorComponent.h"
#include "Settings/LevelEditorPlaySettings.h"
#include "Editor.h"



void UUnrealEditorExStatics::SelectActorComponent(UActorComponent* InActorComponent)
{
	GEditor->GetSelectedComponents()->DeselectAll();
	if (IsValid(InActorComponent))
	{
		GEditor->SelectComponent(InActorComponent, true, true, true);
	}
}
