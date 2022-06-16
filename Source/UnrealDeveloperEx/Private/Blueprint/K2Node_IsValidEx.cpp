#include "Blueprint/K2Node_IsValidEx.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "ScopedTransaction.h"
#include "ToolMenu.h"

#include "ValidEx.h"


#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


IMPLEMENT_PIN(UK2Node_IsValidEx, InputObject, "Input Object");
IMPLEMENT_PIN(UK2Node_IsValidEx, ValidObject, "Valid Object");
IMPLEMENT_PIN(UK2Node_IsValidEx, IsValid, "Is Valid");
IMPLEMENT_PIN(UK2Node_IsValidEx, IsNotValid, "Is Not Valid");

UK2Node_IsValidEx::UK2Node_IsValidEx(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UpcastType.Add(nullptr);
	InputObjectType.Add(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass()));
}

FText UK2Node_IsValidEx::GetTooltipText() const
{
	return LOCTEXT("K2Node_IsValidEx_Tooltip", "Check validity of an object.");
}

FText UK2Node_IsValidEx::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return UpcastType.Num() == 1 && IsValid(UpcastType[0])
		? FText::Format(LOCTEXT("K2Node_IsValidEx_TitleFmt", "IsValid {0}"), FText::FromString(UpcastType[0]->GetName()))
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

void UK2Node_IsValidEx::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("K2NodeIsValid", LOCTEXT("K2Node_IsValidEx_ContextMenu", "IsValid"));

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
						FExecuteAction::CreateUObject(const_cast<UK2Node_IsValidEx*>(this), &UK2Node_IsValidEx::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
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
					FExecuteAction::CreateUObject(const_cast<UK2Node_IsValidEx*>(this), &UK2Node_IsValidEx::InteractiveAddInputPin)
				)
			);
		}

		Section.AddMenuEntry(
			"ResetToWildcard",
			LOCTEXT("ResetToWildcard", "Reset to wildcard"),
			LOCTEXT("ResetToWildcardTooltip", "Reset the node to have wildcard input/outputs. Requires no pins are connected."),
			FSlateIcon(),
			FUIAction(
				//FExecuteAction::CreateUObject(const_cast<UK2Node_MakeArray*>(this), &UK2Node_MakeArray::ClearPinTypeToWildcard),
				//FCanExecuteAction::CreateUObject(this, &UK2Node_MakeArray::CanResetToWildcard)
			)
		);
	}
}

UEdGraphPin* UK2Node_IsValidEx::GetInputObjectPin(int Index) const
{
	if (Index == 0)
		return GetInputObjectPin();

	int InputObjectPinIndex = Pins.IndexOfByKey(GetInputObjectPin()) + Index;
	return Pins[InputObjectPinIndex];
}

UEdGraphPin* UK2Node_IsValidEx::GetValidObjectPin(int Index) const
{
	if (Index == 0)
		return GetValidObjectPin();

	int ValidObjectPinIndex = Pins.IndexOfByKey(GetValidObjectPin()) + Index;
	return Pins[ValidObjectPinIndex];
}

void UK2Node_IsValidEx::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	AllocateInputObjectPins();

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PN_IsValid);
	AllocateValidObjectPins();
	if (!bHideNotValidPin)
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PN_IsNotValid);

	Super::AllocateDefaultPins();
}

void UK2Node_IsValidEx::AllocateInputObjectPins()
{
	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		auto PinName = i == 0 ? PN_InputObject : FName(FString::Printf(TEXT("%s %d"), *PN_InputObject.ToString(), i));
		CreatePin(EGPD_Input, InputObjectType[i], PinName);
	}
}

void UK2Node_IsValidEx::AllocateValidObjectPins()
{
	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		auto PinName = i == 0 ? PN_ValidObject : FName(FString::Printf(TEXT("%s %d"), *PN_ValidObject.ToString(), i));
		auto PinType = InputObjectType[i];
		if (IsValid(UpcastType[i]))
		{
			PinType.PinSubCategoryObject = UpcastType[i];
			if (UpcastType[i]->IsChildOf(UInterface::StaticClass()))
				PinType.PinCategory = UEdGraphSchema_K2::PC_Interface;
			else
				PinType.PinCategory = UEdGraphSchema_K2::PC_Object;
		}

		CreatePin(EGPD_Output, PinType, PinName);
	}
}

void UK2Node_IsValidEx::UpdateWildcardPins()
{
	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		InputObjectType[i] = FK2NodeHelpers::GetWildcardPinObjectType(GetInputObjectPin(i), UObject::StaticClass());
		auto ValidPinType = InputObjectType[i];
		if (IsValid(UpcastType[i]))
			ValidPinType.PinSubCategoryObject = UpcastType[i];

		if_Valid(GetInputObjectPin(i))->PinType = InputObjectType[i];
		if_Valid(GetValidObjectPin(i))->PinType = ValidPinType;
	}
}

void UK2Node_IsValidEx::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin(), GetIsValidPin());

	for (int i = 0; i < GetInputObjectNum(); i++)
	{
		auto ObjectPin = Compiler.SpawnIntermediateNode<UK2Node_Cache>(GetInputObjectPin(i))->GetOutputObjectPin();

		auto ValidateObjectIfThenElse = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(
			Compiler.SpawnIsValidNode(ObjectPin)->GetReturnValuePin());
		auto NotValidPin = ValidateObjectIfThenElse->GetElsePin();

		auto UpcastType_ = UpcastType[i];
		if (UpcastType_ && UpcastType_ != InputObjectType[i].PinSubCategoryObject)
		{
			auto CastNode = Compiler.SpawnIntermediateNode<UK2Node_DynamicCast>(UpcastType_, ObjectPin);
			ObjectPin = CastNode->GetCastResultPin();
			NotValidPin = CastNode->GetInvalidCastPin();
		}

		Compiler.ConnectPins(ObjectPin, GetValidObjectPin(i));
		if (auto IsNotValidPin = GetIsNotValidPin()) Compiler.ConnectPins(NotValidPin, IsNotValidPin);
	}
}

void UK2Node_IsValidEx::ReconstructNode()
{
	UpdateWildcardPins();

	Super::ReconstructNode();
}

void UK2Node_IsValidEx::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	UpdateWildcardPins();
}

#if WITH_EDITOR
void UK2Node_IsValidEx::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ReconstructNode();
}
#endif // WITH_EDITOR


void UK2Node_IsValidEx::InteractiveAddInputPin()
{
	FScopedTransaction Transaction(LOCTEXT("AddPinTx", "Add Pin"));
	AddInputPin();
}

void UK2Node_IsValidEx::AddInputPin()
{
	UpcastType.Add(nullptr);
	InputObjectType.Add(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Wildcard, UObject::StaticClass()));

	ReconstructNode();
}

bool UK2Node_IsValidEx::CanRemovePin(const UEdGraphPin* Pin) const
{
	return InputObjectType.Num() > 1;
}

void UK2Node_IsValidEx::RemoveInputPin(UEdGraphPin* Pin)
{
	//UpcastType.Add(nullptr);
	//InputObjectType.Add(nullptr);
}

#undef LOCTEXT_NAMESPACE
