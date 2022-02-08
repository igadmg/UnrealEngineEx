#pragma once

#include "KismetCompiler.h"
#include "K2Node_AddDelegate.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_CallArrayFunction.h"
#include "K2Node_CallFunction.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_EnumLiteral.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_Knot.h"
#include "K2Node_TemporaryVariable.h"

#include "Blueprint/K2Node_Cache.h"


class UEdGraphNode;
class UEdGraphPin;

class UNREALDEVELOPEREX_API FK2NodeCompilerHelper
{
public:
	UEdGraphNode* Node;
	class FKismetCompilerContext& CompilerContext;
	class UEdGraph* SourceGraph;
	UEdGraphPin* LastThenPin;
	UEdGraphPin* FinishPin;
	bool bIsErrorFree = true;
	bool bFinalize = true;


public:
	FK2NodeCompilerHelper(UEdGraphNode* Node, class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, UEdGraphPin* StartPin, UEdGraphPin* FinishPin = nullptr, bool bFinalize = true)
		: Node(Node)
		, CompilerContext(CompilerContext)
		, SourceGraph(SourceGraph)
		, LastThenPin(StartPin)
		, FinishPin(FinishPin)
		, bFinalize(bFinalize)
	{
	}

	FK2NodeCompilerHelper(const FK2NodeCompilerHelper& Compiler, UEdGraphPin* StartPin, UEdGraphPin* FinishPin = nullptr)
		: Node(Compiler.Node)
		, CompilerContext(Compiler.CompilerContext)
		, SourceGraph(Compiler.SourceGraph)
		, LastThenPin(StartPin)
		, FinishPin(FinishPin)
		, bFinalize(false)
	{
	}

	~FK2NodeCompilerHelper();


public:
	FString GetGuid() const
	{
		return CompilerContext.GetGuid(Node);
	}


public:
	UK2Node_TemporaryVariable* SpawnInternalVariable(const struct FEdGraphPinType& PinType);

	template <typename NodeType, typename... Ts>
	NodeType* SpawnIntermediateEventNode(UEdGraphPin* SourcePin, Ts... args)
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

		ConnectToExecChain(Result);

		return Result;
	}

	template <typename NodeType, typename... Ts>
	NodeType* SpawnIntermediateNode(Ts... args)
	{
		auto Result = CompilerContext.SpawnIntermediateNode<NodeType>(Node, SourceGraph);
		SetupNode(Result, args...);
		Result->AllocateDefaultPins();
		ConnectNode(Result, args...);

		ConnectToExecChain(Result);

		return Result;
	}


public:
	template <typename NodeType>
	UEdGraphPin* GetExecPin(NodeType* Node)
	{
		return Node->FindPin(UEdGraphSchema_K2::PN_Execute);
	}

	template <typename NodeType>
	UEdGraphPin* GetThenPin(NodeType* Node)
	{
		return Node->FindPin(UEdGraphSchema_K2::PN_Then);
	}


protected:
	template <typename NodeType>
	void ConnectToExecChain(NodeType* Node)
	{
		auto ExecPin = GetExecPin(Node);
		if (LastThenPin && !Node->IsNodePure() && ExecPin)
		{
			ConnectPins(LastThenPin, ExecPin);
			LastThenPin = GetThenPin(Node);
		}
	}


public:
	UEdGraphPin* CacheInLocalVariable(UEdGraphPin* ObjectPin);
	UK2Node_CallFunction* SpawnIsValidNode(UEdGraphPin* ObjectPin);


	bool ConnectPins(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin, bool bOptional = false);

	bool CreateSetParamByNameNodes(UEdGraphPin* ObjectPin, UEdGraphPin* SpawnVarPin);

protected:
	void SetupNode(UK2Node_Event* Event, FName CustomFunctionName);
	void SetupNode(UK2Node_Event* Event, FName CustomFunctionName, class UFunction* Function);

	void SetupNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin);
	void ConnectNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin);
	void SetupNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin);
	void ConnectNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin);

	void SetupNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, UEdGraphPin* ValuePin) {}
	void ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, UEdGraphPin* ValuePin);

	void SetupNode(UK2Node_Cache* Cahce, UEdGraphPin* ObjectPin) {}
	void ConnectNode(UK2Node_Cache* Cahce, UEdGraphPin* ObjectPin);

	void SetupNode(UK2Node_CallFunction* CallFunction, class UFunction* Function);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UFunction* Function) {}
	void SetupNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName) {}

	void SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UFunction* Function);
	void ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UFunction* Function);
	void SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName);
	void ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName);

	void SetupNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, UEdGraphPin* ObjectPin);
	void ConnectNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, UEdGraphPin* ObjectPin);

	void SetupNode(UK2Node_EnumLiteral* EnumLiteral, UEdGraphPin* ValuePin, UEdGraphPin* ResultPin);
	void ConnectNode(UK2Node_EnumLiteral* EnumLiteral, UEdGraphPin* ValuePin, UEdGraphPin* ResultPin);

	void SetupNode(UK2Node_IfThenElse* IfThenElse, UEdGraphPin* ConditionPin) {}
	void ConnectNode(UK2Node_IfThenElse* IfThenElse, UEdGraphPin* ConditionPin);

	void SetupNode(UK2Node_Knot* KnotNode, UEdGraphPin* ObjectPin) {}
	void ConnectNode(UK2Node_Knot* KnotNode, UEdGraphPin* ObjectPin);
};

#define EXPAND_FUNCTION_NAME(ClassName, FunctionName) \
	ClassName::StaticClass(), GET_FUNCTION_NAME_CHECKED(ClassName, FunctionName)
