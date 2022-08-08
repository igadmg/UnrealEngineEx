#include "Blueprint/K2Node_ForEachLoop.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"

#include "UnrealDeveloperEx.final.h"

#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


IMPLEMENT_PIN(UK2Node_ForEachLoop, Array, "Array");
IMPLEMENT_PIN(UK2Node_ForEachLoop, LoopBody, "Loop Body");
IMPLEMENT_PIN(UK2Node_ForEachLoop, ArrayElement, "Array Element");
IMPLEMENT_PIN(UK2Node_ForEachLoop, ArrayIndex, "Array Index");
IMPLEMENT_PIN(UK2Node_ForEachLoop, BreakFlag, "Break?");
IMPLEMENT_PIN(UK2Node_ForEachLoop, OutputElement, "Output Element");
IMPLEMENT_PIN(UK2Node_ForEachLoop, FilterFlag, "Keep?");
IMPLEMENT_PIN(UK2Node_ForEachLoop, Completed, "Completed");
IMPLEMENT_PIN(UK2Node_ForEachLoop, ResultArray, "Result Array");
IMPLEMENT_PIN(UK2Node_ForEachLoop, ResultItem, "Result Item");
IMPLEMENT_PIN(UK2Node_ForEachLoop, FoundFlag, "Found?");


UK2Node_ForEachLoop::UK2Node_ForEachLoop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ArrayType(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard, EPinContainerType::Array))
	, OutputElementType(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard))
{
}

FText UK2Node_ForEachLoop::GetTooltipText() const
{
	return LOCTEXT("K2Node_ForEachLoop_Tooltip", "Iterate over array of objects.");
}

FText UK2Node_ForEachLoop::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	switch (LoopFunction)
	{
	case ELoopFunction::Loop:
		return LoopDirection == ELoopDirection::Forward
			? LOCTEXT("K2Node_ForEachLoop_Title", "For Each Loop")
			: LOCTEXT("K2Node_ForEachLoop_TitleReverse", "Reverse For Each Loop");
	case ELoopFunction::Transfrom:
		return LoopDirection == ELoopDirection::Forward
			? LOCTEXT("K2Node_ForEachTransform_Title", "Transform Loop")
			: LOCTEXT("K2Node_ForEachTransform_TitleReverse", "Reverse Transform Loop");
	case ELoopFunction::Filter:
		return LoopDirection == ELoopDirection::Forward
			? LOCTEXT("K2Node_ForEachFilter_Title", "Filter Loop")
			: LOCTEXT("K2Node_ForEachFilter_TitleReverse", "Reverse Filter Loop");
	case ELoopFunction::Accumulate:
		return LoopDirection == ELoopDirection::Forward
			? LOCTEXT("K2Node_ForEachAccumulate_Title", "Accumulate Loop")
			: LOCTEXT("K2Node_ForEachAccumulate_TitleReverse", "Reverse Accumulate Loop");
	case ELoopFunction::Find:
		return LoopDirection == ELoopDirection::Forward
			? LOCTEXT("K2Node_ForEachFind_Title", "Find Loop")
			: LOCTEXT("K2Node_ForEachFind_TitleReverse", "Reverse Find Loop");
	}

	return LOCTEXT("K2Node_ForEachLoop_Title", "For Each Loop");
}

FSlateIcon UK2Node_ForEachLoop::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Macro.ForEach_16x");
	return Icon;
}

FText UK2Node_ForEachLoop::GetMenuCategory() const
{
	static FNodeTextCache CachedCategory;
	if (CachedCategory.IsOutOfDate(this))
	{
		// FText::Format() is slow, so we cache this to save on performance
		CachedCategory.SetCachedText(FEditorCategoryUtils::BuildCategoryString(FCommonEditorCategory::Utilities, FText()), this);
	}
	return CachedCategory;
}

void UK2Node_ForEachLoop::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

