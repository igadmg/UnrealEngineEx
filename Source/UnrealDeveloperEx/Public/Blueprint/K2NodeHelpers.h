#pragma once

#include "KismetCompiler.h"
#include "K2Node_AddDelegate.h"
#include "K2Node_CallArrayFunction.h"
#include "K2Node_CallFunction.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_EnumLiteral.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_TemporaryVariable.h"


class UNREALDEVELOPEREX_API FK2NodeHelpers
{
public:
	static bool CreateInputPins(class UEdGraphNode* Node, class UFunction* Function);

	static void CreateOutputPins(class UEdGraphNode* Node, class FMulticastDelegateProperty* Property);
	static void CreateOutputPins(class UEdGraphNode* Node, class UClass* Class);
};

class UNREALDEVELOPEREX_API FK2NodeCompilerHelper
{
public:
	class UEdGraphNode* Node;
	class FKismetCompilerContext& CompilerContext;
	class UEdGraph* SourceGraph;

public:
	FK2NodeCompilerHelper(class UEdGraphNode* Node, class FKismetCompilerContext& CompilerContext, class UEdGraph* SourceGraph)
		: Node(Node)
		, CompilerContext(CompilerContext)
		, SourceGraph(SourceGraph)
	{
	}

	UK2Node_TemporaryVariable* SpawnInternalVariable(const struct FEdGraphPinType& PinType);

	template <typename NodeType, typename... Ts>
	NodeType* SpawnIntermediateEventNode(class UEdGraphPin* SourcePin, Ts... args)
	{
		auto Result = CompilerContext.SpawnIntermediateEventNode<NodeType>(Node, SourcePin, SourceGraph);
		SetupNode(Result, args...);
		Result->AllocateDefaultPins();

		return Result;
	}

	template <typename NodeType>
	NodeType* SpawnIntermediateNode()
	{
		auto Result = CompilerContext.SpawnIntermediateNode<NodeType>(Node, SourceGraph);
		Result->AllocateDefaultPins();

		return Result;
	}

	template <typename NodeType, typename... Ts>
	NodeType* SpawnIntermediateNode(Ts... args)
	{
		auto Result = CompilerContext.SpawnIntermediateNode<NodeType>(Node, SourceGraph);
		SetupNode(Result, args...);
		Result->AllocateDefaultPins();
		ConnectNode(Result, args...);

		return Result;
	}

	UK2Node_CallFunction* SpawnIsValidNode(class UEdGraphPin* ObjectPin);

	bool MovePinLinksToIntermediate(class UEdGraphNode* Source, FName SourcePinName, class UEdGraphNode* Target, FName TargetPinName, bool bOptional = false);
	bool MovePinLinksToIntermediate(class UEdGraphNode* Source, FName SourcePinName, class UEdGraphPin* TargetPin, bool bOptional = false);
	bool MovePinLinksToIntermediate(class UEdGraphPin* SourcePin, class UEdGraphPin* TargetPin, bool bOptional = false);

	bool CreateSetParamByNameNodes(class UEdGraphPin* ObjectPin, class UEdGraphPin* SpawnVarPin, class UEdGraphPin*& LastThenPin);


protected:
	void SetupNode(UK2Node_Event* Event, FName CustomFunctionName);

	void SetupNode(UK2Node_AddDelegate* AddDelegatem, const FProperty* Property, UEdGraphPin* ObjectPin);
	void ConnectNode(UK2Node_AddDelegate* AddDelegatem, const FProperty* Property, UEdGraphPin* ObjectPin);

	void SetupNode(UK2Node_CallFunction* CallFunction, class UFunction* Function);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UFunction* Function) {}
	void SetupNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName) {}

	void SetupNode(UK2Node_CallFunction* CallFunction, class UEdGraphPin* SelfPin, class UFunction* Function);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UEdGraphPin* SelfPin, class UFunction* Function);
	void SetupNode(UK2Node_CallFunction* CallFunction, class UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName);

	void SetupNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, class UEdGraphPin* ObjectPin);
	void ConnectNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, class UEdGraphPin* ObjectPin);

	void SetupNode(UK2Node_EnumLiteral* EnumLiteral, class UEdGraphPin* ValuePin, class UEdGraphPin* ResultPin);
	void ConnectNode(UK2Node_EnumLiteral* EnumLiteral, class UEdGraphPin* ValuePin, class UEdGraphPin* ResultPin);

	void SetupNode(UK2Node_IfThenElse* IfThenElse, class UEdGraphPin* ConditionPin) {}
	void ConnectNode(UK2Node_IfThenElse* IfThenElse, class UEdGraphPin* ConditionPin);
};

