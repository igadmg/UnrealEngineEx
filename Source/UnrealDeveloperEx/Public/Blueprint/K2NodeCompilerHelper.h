#pragma once

#include "KismetCompiler.h"
#include "K2Node_AddDelegate.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_CallArrayFunction.h"
#include "K2Node_CallFunction.h"
#include "K2Node_CreateDelegate.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_EnumLiteral.h"
#include "K2Node_ExecutionSequence.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_Knot.h"
#include "K2Node_RemoveDelegate.h"
#include "K2Node_TemporaryVariable.h"

#include "Blueprint/K2Node_Cache.h"


class UEdGraphNode;
class UEdGraphPin;

/**
Used to pass parameters to spawn node calls
All parameters should be in round brackets (Name, Value)
ex.
	PARAMETERS((TEXT("A"), 1))
	PARAMETERS((TEXT("A"), 1), (TEXT("B"), TEXT("Text")), (TEXT("C"), 1))
*/
#define PARAMETER(X) MP X
#define PARAMETERS(...) FOLD_N(PARAMETER, __VA_ARGS__)

struct FFunctionParameterList
{
	typedef TVariant<UEdGraphPin*, int, bool, FString> Variant;
	typedef std::initializer_list<TPairInitializer<FString, Variant>> Initializer;

	TMap<FName, Variant> list;

	FFunctionParameterList(std::initializer_list<TPairInitializer<FName, Variant>> i)
//		: list(i)
	{
		list.Reserve((int32)i.size());
		for (auto& Element : i)
		{
			list.Add(Element.Key, Element.Value);
		}
	}

	template <typename ...Ks, typename ...Vs>
	FFunctionParameterList(TPairInitializer<Ks, Vs> ...pair)
	{
		(list.Add(FName(pair.Key), Variant(TInPlaceType<Vs>(), pair.Value)), ...);
	}

	template <typename T>
	static Variant v(T v_)
	{
		return Variant(TInPlaceType<T>(), v_);
	}

	template <typename Ks, typename Vs>
	static TPairInitializer<Ks, Vs> p(Ks k, Vs v)
	{
		return TPairInitializer(k, v);
	}
};
typedef FFunctionParameterList FPL;
typedef TPairInitializer<FName, FPL::Variant> FP;

template <typename Ks, typename Vs>
inline TPairInitializer<Ks, Vs> MP(Ks k, Vs v)
{
	return TPairInitializer(k, v);
}

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
	bool bLog = false;


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
	UK2Node_CallFunction* SpawnSetVariableNode(const FEdGraphPinType& PinType);
	UK2Node_CallFunction* SpawnSetVariableNode(UEdGraphPin* SourcePin, FString PropertyName);

	template <typename NodeType, typename... Ts>
	NodeType* SpawnSetVariableNode(UEdGraphPin* SourcePin, Ts... args)
	{
		auto Result = SpawnSetVariableNode<NodeType>(SourcePin);

		return Result;
	}

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

		if (bLog) LogNode(Result);
		ConnectToExecChain(Result);

		return Result;
	}


public:
	template <typename NodeType>
	UEdGraphPin* GetExecPin(NodeType* InNode)
	{
		return InNode->FindPin(UEdGraphSchema_K2::PN_Execute);
	}

	template <typename NodeType>
	UEdGraphPin* GetThenPin(NodeType* InNode)
	{
		return InNode->FindPin(UEdGraphSchema_K2::PN_Then);
	}


protected:
	template <typename NodeType>
	void ConnectToExecChain(NodeType* InNode)
	{
		auto ExecPin = GetExecPin(InNode);
		if (LastThenPin && !InNode->IsNodePure() && ExecPin)
		{
			ConnectPins(LastThenPin, ExecPin);
			LastThenPin = GetThenPin(InNode);
		}
	}


public:
	void LogString(const FString& InString = FString(TEXT("Hello")), bool bPrintToLog = true);

	UEdGraphPin* CacheInLocalVariable(bool Value);
	UEdGraphPin* CacheInLocalVariable(int32 Value);
	UEdGraphPin* CacheInLocalVariable(UEdGraphPin* ObjectPin);
	UK2Node_CallFunction* SpawnIsValidNode(UEdGraphPin* ObjectPin);

	bool ConnectPins(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin, bool bOptional = false);
	bool SetObjectProperty(UEdGraphPin* ObjectPin, UEdGraphPin* SourcePin);