void UK2Node_ForEachLoop::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, ArrayType, PN_Array);

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PN_LoopBody);
	CreatePin(EGPD_Output, ArrayElementType, PN_ArrayElement);
	if (bExposeArrayIndex)
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, PN_ArrayIndex);
	switch (LoopFunction)
	{
	case ELoopFunction::Transfrom:
		CreatePin(EGPD_Output, OutputElementType, PN_OutputElement);
		break;
	case ELoopFunction::Filter:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, PN_FilterFlag);
		break;
	case ELoopFunction::Accumulate:
		CreatePin(EGPD_Output, OutputElementType, PN_OutputElement);
		break;
	case ELoopFunction::Find:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, PN_FilterFlag);
		break;
	}

	if (bHaveBreak)
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, PN_BreakFlag);
	if (bExposeCompleted)
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PN_Completed);

	switch (LoopFunction)
	{
	case ELoopFunction::Transfrom:
	case ELoopFunction::Filter:
		CreatePin(EGPD_Output, ResultArrayType, PN_ResultArray);
		break;
	case ELoopFunction::Accumulate:
		CreatePin(EGPD_Output, OutputElementType, PN_ResultItem);
		break;
	case ELoopFunction::Find:
		CreatePin(EGPD_Output, ArrayElementType, PN_ResultItem);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, PN_FoundFlag);
		break;
	}

	Super::AllocateDefaultPins();
}

