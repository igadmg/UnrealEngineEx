#include "Blueprint/K2NodeCompilerHelper.h"

#include "EdGraph/EdGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/EngineVersionComparison.h"
#include "Engine/Engine.h"
#include "EdGraphSchema_K2.h"

#include "CoreEx.h"


#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"

FK2NodeCompilerHelper::~FK2NodeCompilerHelper()
{
	if (bIsErrorFree)
	{
		if (LastThenPin && FinishPin)
		{
			ConnectPins(LastThenPin, FinishPin);
		}
	}

	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "@@: Internal connection error. @@").ToString(), *Node->GetFullName(), Node);
		//CompilerContext.MessageLog.Error(*FText::Format(LOCTEXT("InternalConnectionError", "{0}: Internal connection error. @@"), FText::FromString(Node->GetFullName())).ToString(), Node);
	}

	if (bFinalize)
	{
		Node->BreakAllNodeLinks();
	}
}

UK2Node_CallFunction* FK2NodeCompilerHelper::SpawnSetVariableNode(const FEdGraphPinType& PinType)
{
	UFunction* SetByNameFunction = UEdGraphSchema_K2::FindSetVariableByNameFunction(PinType);
	if (SetByNameFunction)
	{
		if (PinType.IsArray())
		{
			return SpawnIntermediateNode<UK2Node_CallArrayFunction>(SetByNameFunction);
		}

		return SpawnIntermediateNode<UK2Node_CallFunction>(SetByNameFunction);
	}

	return nullptr;
}

UK2Node_CallFunction* FK2NodeCompilerHelper::SpawnSetVariableNode(UEdGraphPin* SourcePin)
{
	auto SetVariableNode = SpawnSetVariableNode(SourcePin->PinType);
	if (SetVariableNode)
	{
		UEdGraphPin* PropertyNamePin = SetVariableNode->FindPinChecked(TEXT("PropertyName"));
#if !UE_VERSION_OLDER_THAN(4, 19, 0)
		PropertyNamePin->DefaultValue = SourcePin->PinName.ToString();
#else
		PropertyNamePin->DefaultValue = SourcePin->PinName;
#endif

		// Move connection from the variable pin on the spawn node to the 'value' pin
		UEdGraphPin* ValuePin = SetVariableNode->FindPinChecked(TEXT("Value"));
		if (SourcePin->LinkedTo.Num() == 0) // TODO: sus
		{
			if (SourcePin->DefaultValue != FString()
				&& SourcePin->PinType.PinCategory == UEdGraphSchema_K2::PC_Byte
				&& SourcePin->PinType.PinSubCategoryObject.IsValid()
				&& SourcePin->PinType.PinSubCategoryObject->IsA<UEnum>())
			{
				// Pin is an enum, we need to alias the enum value to an int:
				SourcePin = SpawnIntermediateNode<UK2Node_EnumLiteral>(SourcePin)
					->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
			}
			else if (!SourcePin->PinType.IsArray()
				&& SourcePin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
			{
				// For non-array struct pins that are not linked, transfer the pin type so that the node will expand an auto-ref that will assign the value by-ref.
				ValuePin->PinType.PinCategory = SourcePin->PinType.PinCategory;
				ValuePin->PinType.PinSubCategory = SourcePin->PinType.PinSubCategory;
				ValuePin->PinType.PinSubCategoryObject = SourcePin->PinType.PinSubCategoryObject;
			}
		}

		ConnectPins(SourcePin, ValuePin);
		SetVariableNode->PinConnectionListChanged(ValuePin);
	}

	return SetVariableNode;
}

UK2Node_TemporaryVariable* FK2NodeCompilerHelper::SpawnInternalVariable(const FEdGraphPinType& PinType)
{
	return CompilerContext.SpawnInternalVariable(Node
		, PinType.PinCategory, PinType.PinSubCategory, PinType.PinSubCategoryObject.Get()
		, PinType.ContainerType, PinType.PinValueType);
}

void FK2NodeCompilerHelper::LogString(const FString& InString, bool bPrintToLog)
{
	auto nolog = TGuardValue<bool>(bLog, false);
	auto PrintString = SpawnIntermediateNode<UK2Node_CallFunction>(EXPAND_FUNCTION_NAME(UKismetSystemLibrary, LogString));
	PrintString->FindPin(TEXT("InString"))->DefaultValue = InString;
	PrintString->FindPin(TEXT("bPrintToLog"))->DefaultValue = UKismetStringLibrary::Conv_BoolToString(bPrintToLog);
}

UEdGraphPin* FK2NodeCompilerHelper::CacheInLocalVariable(bool Value)
{
	auto LocalObjectVar = SpawnInternalVariable(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Boolean));
	auto LocalObjectVarAssignement = SpawnIntermediateNode<UK2Node_AssignmentStatement>(LocalObjectVar->GetVariablePin(), Value);

	return LocalObjectVar->GetVariablePin();
}