protected:
	void SetupNode(UK2Node_Event* Event, FName CustomFunctionName);
	void SetupNode(UK2Node_Event* Event, FName CustomFunctionName, class UFunction* Function);

	void SetupNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin);
	void ConnectNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin);
	void SetupNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin);
	void ConnectNode(UK2Node_AddDelegate* AddDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin);

	void SetupNode(UK2Node_RemoveDelegate* RemoveDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin);
	void ConnectNode(UK2Node_RemoveDelegate* RemoveDelegate, const FProperty* Property, UEdGraphPin* SelfPin, UEdGraphPin* DelegatePin);

	void SetupNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin) {}
	void ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin);
	void SetupNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, bool Value) {}
	void ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, bool Value);
	void SetupNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, int32 Value) {}
	void ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, int32 Value);
	void SetupNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, UEdGraphPin* ValuePin) {}
	void ConnectNode(UK2Node_AssignmentStatement* AssignmentStatement, UEdGraphPin* VariablePin, UEdGraphPin* ValuePin);

	void SetupNode(UK2Node_Cache* Cahce, UEdGraphPin* ObjectPin) {}
	void ConnectNode(UK2Node_Cache* Cahce, UEdGraphPin* ObjectPin);

	void SetupNode(UK2Node_CallFunction* CallFunction, class UFunction* Function);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UFunction* Function) {}
	void SetupNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName) {}
	//void SetupNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName, TMap<FName, UEdGraphPin*> Params);
	//void ConnectNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName, TMap<FName, UEdGraphPin*> Params);

	void SetupNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName, const FFunctionParameterList& Params);
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName, const FFunctionParameterList& Params);

	template <typename ...Ks, typename ...Vs>
	void SetupNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName, TPairInitializer<Ks, Vs> ...pair)
	{
		auto list = { FP(FName(pair.Key), FPL::Variant(TInPlaceType<Vs>(), pair.Value))... };
		SetupNode(CallFunction, FunctionClass, FunctionName, FFunctionParameterList(list));
	}

	template <typename ...Ks, typename ...Vs>
	void ConnectNode(UK2Node_CallFunction* CallFunction, class UClass* FunctionClass, FName FunctionName, TPairInitializer<Ks, Vs> ...pair)
	{
		auto list = { FP(FName(pair.Key), FPL::Variant(TInPlaceType<Vs>(), pair.Value))... };
		ConnectNode(CallFunction, FunctionClass, FunctionName, FFunctionParameterList(list));
	}

	void SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UFunction* Function);
	void ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UFunction* Function);
	void SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName);
	void ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName);

	void SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName, const FFunctionParameterList& Params);
	void ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName, const FFunctionParameterList& Params);

	template <typename ...Ks, typename ...Vs>
	void SetupNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName, TPairInitializer<Ks, Vs> ...pair)
	{
		auto list = { FP(FName(pair.Key), FPL::Variant(TInPlaceType<Vs>(), pair.Value))... };
		SetupNode(CallFunction, SelfPin, FunctionClass, FunctionName, FFunctionParameterList(list));
	}

	template <typename ...Ks, typename ...Vs>
	void ConnectNode(UK2Node_CallFunction* CallFunction, UEdGraphPin* SelfPin, class UClass* FunctionClass, FName FunctionName, TPairInitializer<Ks, Vs> ...pair)
	{
		auto list = { FP(FName(pair.Key), FPL::Variant(TInPlaceType<Vs>(), pair.Value))... };
		ConnectNode(CallFunction, SelfPin, FunctionClass, FunctionName, FFunctionParameterList(list));
	}

	void SetupNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, UEdGraphPin* ObjectPin);
	void ConnectNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, UEdGraphPin* ObjectPin);
	void SetupNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, UEdGraphPin* ObjectPin, bool bPure);
	void ConnectNode(UK2Node_DynamicCast* DynamicCast, class UClass* CastClass, UEdGraphPin* ObjectPin, bool bPure);

	void SetupNode(UK2Node_EnumLiteral* EnumLiteral, UEdGraphPin* ValuePin);
	void ConnectNode(UK2Node_EnumLiteral* EnumLiteral, UEdGraphPin* ValuePin);

	void SetupNode(UK2Node_IfThenElse* IfThenElse, UEdGraphPin* ConditionPin) {}
	void ConnectNode(UK2Node_IfThenElse* IfThenElse, UEdGraphPin* ConditionPin);

	void SetupNode(UK2Node_Knot* KnotNode, UEdGraphPin* ObjectPin) {}
	void ConnectNode(UK2Node_Knot* KnotNode, UEdGraphPin* ObjectPin);

	/*
	void LogNode(UK2Node_AddDelegate* AddDelegate);
	void LogNode(UK2Node_RemoveDelegate* RemoveDelegate);
	void LogNode(UK2Node_AssignmentStatement* AssignmentStatement);
	void LogNode(UK2Node_Cache* Cahce);
	void LogNode(UK2Node_CallFunction* CallFunction);
	void LogNode(UK2Node_DynamicCast* DynamicCast);
	void LogNode(UK2Node_IfThenElse* IfThenElse);
	*/
	void LogNode(UK2Node* Node);
};

#define EXPAND_FUNCTION_NAME(ClassName, FunctionName) \
	ClassName::StaticClass(), GET_FUNCTION_NAME_CHECKED(ClassName, FunctionName)
