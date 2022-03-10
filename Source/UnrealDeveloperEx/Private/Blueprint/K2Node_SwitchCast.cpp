#include "Blueprint/K2Node_SwitchCast.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"

#include "CoreEx.h"

#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


UK2Node_SwitchCast::UK2Node_SwitchCast(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UK2Node_SwitchCast::GetTooltipText() const
{
	return LOCTEXT("K2Node_SwitchCast_Tooltip", "Try cast to multiple types.");
}

FText UK2Node_SwitchCast::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_SwitchCast_Title", "Switch Cast");
}

FSlateIcon UK2Node_SwitchCast::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Cast_16x");
	return Icon;
}

FText UK2Node_SwitchCast::GetMenuCategory() const
{
	static FNodeTextCache CachedCategory;
	if (CachedCategory.IsOutOfDate(this))
	{
		// FText::Format() is slow, so we cache this to save on performance
		CachedCategory.SetCachedText(FEditorCategoryUtils::BuildCategoryString(FCommonEditorCategory::Utilities, LOCTEXT("ActionMenuCategory", "Casting")), this);
	}
	return CachedCategory;
}

void UK2Node_SwitchCast::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

UEdGraphPin* UK2Node_SwitchCast::GetObjectPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_ObjectToCast);
}

UEdGraphPin* UK2Node_SwitchCast::GetInvalidCastPin() const
{
	return FindPin(UEdGraphSchema_K2::PN_CastFailed);
}

void UK2Node_SwitchCast::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass(), UEdGraphSchema_K2::PN_ObjectToCast);

	for (auto TargetType : TargetTypes)
	{
		if (!IsValid(TargetType))
			continue;

		const FString CastExecutePinName = FString(TEXT("Is")) + TargetType->GetDisplayNameText().ToString();
		const FString CastResultPinName = UEdGraphSchema_K2::PN_CastedValuePrefix + TargetType->GetDisplayNameText().ToString();

		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, *CastExecutePinName);
		if (TargetType->IsChildOf(UInterface::StaticClass()))
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Interface, *TargetType, *CastResultPinName);
		}
		else
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, *TargetType, *CastResultPinName);
		}
	}

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_CastFailed);

	Super::AllocateDefaultPins();
}

void UK2Node_SwitchCast::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin());

	auto ObjectPinType = FK2NodeHelpers::GetWildcardPinObjectType(GetObjectPin(), UObject::StaticClass());
	auto ObjectPin = Compiler.SpawnIntermediateNode<UK2Node_Cache>(GetObjectPin())->GetOutputObjectPin();

	auto IsValidBranchNode = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(
		Compiler.SpawnIsValidNode(ObjectPin)->GetReturnValuePin());

	for (auto TargetType : TargetTypes)
	{
		if (!IsValid(TargetType))
			continue;

		const FString CastExecutePinName = FString(TEXT("Is")) + TargetType->GetDisplayNameText().ToString();
		const FString CastResultPinName = UEdGraphSchema_K2::PN_CastedValuePrefix + TargetType->GetDisplayNameText().ToString();

		auto ExecuteTargetTypePin = FindPin(*CastExecutePinName);
		auto ResultTargetTypePin = FindPin(*CastResultPinName);

		if (!IsValid(ObjectPinType) || !Valid<UClass>(ObjectPinType.PinSubCategoryObject)->IsChildOf(TargetType))
		{
			auto UpCastNode = Compiler.SpawnIntermediateNode<UK2Node_DynamicCast>(TargetType, ObjectPin);
			Compiler.LastThenPin = UpCastNode->GetInvalidCastPin();

			Compiler.ConnectPins(UpCastNode->GetValidCastPin(), ExecuteTargetTypePin);
			Compiler.ConnectPins(UpCastNode->GetCastResultPin(), ResultTargetTypePin);
		}
		else
		{
			Compiler.ConnectPins(Compiler.LastThenPin, ExecuteTargetTypePin);
			Compiler.ConnectPins(ObjectPin, ResultTargetTypePin);

			break;
		}
	}

	Compiler.ConnectPins(IsValidBranchNode->GetElsePin(), GetInvalidCastPin());
}

void UK2Node_SwitchCast::PostReconstructNode()
{
	Super::PostReconstructNode();

	if (auto ObjectPin = GetObjectPin())
	{
		if (ObjectPin->LinkedTo.Num() > 0)
		{
			ObjectPin->PinType = ObjectPin->LinkedTo[0]->PinType;
			ObjectPin->PinType.ContainerType = EPinContainerType::None;
			ObjectPin->PinType.bIsReference = false;
		}
	}
}

void UK2Node_SwitchCast::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin->ParentPin == nullptr)
	{
		if (auto ObjectPin = GetObjectPin())
		{
			// If a pin connection is made, then we need to propagate the change to the other pins and validate all connections, otherwise reset them to Wildcard pins
			if (ObjectPin->LinkedTo.Num() > 0)
			{
				ObjectPin->PinType = ObjectPin->LinkedTo[0]->PinType;
				ObjectPin->PinType.ContainerType = EPinContainerType::None;
				ObjectPin->PinType.bIsReference = false;
			}
			else if (ObjectPin->LinkedTo.Num() == 0)
			{
				ObjectPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
				ObjectPin->PinType.PinSubCategory = TEXT("");
				ObjectPin->PinType.PinSubCategoryObject = NULL;

				ObjectPin->BreakAllPinLinks();
			}
		}
	}
}

#if WITH_EDITOR
void UK2Node_SwitchCast::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if_PropertyChanged(TargetTypes)
	{
		TargetTypes.StableSort([](auto A, auto B) { return A && B && A->IsChildOf(B); });
		ReconstructNode();
	}
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