UEdGraphPin* FK2NodeCompilerHelper::CacheInLocalVariable(int32 Value)
{
	auto LocalObjectVar = SpawnInternalVariable(FK2NodeHelpers::MakePinType(UEdGraphSchema_K2::PC_Int));
	auto LocalObjectVarAssignement = SpawnIntermediateNode<UK2Node_AssignmentStatement>(LocalObjectVar->GetVariablePin(), Value);

	return LocalObjectVar->GetVariablePin();
}

UEdGraphPin* FK2NodeCompilerHelper::CacheInLocalVariable(UEdGraphPin* ObjectPin)
{
	auto LocalObjectVar = SpawnInternalVariable(ObjectPin->PinType);
	auto LocalObjectVarAssignement = SpawnIntermediateNode<UK2Node_AssignmentStatement>(LocalObjectVar->GetVariablePin(), ObjectPin);

	return LocalObjectVar->GetVariablePin();
}

UK2Node_CallFunction* FK2NodeCompilerHelper::SpawnIsValidNode(UEdGraphPin* ObjectPin)
{
	auto Result = SpawnIntermediateNode<UK2Node_CallFunction>(EXPAND_FUNCTION_NAME(UKismetSystemLibrary, IsValid));

	UEdGraphPin* IsValidInputPin = Result->FindPin(UEdGraphSchema_K2::PN_ObjectToCast);
	CompilerContext.GetSchema()->TryCreateConnection(ObjectPin, IsValidInputPin);

	return Result;
}

bool FK2NodeCompilerHelper::ConnectPins(class UEdGraphPin* SourcePin, class UEdGraphPin* TargetPin, bool bOptional)
{
	if (SourcePin == nullptr)
		return bIsErrorFree && bOptional;

	if (TargetPin == nullptr)
		return bIsErrorFree && bOptional;

	if (SourcePin->GetOwningNode() == Node && TargetPin->GetOwningNode() == Node)
	{
		bool bIsExec = SourcePin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec;
		for (auto TargetLink : TargetPin->LinkedTo)
		{
			if (!IsValid(TargetLink))
				continue;

			if (!bIsExec)
				bIsErrorFree &= CompilerContext.CopyPinLinksToIntermediate(*SourcePin, *TargetLink).CanSafeConnect();
			else
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*SourcePin, *TargetLink).CanSafeConnect();
		}
	}
	else if (SourcePin->GetOwningNode() == Node)
	{
		if (SourcePin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec)
			bIsErrorFree &= CompilerContext.CopyPinLinksToIntermediate(*SourcePin, *TargetPin).CanSafeConnect();
		else
			bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*SourcePin, *TargetPin).CanSafeConnect();
	}
	else if (TargetPin->GetOwningNode() == Node)
	{
		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*TargetPin, *SourcePin).CanSafeConnect();
	}
	else
	{
		bIsErrorFree &= CompilerContext.GetSchema()->TryCreateConnection(SourcePin, TargetPin);
	}

	if (auto SourceNode = Valid<UK2Node>(SourcePin->GetOwningNode()))
		if (SourceNode != Node)
			SourceNode->NotifyPinConnectionListChanged(SourcePin);

	if (auto TargetNode = Valid<UK2Node>(TargetPin->GetOwningNode()))
		if (TargetNode != Node)
			TargetNode->NotifyPinConnectionListChanged(TargetPin);

	return bIsErrorFree;
}