void UK2Node_ForEachLoop::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	TGuardValue<bool> ExpandNode(bIsExpandingNode, true);

	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin(), GetCompletedPin());

	auto ArrayPin = Compiler.SpawnIntermediateNode<UK2Node_Cache>(GetArrayPin())->GetOutputObjectPin();
	auto ArrayLength = Compiler.SpawnIntermediateNode<UK2Node_CallArrayFunction>(
		EXPAND_FUNCTION_NAME(UKismetArrayLibrary, Array_Length)
		, PARAMETERS((TEXT("TargetArray"), ArrayPin)));

	auto Counter = LoopDirection == ELoopDirection::Forward
		? Compiler.CacheInLocalVariable(0)
		: Compiler.CacheInLocalVariable(ArrayLength->GetReturnValuePin());
	auto Length = LoopDirection == ELoopDirection::Forward
		? Compiler.CacheInLocalVariable(ArrayLength->GetReturnValuePin())
		: Compiler.CacheInLocalVariable(0);

	if (LoopDirection == ELoopDirection::Backward)
	{
		auto DecreaseCounter1 = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
			EXPAND_FUNCTION_NAME(UKismetMathLibrary, Subtract_IntInt)
			, PARAMETERS((TEXT("A"), Counter)));
		Compiler.SpawnIntermediateNode<UK2Node_AssignmentStatement>(Counter, DecreaseCounter1->GetReturnValuePin());
	}

	auto BreakFlag = GetBreakFlagPin() ? Compiler.CacheInLocalVariable(false) : nullptr;
	auto FilterFlag = GetFilterFlagPin() ? Compiler.CacheInLocalVariable(false) : nullptr;
	auto OutputElement = GetOutputElementPin() ? Compiler.SpawnInternalVariable(OutputElementType)->GetVariablePin() : nullptr;
	if (OutputElement) Compiler.ConnectPins(OutputElement, GetOutputElementPin());
	auto OutputArray = GetResultArrayPin() ? Compiler.SpawnInternalVariable(ResultArrayType)->GetVariablePin() : nullptr;
	if (OutputArray) Compiler.ConnectPins(OutputArray, GetResultArrayPin());
	auto ResultItem = GetResultItemPin() ? (LoopFunction == ELoopFunction::Accumulate) ? Compiler.SpawnInternalVariable(ResultItemType)->GetVariablePin() : nullptr : nullptr;
	if (ResultItem) Compiler.ConnectPins(ResultItem, GetResultItemPin());

	if (auto ArrayIndexPin = GetArrayIndexPin()) Compiler.ConnectPins(Counter, ArrayIndexPin);
	if (auto BreakFlagPin = GetBreakFlagPin()) Compiler.ConnectPins(BreakFlag, BreakFlagPin);
	if (auto FilterFlagPin = GetFilterFlagPin()) Compiler.ConnectPins(FilterFlag, FilterFlagPin);

	auto LoopCondition = LoopDirection == ELoopDirection::Forward
		? Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
			EXPAND_FUNCTION_NAME(UKismetMathLibrary, Less_IntInt)
			, PARAMETERS(
				(TEXT("A"), Counter),
				(TEXT("B"), Length)
			))
		: Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
			EXPAND_FUNCTION_NAME(UKismetMathLibrary, Greater_IntInt)
			, PARAMETERS(
				(TEXT("A"), Counter),
				(TEXT("B"), Length)
			));

	auto LoopCounter = LoopDirection == ELoopDirection::Forward
		? Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
			EXPAND_FUNCTION_NAME(UKismetMathLibrary, Add_IntInt), PARAMETERS((TEXT("A"), Counter)))
		: Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
			EXPAND_FUNCTION_NAME(UKismetMathLibrary, Subtract_IntInt), PARAMETERS((TEXT("A"), Counter)));

	auto LoopIfThenElse = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(LoopCondition->GetReturnValuePin());
	{
		if (BreakFlag) Compiler.SpawnIntermediateNode<UK2Node_AssignmentStatement>(BreakFlag, false);
		if (FilterFlag) Compiler.SpawnIntermediateNode<UK2Node_AssignmentStatement>(FilterFlag, false);
		if (OutputElement) Compiler.SpawnIntermediateNode<UK2Node_AssignmentStatement>(OutputElement);

		auto ArrayGet = Compiler.SpawnIntermediateNode<UK2Node_CallArrayFunction>(
			EXPAND_FUNCTION_NAME(UKismetArrayLibrary, Array_Get)
			, PARAMETERS(
				(TEXT("TargetArray"), ArrayPin),
				(TEXT("Index"), Counter)
			));
		auto ArrayElement = Compiler.SpawnIntermediateNode<UK2Node_Cache>(ArrayGet->FindPin(TEXT("Item")))->GetOutputObjectPin();

		Compiler.ConnectPins(ArrayElement, GetArrayElementPin());

		auto Sequence = Compiler.SpawnIntermediateNode<UK2Node_ExecutionSequence>();
		Compiler.ConnectPins(Sequence->GetThenPinGivenIndex(0), GetLoopBodyPin());

		Compiler.LastThenPin = Sequence->GetThenPinGivenIndex(1);
		switch (LoopFunction)
		{
		case ELoopFunction::Transfrom: {
				Compiler.SpawnIntermediateNode<UK2Node_CallArrayFunction>(
					EXPAND_FUNCTION_NAME(UKismetArrayLibrary, Array_Add)
					, PARAMETERS((TEXT("TargetArray"), OutputArray), (TEXT("NewItem"), OutputElement)));
			}
			Sequence->AddInputPin();
			Compiler.LastThenPin = Sequence->GetThenPinGivenIndex(2);
			break;
		case ELoopFunction::Filter: {
				auto FilterFlagCondition = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
					EXPAND_FUNCTION_NAME(UKismetMathLibrary, EqualEqual_BoolBool)
					, PARAMETERS(
						(TEXT("A"), FilterFlag),
						(TEXT("B"), true)
					));
				auto FilterIfThenElse = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(FilterFlagCondition->GetReturnValuePin());
				{
					Compiler.SpawnIntermediateNode<UK2Node_CallArrayFunction>(
						EXPAND_FUNCTION_NAME(UKismetArrayLibrary, Array_Add)
						, PARAMETERS(
							(TEXT("TargetArray"), OutputArray),
							(TEXT("NewItem"), ArrayElement)
						));
				}
			}
			Sequence->AddInputPin();
			Compiler.LastThenPin = Sequence->GetThenPinGivenIndex(2);
			break;
		case ELoopFunction::Accumulate:
			Sequence->AddInputPin();
			Compiler.LastThenPin = Sequence->GetThenPinGivenIndex(2);
			break;
		case ELoopFunction::Find: {
				auto FilterFlagCondition = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
					EXPAND_FUNCTION_NAME(UKismetMathLibrary, EqualEqual_BoolBool)
					, PARAMETERS(
						(TEXT("A"), FilterFlag),
						(TEXT("B"), true)
					));
				Compiler.ConnectPins(FilterFlag, GetFoundFlagPin());
				auto FilterIfThenElse = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(FilterFlagCondition->GetReturnValuePin());
				Compiler.ConnectPins(FilterIfThenElse->GetThenPin(), GetCompletedPin());
				Compiler.LastThenPin = FilterIfThenElse->GetElsePin();
			}
			break;
		}

		if (BreakFlag)
		{
			auto BreakFlagCondifiton = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(
				EXPAND_FUNCTION_NAME(UKismetMathLibrary, EqualEqual_BoolBool)
				, PARAMETERS(
					(TEXT("A"), BreakFlag),
					(TEXT("B"), true)
				));
			auto BreakFlagIfThenElse = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(BreakFlagCondifiton->GetReturnValuePin());
			Compiler.LastThenPin = BreakFlagIfThenElse->GetElsePin();
		}
		Compiler.SpawnIntermediateNode<UK2Node_AssignmentStatement>(Counter, LoopCounter->GetReturnValuePin());

		Compiler.ConnectPins(Compiler.LastThenPin, LoopIfThenElse->GetExecPin());
	}

	Compiler.LastThenPin = LoopIfThenElse->GetElsePin();
}

