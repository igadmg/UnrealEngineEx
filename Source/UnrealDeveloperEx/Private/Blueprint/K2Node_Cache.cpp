#include "Blueprint/K2Node_Cache.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"

#include "K2Node_FunctionEntry.h"
#include "K2Node_VariableGet.h"

#include "ValidEx.h"


#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


IMPLEMENT_PIN(UK2Node_Cache, InputObject, " ");
IMPLEMENT_PIN(UK2Node_Cache, OutputObject, "  ");
IMPLEMENT_SCHEMA_PIN(UK2Node_Cache, Then, UEdGraphSchema_K2::PN_Then);

UK2Node_Cache::UK2Node_Cache(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InputObjectType(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard))
{
}

FText UK2Node_Cache::GetTooltipText() const
{
	return LOCTEXT("K2Node_Cache_Tooltip", "Cache value of an object in local variable if needed.");
}

FText UK2Node_Cache::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return ShouldCacheValue()
		? LOCTEXT("K2Node_Cache_Title", "Cached")
		: LOCTEXT("K2Node_Cache_Title", "Cache");
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

bool UK2Node_Cache::ShouldCacheValue() const
{
	if (auto LinkedObjectPinNode = FK2NodeHelpers::GetLinkedPinNode(GetInputObjectPin()))
	{
		if (LinkedObjectPinNode->IsA<UK2Node_VariableGet>())
			return false;
		if (LinkedObjectPinNode->IsA<UK2Node_FunctionEntry>())
			return false;

		if (auto AsFunctionCall = Valid<UK2Node_CallFunction>(LinkedObjectPinNode))
		{
			return AsFunctionCall->bIsPureFunc;
		}

		return GetOutputObjectPin()->LinkedTo.Num() > 1;
	}

	return false;
}

void UK2Node_Cache::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, InputObjectType, PN_InputObject);

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, InputObjectType, PN_OutputObject);
	
	Super::AllocateDefaultPins();
}

void UK2Node_Cache::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin(), GetThenPin());

	auto ObjectPin = GetInputObjectPin();
	if (ShouldCacheValue())
	{
		ObjectPin = Compiler.CacheInLocalVariable(ObjectPin);
	}
	else
	{
		ObjectPin = Compiler.SpawnIntermediateNode<UK2Node_Knot>(ObjectPin)->GetOutputPin();
	}

	Compiler.ConnectPins(ObjectPin, GetOutputObjectPin());
}

void UK2Node_Cache::ReconstructNode()
{
	InputObjectType = FK2NodeHelpers::GetWildcardPinType(GetInputObjectPin());

	Super::ReconstructNode();
}

void UK2Node_Cache::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetInputObjectPin())
	{
		InputObjectType = FK2NodeHelpers::GetWildcardPinType(GetInputObjectPin());

		GetInputObjectPin()->PinType = InputObjectType;
		GetOutputObjectPin()->PinType = InputObjectType;
	}
}

#if WITH_EDITOR
void UK2Node_Cache::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ReconstructNode();
}
#endif // WITH_EDITOR


#undef LOCTEXT_NAMESPACE 
