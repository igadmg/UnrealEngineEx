#include "UnrealEditorExPrivatePCH.h"
#include "VisualizerComponent.h"

#include "Interfaces/VisualizerInterface.h"
#include "UnrealEngineEx.h"



UVisualizerComponent::UVisualizerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//bIsEditorOnly = true;
}

TArray<UActorComponent*> UVisualizerComponent::GetVisualizedComponents() const
{
	TArray<UActorComponent*> Result;

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
		return Result;

	for (UActorComponent* Component : Owner->GetComponents())
	{
		if (IsValid(Component) && Component->GetName().StartsWith(ComponentName))
		{
			Result.Add(Component);
		}
	}

	return Result;
}

void UVisualizerComponent::ForEachVisualizedComponent(TFunction<void(UActorComponent*)> IFunc, TFunction<void(UActorComponent*)> EFunc) const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
		return;

	for (UActorComponent* Component : Owner->GetComponents())
	{
		if (IsValid(Component) && Component->GetName().StartsWith(ComponentName))
		{
			if (FUnrealEngineEx::DoesImplementInterface<UVisualizerInterface>(Component))
			{
				IFunc(Component);
			}
			else
			{
				EFunc(Component);
			}
		}
	}
}
