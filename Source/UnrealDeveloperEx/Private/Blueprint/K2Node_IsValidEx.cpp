#include "Blueprint/K2Node_IsValidEx.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"

#include "ValidEx.h"


#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


IMPLEMENT_PIN(UK2Node_IsValidEx, InputObject, "Input Object");
IMPLEMENT_PIN(UK2Node_IsValidEx, ValidObject, "Valid Object");
IMPLEMENT_PIN(UK2Node_IsValidEx, IsValid, "Is Valid");
IMPLEMENT_PIN(UK2Node_IsValidEx, IsNotValid, "Is Not Valid");

UK2Node_IsValidEx::UK2Node_IsValidEx(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UK2Node_IsValidEx::GetTooltipText() const
{
	return LOCTEXT("K2Node_IsValidEx_Tooltip", "Check validity of an object.");
}

FText UK2Node_IsValidEx::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return IsValid(UpcastType)
		? FText::Format(LOCTEXT("K2Node_IsValidEx_TitleFmt", "IsValid {0}"), FText::FromString(UpcastType->GetName()))
		: LOCTEXT("K2Node_IsValidEx_Title", "IsValidEx");
}

FSlateIcon UK2Node_IsValidEx::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Macro.IsValid_16x");
	return Icon;
}

FText UK2Node_IsValidEx::GetMenuCategory() const
{
	static FNodeTextCache CachedCategory;
	if (CachedCategory.IsOutOfDate(this))
	{
		// FText::Format() is slow, so we cache this to save on performance
		CachedCategory.SetCachedText(FEditorCategoryUtils::BuildCategoryString(FCommonEditorCategory::Utilities, FText()), this);
	}
	return CachedCategory;
}

void UK2Node_IsValidEx::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

void UK2Node_IsValidEx::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	if (IsValid(InputObjectType))
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, InputObjectType, PN_InputObject);
	}
	else
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass(), PN_InputObject);
	}

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PN_IsValid);
	if (IsValid(InputObjectType))
	{
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, IsValid(UpcastType) ? *UpcastType : InputObjectType, PN_ValidObject);
	}
	else
	{
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass(), PN_ValidObject);
	}
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PN_IsNotValid);

	Super::AllocateDefaultPins();
}

void UK2Node_IsValidEx::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin(), GetIsValidPin());

	auto ObjectPin = Compiler.SpawnIntermediateNode<UK2Node_Cache>(GetInputObjectPin())->GetOutputObjectPin();

	auto ValidateObjectIfThenElse = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(
		Compiler.SpawnIsValidNode(ObjectPin)->GetReturnValuePin());

	if (UpcastType && UpcastType != InputObjectType)
	{
		auto UpCastNode = Compiler.SpawnIntermediateNode<UK2Node_DynamicCast>(UpcastType, ObjectPin);
		ObjectPin = UpCastNode->GetCastResultPin();
	}

	Compiler.ConnectPins(ObjectPin, GetValidObjectPin());
	Compiler.ConnectPins(ValidateObjectIfThenElse->GetElsePin(), GetIsNotValidPin());
}

void UK2Node_IsValidEx::ReconstructNode()
{
	InputObjectType = FK2NodeHelpers::GetWildcardPinClassType(GetInputObjectPin());

	Super::ReconstructNode();
}

void UK2Node_IsValidEx::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetInputObjectPin())
	{
		// TODO: Fix this. Node reconstruct here can cause crash.

		ReconstructNode();
	}
}

#if WITH_EDITOR
void UK2Node_IsValidEx::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ReconstructNode();
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