void UK2Node_ForEachLoop::UpdateArrayTypes()
{
	if (bIsExpandingNode)
		return;

	ArrayType = FK2NodeHelpers::GetWildcardPinType(GetArrayPin(), EPinContainerType::Array);
	ArrayElementType = ArrayType;
	ArrayElementType.ContainerType = EPinContainerType::None;
}

void UK2Node_ForEachLoop::UpdateOutputTypes()
{
	if (bIsExpandingNode)
		return;

	OutputElementType = FK2NodeHelpers::GetWildcardPinType(GetOutputElementPin());
	OutputElementType.bIsReference = false;
	switch (LoopFunction)
	{
	case ELoopFunction::Transfrom:
		ResultArrayType = OutputElementType;
		ResultArrayType.bIsReference = false;
		ResultArrayType.ContainerType = EPinContainerType::Array;
		break;
	case ELoopFunction::Filter:
		ResultArrayType = ArrayType;
		break;
	}
}

void UK2Node_ForEachLoop::ReconstructNode()
{
	UpdateArrayTypes();
	UpdateOutputTypes();

	Super::ReconstructNode();
}

void UK2Node_ForEachLoop::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetArrayPin())
	{
		UpdateArrayTypes();

		if_Valid(GetArrayPin())->PinType = ArrayType;
		if_Valid(GetArrayElementPin())->PinType = ArrayElementType;
	}
	else if (Pin == GetOutputElementPin())
	{
		UpdateOutputTypes();

		if_Valid(GetOutputElementPin())->PinType = OutputElementType;
		if_Valid(GetResultArrayPin())->PinType = ResultArrayType;
	}
	else if (Pin == GetResultItemPin())
	{
		UpdateOutputTypes();

		if (LoopFunction == ELoopFunction::Accumulate)
		{
			if_Valid(GetResultItemPin())->PinType = OutputElementType;
		}
	}
}

#if WITH_NOTIFY_GRAPH_PATCH
void UK2Node_ForEachLoop::NotifyGraphChanged(const struct FEdGraphEditAction& InAction)
{
	Super::NotifyGraphChanged(InAction);

	{
		UpdateArrayTypes();

		if_Valid(GetArrayPin())->PinType = ArrayType;
		if_Valid(GetArrayElementPin())->PinType = ArrayElementType;
	}

	{
		UpdateOutputTypes();

		if_Valid(GetOutputElementPin())->PinType = OutputElementType;
		if_Valid(GetResultArrayPin())->PinType = ResultArrayType;

		if (LoopFunction == ELoopFunction::Accumulate)
		{
			if_Valid(GetResultItemPin())->PinType = OutputElementType;
		}
	}
}
#endif

#if WITH_EDITOR
void UK2Node_ForEachLoop::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ReconstructNode();
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
