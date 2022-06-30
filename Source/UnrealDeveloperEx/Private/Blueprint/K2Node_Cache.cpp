#include "Blueprint/K2Node_Cache.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "ScopedTransaction.h"
#include "ToolMenu.h"

#include "K2Node_FunctionEntry.h"
#include "K2Node_VariableGet.h"

#include "UnrealDeveloperEx.final.h"

#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


IMPLEMENT_PIN(UK2Node_Cache, InputObject, " ");
IMPLEMENT_PIN(UK2Node_Cache, OutputObject, "  ");
IMPLEMENT_SCHEMA_PIN(UK2Node_Cache, Then, UEdGraphSchema_K2::PN_Then);

UK2Node_Cache::UK2Node_Cache(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputObjectType.Add(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard));
}

FText UK2Node_Cache::GetTooltipText() const
{
	return LOCTEXT("K2Node_Cache_Tooltip", "Cache value of an object in local variable if needed.");
}

FText UK2Node_Cache::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_Cache_Title", "Cache");
}

FSlateIcon UK2Node_Cache::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Macro.IsValid_16x");
	return Icon;
}

FText UK2Node_Cache::GetMenuCategory() const
{
	static FNodeTextCache CachedCategory;
	if (CachedCategory.IsOutOfDate(this))
	{
		// FText::Format() is slow, so we cache this to save on performance
		CachedCategory.SetCachedText(FEditorCategoryUtils::BuildCategoryString(FCommonEditorCategory::Utilities, FText()), this);
	}
	return CachedCategory;
}

void UK2Node_Cache::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

void UK2Node_Cache::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("K2NodeCache", LOCTEXT("K2Node_Cache_ContextMenu", "Cache"));

		if (Context->Pin != NULL)
		{
			if (Context->Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec)
			{
				Section.AddMenuEntry(
					"RemovePin",
					LOCTEXT("RemovePin", "Remove array element pin"),
					LOCTEXT("RemovePinTooltip", "Remove this array element pin"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject(const_cast<UK2Node_Cache*>(this), &UK2Node_Cache::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
					)
				);
			}
		}
		else
		{
			Section.AddMenuEntry(
				"AddPin",
				LOCTEXT("AddPin", "Add array element pin"),
				LOCTEXT("AddPinTooltip", "Add another array element pin"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_Cache*>(this), &UK2Node_Cache::InteractiveAddInputPin)
				)
			);
		}

		Section.AddMenuEntry(
			"ResetToWildcard",
			LOCTEXT("ResetToWildcard", "Reset to wildcard"),
			LOCTEXT("ResetToWildcardTooltip", "Reset the node to have wildcard input/outputs. Requires no pins are connected."),
			FSlateIcon(),
			FUIAction(
				//FExecuteAction::CreateUObject(const_cast<UK2Node_Cache*>(this), &UK2Node_Cache::ClearPinTypeToWildcard),
				//FCanExecuteAction::CreateUObject(this, &UK2Node_MakeArray::CanResetToWildcard)
			)
		);
	}
}

bool UK2Node_Cache::ShouldCacheValue(int Index) const
{
	switch (CachingMode)
	{
	case ECachingMode::Force: return true;
	case ECachingMode::None: return false;
	}

	if (auto LinkedObjectPinNode = FK2NodeHelpers::GetLinkedPinNode(GetInputObjectPin(Index)))
	{
		if (LinkedObjectPinNode->IsA<UK2Node_VariableGet>())
			return false;
		if (LinkedObjectPinNode->IsA<UK2Node_FunctionEntry>())
			return false;

		if (auto AsFunctionCall = Valid<UK2Node_CallFunction>(LinkedObjectPinNode))
		{
			return AsFunctionCall->bIsPureFunc;
		}

		return GetOutputObjectPin(Index)->LinkedTo.Num() > 1;
	}

	return false;
}

UEdGraphPin* UK2Node_Cache::GetInputObjectPin(int Index) const
{
	if (Index == 0)
		return GetInputObjectPin();

	int InputObjectPinIndex = Pins.IndexOfByKey(GetInputObjectPin()) + Index;
	return Pins[InputObjectPinIndex];
}

UEdGraphPin* UK2Node_Cache::GetOutputObjectPin(int Index) const
{
	if (Index == 0)
		return GetOutputObjectPin();

	int OutputObjectPinIndex = Pins.IndexOfByKey(GetOutputObjectPin()) + Index;
	return Pins[OutputObjectPinIndex];
}

void UK2Node_Cache::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	AllocateInputObjectPins();

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	AllocateOutputObjectPins();

	Super::AllocateDefaultPins();
}

void UK2Node_Cache::AllocateInputObjectPins()
{
	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		auto PinName = i == 0 ? PN_InputObject : FName(FString::Printf(TEXT("%s %d"), *PN_InputObject.ToString(), i));
		CreatePin(EGPD_Input, InputObjectType[i], PinName);
	}
}

void UK2Node_Cache::AllocateOutputObjectPins()
{
	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		auto PinName = i == 0 ? PN_OutputObject : FName(FString::Printf(TEXT("%s %d"), *PN_OutputObject.ToString(), i));
		auto PinType = InputObjectType[i];

		CreatePin(EGPD_Output, PinType, PinName);
	}
}

void UK2Node_Cache::UpdateWildcardPins()
{
	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		InputObjectType[i] = FK2NodeHelpers::GetWildcardPinType(GetInputObjectPin(i));
		auto ValidPinType = InputObjectType[i];

		if_Valid(GetInputObjectPin(i))->PinType = InputObjectType[i];
		if_Valid(GetOutputObjectPin(i))->PinType = ValidPinType;
	}
}

void UK2Node_Cache::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin(), GetThenPin());

	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		auto ObjectPin = GetInputObjectPin(i);
		if (ShouldCacheValue(i))
		{
			ObjectPin = Compiler.CacheInLocalVariable(ObjectPin);
		}
		else
		{
			ObjectPin = Compiler.SpawnIntermediateNode<UK2Node_Knot>(ObjectPin)->GetOutputPin();
		}

		Compiler.ConnectPins(ObjectPin, GetOutputObjectPin(i));
	}
}

void UK2Node_Cache::ReconstructNode()
{
	Super::ReconstructNode();

	UpdateWildcardPins();
}

void UK2Node_Cache::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	UpdateWildcardPins();
}

#if WITH_EDITOR
void UK2Node_Cache::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ReconstructNode();
}
#endif // WITH_EDITOR


void UK2Node_Cache::InteractiveAddInputPin()
{
	FScopedTransaction Transaction(LOCTEXT("AddPinTx", "Add Pin"));
	AddInputPin();
}

void UK2Node_Cache::AddInputPin()
{
	//UpcastType.Add(nullptr);
	InputObjectType.Add(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass()));

	ReconstructNode();
}

bool UK2Node_Cache::CanRemovePin(const UEdGraphPin* Pin) const
{
	return InputObjectType.Num() > 1;
}

void UK2Node_Cache::RemoveInputPin(UEdGraphPin* Pin)
{
	//UpcastType.Add(nullptr);
	//InputObjectType.Add(nullptr);
}

#undef LOCTEXT_NAMESPACE
