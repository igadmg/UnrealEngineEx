#include "Blueprint/K2Node_DispatchEvents.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"

#include "CoreEx.h"


#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


IMPLEMENT_PIN(UK2Node_DispatchEvents, InputObject, "Input Object");

UK2Node_DispatchEvents::UK2Node_DispatchEvents(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UK2Node_DispatchEvents::GetTooltipText() const
{
	return LOCTEXT("K2Node_DispatchEvents_Tooltip", "Bind on objects events and expand them as Exec pins");
}

FText UK2Node_DispatchEvents::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_DispatchEvents_Title", "DispatchEvents");
}

/*
FSlateIcon UK2Node_DispatchEvents::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Macro.IsValid_16x");
	return Icon;
}
*/

FText UK2Node_DispatchEvents::GetMenuCategory() const
{
	static FNodeTextCache CachedCategory;
	if (CachedCategory.IsOutOfDate(this))
	{
		// FText::Format() is slow, so we cache this to save on performance
		CachedCategory.SetCachedText(FEditorCategoryUtils::BuildCategoryString(FCommonEditorCategory::Utilities, FText()), this);
	}
	return CachedCategory;
}

void UK2Node_DispatchEvents::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that
	// actions might have to be updated (or deleted) if their object-key is
	// mutated (or removed)... here we use the node's class (so if the node
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

UClass* UK2Node_DispatchEvents::GetInputObjectPinType() const
{
	if (auto ObjectPin = GetInputObjectPin())
	{
		if (ObjectPin->LinkedTo.Num() > 0)
		{
			auto ObjectPinType = ObjectPin->LinkedTo[0]->PinType;
			UClass* SourceClass = Cast<UClass>(ObjectPinType.PinSubCategoryObject.Get());
			if ((SourceClass == nullptr) && (ObjectPinType.PinSubCategory == UEdGraphSchema_K2::PSC_Self))
			{
				if (UK2Node* K2Node = Cast<UK2Node>(ObjectPin->GetOwningNode()))
				{
					SourceClass = K2Node->GetBlueprint()->GeneratedClass;
				}
			}

			return SourceClass;
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

void UK2Node_DispatchEvents::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	if (IsValid(InputObjectType))
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, InputObjectType, PN_InputObject);
	else
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass(), PN_InputObject);

	for (const auto& ExposedEvent : ExposedEvents)
	{
		if (ExposedEvent.Value == EEventDispatchType::Disabled)
			continue;

		auto EventProperty = EventsByName.FindChecked(ExposedEvent.Key);
		auto PropertyName = EventProperty->GetFName();
		if (ExposedEvent.Value == EEventDispatchType::Once)
			PropertyName = FName(*FString::Printf(TEXT("Once %s"), *EventProperty->GetName()));
		FK2NodeHelpers::CreateOutputPins(this, EventProperty, PropertyName);
	}

	Super::AllocateDefaultPins();
}

void UK2Node_DispatchEvents::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin());

	auto InputObjectPin = Compiler.SpawnIntermediateNode<UK2Node_Cache>(GetInputObjectPin())->GetOutputObjectPin();

	if (auto ObjectType = Valid(InputObjectType))
	{
		ex(ObjectType).ForEachOutputDelegate([this](auto Delegate) {
			if (FilterByClass && !ex(*FilterByClass).IsAssignableFrom(Delegate->GetOwnerClass()))
				return;

			auto DelegateName = Delegate->GetFName();
			EventsByName.Add(DelegateName, Delegate);
		});
	}

	for (auto ExposedEvent : ExposedEvents)
	{
		if (ExposedEvent.Value == EEventDispatchType::Disabled)
			continue;

		auto DelegateName = ExposedEvent.Key;
		//if (ExposedEvent.Value == EEventDispatchType::Once)
		//	DelegateName = FName(*FString::Printf(TEXT("Once %s"), *ExposedEvent.Key.ToString()));

		auto Delegate = EventsByName.FindChecked(DelegateName);
		auto DelegateAndPins = FDelegateAndPins::FindDelegatePins(this, Delegate);

		auto CustomDelegateName = FName(*FString::Printf(TEXT("%s_%s"), *DelegateAndPins.Delegate->GetName(), *Compiler.GetGuid()));
		auto CustomEventNode = Compiler.SpawnIntermediateEventNode<UK2Node_CustomEvent>(DelegateAndPins.ExecPin, CustomDelegateName, Delegate->SignatureFunction);
		{
			FK2NodeCompilerHelper CustomEventCompiler(Compiler, Compiler.GetThenPin(CustomEventNode), DelegateAndPins.ExecPin);

			for (auto OutputPin : DelegateAndPins.ParameterPins)
			{
				if (auto EventDataPin = CustomEventNode->FindPin(OutputPin->PinName))
				{
					CustomEventCompiler.ConnectPins(EventDataPin, OutputPin);
				}
			}

			if (ExposedEvent.Value == EEventDispatchType::Once)
			{
				CustomEventCompiler.FinishPin = nullptr;
				auto Sequence = CustomEventCompiler.SpawnIntermediateNode<UK2Node_ExecutionSequence>();
				CustomEventCompiler.ConnectPins(Sequence->GetThenPinGivenIndex(0), DelegateAndPins.ExecPin);

				CustomEventCompiler.LastThenPin = Sequence->GetThenPinGivenIndex(1);
				auto RemoveDelegate = CustomEventCompiler.SpawnIntermediateNode<UK2Node_RemoveDelegate>(DelegateAndPins.Delegate, InputObjectPin, CustomEventNode->FindPin(UK2Node_Event::DelegateOutputName));
			}
		}

		// Bind Custom Event to State Object delegate.
		auto AddDelegateNode = Compiler.SpawnIntermediateNode<UK2Node_AddDelegate>(DelegateAndPins.Delegate, InputObjectPin, CustomEventNode->FindPin(UK2Node_Event::DelegateOutputName));
	}
}

void UK2Node_DispatchEvents::ReconstructNode()
{
	InputObjectType = GetInputObjectPinType();

	auto PrevExposedEvents = ExposedEvents;
	ExposedEvents.Empty();
	EventsByName.Empty();

	if (auto ObjectType = Valid(InputObjectType))
	{
		ex(ObjectType).ForEachOutputDelegate([this, PrevExposedEvents](auto Delegate) {
			if (FilterByClass && !ex(*FilterByClass).IsAssignableFrom(Delegate->GetOwnerClass()))
				return;

			auto DelegateName = Delegate->GetFName();
			ExposedEvents.Add(DelegateName, ex(PrevExposedEvents).FindRef(DelegateName, EEventDispatchType::Permanent));
			EventsByName.Add(DelegateName, Delegate);
		});
	}

	Super::ReconstructNode();
}

void UK2Node_DispatchEvents::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetInputObjectPin())
	{
		ReconstructNode();
	}
}

#if WITH_EDITOR
void UK2Node_DispatchEvents::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ReconstructNode();
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
