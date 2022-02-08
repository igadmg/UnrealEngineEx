#pragma once

class UEdGraphNode;
class UEdGraphPin;

class UNREALDEVELOPEREX_API FK2NodeHelpers
{
public:
	static bool CreateInputPins(UEdGraphNode* Node, class UFunction* Function);

	static void CreateOutputPins(UEdGraphNode* Node, class FMulticastDelegateProperty* Property);
	static void CreateOutputPins(UEdGraphNode* Node, class FMulticastDelegateProperty* Property, UEdGraphPin*& ExecPin, TArray<UEdGraphPin*>& ParameterPins);
	static void CreateOutputPins(UEdGraphNode* Node, class UClass* Class);

	static FEdGraphPinType GetWildcardPinType(UEdGraphPin* Pin);
	static UClass* GetWildcardPinClassType(UEdGraphPin* Pin);

	static UEdGraphNode* GetLinkedPinNode(UEdGraphPin* Pin);
};

#define DECLARE_PIN(PinName)\
	static FName PN_##PinName;\
	class UEdGraphPin* Get##PinName##Pin() const;

#define IMPLEMENT_PIN(Class, PinName, DisplayName)\
FName Class::PN_##PinName(TEXT(DisplayName));\
class UEdGraphPin* Class::Get##PinName##Pin() const { return FindPin(PN_##PinName); }

#define IMPLEMENT_SCHEMA_PIN(Class, PinName, SchemaName)\
class UEdGraphPin* Class::Get##PinName##Pin() const { return FindPin(SchemaName); }

struct FDelegateAndPins
{
	FMulticastDelegateProperty* Delegate;
	UEdGraphPin* ExecPin;
	TArray<UEdGraphPin*> ParameterPins;

	UNREALDEVELOPEREX_API static FDelegateAndPins FindDelegatePins(UEdGraphNode* Node, FMulticastDelegateProperty* Delegate);
};