bool FK2NodeCompilerHelper::ConnectSetVariable(UEdGraphPin* SourcePin, UEdGraphPin* ObjectPin)
{
	if (auto SetVarNode = SpawnSetVariableNode(SourcePin))
	{
		ConnectPins(ObjectPin, SetVarNode->FindPinChecked(TEXT("Object")));

		return bIsErrorFree;
	}

	return bIsErrorFree;
}


void FK2NodeCompilerHelper::SetupNode(UK2Node_Event* Event, FName CustomFunctionName)
{
	Event->CustomFunctionName = CustomFunctionName;
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_Event* Event, FName CustomFunctionName, class UFunction* Function)
{
	SetupNode(Event, CustomFunctionName);

	auto Schema = CompilerContext.GetSchema();

	for (TFieldIterator<FProperty> PropIt(Function); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
	{
		const FProperty* Param = *PropIt;
		if (!Param->HasAnyPropertyFlags(CPF_OutParm) || Param->HasAnyPropertyFlags(CPF_ReferenceParm))
		{
			FEdGraphPinType PinType;
			bIsErrorFree &= Schema->ConvertPropertyToPinType(Param, /*out*/ PinType);
			bIsErrorFree &= (nullptr != Event->CreateUserDefinedPin(Param->GetFName(), PinType, EGPD_Output));
		}
	}
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin)
{
	AddDelegate->SetFromProperty(Property, false, Property->GetOwnerClass());
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin)
{
	ConnectPins(AddDelegate->FindPin(UEdGraphSchema_K2::PN_Self), SelfPin);
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin)
{
	SetupNode(AddDelegate, Property, SelfPin);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin)
{
	ConnectNode(AddDelegate, Property, SelfPin);

	ConnectPins(AddDelegate->FindPin(TEXT("Delegate")), DelegatePin);
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_RemoveDelegate* RemoveDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin)
{
	RemoveDelegate->SetFromProperty(Property, false, Property->GetOwnerClass());
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_RemoveDelegate* RemoveDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin)
{
	ConnectPins(RemoveDelegate->FindPin(UEdGraphSchema_K2::PN_Self), SelfPin);
	ConnectPins(RemoveDelegate->FindPin(TEXT("Delegate")), DelegatePin);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, bool Value)
{
	ConnectPins(VariablePin, AssignmentStatement->GetVariablePin());
	AssignmentStatement->GetValuePin()->DefaultValue = UKismetStringLibrary::Conv_BoolToString(Value);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, int32 Value)
{
	ConnectPins(VariablePin, AssignmentStatement->GetVariablePin());
	AssignmentStatement->GetValuePin()->DefaultValue = UKismetStringLibrary::Conv_IntToString(Value);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, UEdGraphPin* ValuePin)
{
	ConnectPins(VariablePin, AssignmentStatement->GetVariablePin());
	ConnectPins(AssignmentStatement->GetValuePin(), ValuePin);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_Cache* Cahce, UEdGraphPin* ObjectPin)
{
	ConnectPins(ObjectPin, Cahce->GetInputObjectPin());
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_CallFunction* CallFunction, UFunction* Function)
{
	CallFunction->SetFromFunction(Function);
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_CallFunction* CallFunction, UClass* FunctionClass, FName FunctionName)
{
	CallFunction->FunctionReference.SetExternalMember(FunctionName, FunctionClass);
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_CallFunction* CallFunction, UClass* FunctionClass, FName FunctionName, TMap<FName, UEdGraphPin*> Params)
{
	SetupNode(CallFunction, FunctionClass, FunctionName);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_CallFunction* CallFunction, UClass* FunctionClass, FName FunctionName, TMap<FName, UEdGraphPin*> Params)
{
	for (auto Pair : Params)
	{
		ConnectPins(Pair.Value, CallFunction->FindPin(Pair.Key));
	}
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, UFunction* Function)
{
	SetupNode(CallFunction, Function);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, UFunction* Function)
{
	ConnectPins(SelfPin, CallFunction->FindPin(UEdGraphSchema_K2::PN_Self));
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, UClass* FunctionClass, FName FunctionName)
{
	SetupNode(CallFunction, FunctionClass, FunctionName);
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, UClass* FunctionClass, FName FunctionName)
{
	ConnectPins(SelfPin, CallFunction->FindPin(UEdGraphSchema_K2::PN_Self));
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_DynamicCast* DynamicCast, UClass* CastClass, UEdGraphPin* ObjectPin)
{
	DynamicCast->TargetType = CastClass;
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_DynamicCast* DynamicCast, UClass* CastClass, UEdGraphPin* ObjectPin)
{
	ConnectPins(ObjectPin, DynamicCast->GetCastSourcePin());
}

void FK2NodeCompilerHelper::SetupNode(UK2Node_EnumLiteral* EnumLiteral, UEdGraphPin* ValuePin)
{
	EnumLiteral->Enum = CastChecked<UEnum>(ValuePin->PinType.PinSubCategoryObject.Get());
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_EnumLiteral* EnumLiteral, UEdGraphPin* ValuePin)
{
	if (auto InPin = EnumLiteral->FindPinChecked(UK2Node_EnumLiteral::GetEnumInputPinName()))
	{
		InPin->DefaultValue = ValuePin->DefaultValue;
	}
	else
	{
		bIsErrorFree &= false;
	}
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_IfThenElse* IfThenElse, UEdGraphPin* ConditionPin)
{
	ConnectPins(ConditionPin, IfThenElse->GetConditionPin());
}

void FK2NodeCompilerHelper::ConnectNode(UK2Node_Knot* KnotNode, UEdGraphPin* ObjectPin)
{
	if (ObjectPin->GetOwningNode() == Node)
	{
		if (ObjectPin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*ObjectPin, *KnotNode->GetInputPin()).CanSafeConnect();
			KnotNode->NotifyPinConnectionListChanged(KnotNode->GetInputPin());
		}
		else
		{
			bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*ObjectPin, *KnotNode->GetOutputPin()).CanSafeConnect();
			KnotNode->NotifyPinConnectionListChanged(KnotNode->GetOutputPin());
		}
	}
	else
	{
		if (ObjectPin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			bIsErrorFree &= CompilerContext.GetSchema()->TryCreateConnection(ObjectPin, KnotNode->GetInputPin());
			KnotNode->NotifyPinConnectionListChanged(KnotNode->GetInputPin());
		}
		else
		{
			bIsErrorFree &= CompilerContext.GetSchema()->TryCreateConnection(KnotNode->GetOutputPin(), ObjectPin);
			KnotNode->NotifyPinConnectionListChanged(KnotNode->GetOutputPin());
		}
	}
}
/*
void FK2NodeCompilerHelper::LogNode(UK2Node_AddDelegate* AddDelegate)
{

}

void FK2NodeCompilerHelper::LogNode(UK2Node_RemoveDelegate* RemoveDelegate)
{

}

void FK2NodeCompilerHelper::LogNode(UK2Node_AssignmentStatement* AssignmentStatement)
{

}

void FK2NodeCompilerHelper::LogNode(UK2Node_Cache* Cahce)
{

}

void FK2NodeCompilerHelper::LogNode(UK2Node_CallFunction* CallFunction)
{

}

void FK2NodeCompilerHelper::LogNode(UK2Node_DynamicCast* DynamicCast)
{

}

void FK2NodeCompilerHelper::LogNode(UK2Node_IfThenElse* IfThenElse)
{

}
*/
void FK2NodeCompilerHelper::LogNode(UK2Node* GenericNode)
{
	if (GenericNode->IsNodePure())
		return;

	LogString(GenericNode->GetName());
}

#undef LOCTEXT_NAMESPACE
