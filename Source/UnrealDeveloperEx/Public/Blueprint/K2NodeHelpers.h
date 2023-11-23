#pragma once

#include "EdGraph/EdGraphPin.h"

class UEdGraphNode;
class UEdGraphPin;

class UNREALDEVELOPEREX_API FK2NodeHelpers
{
public:
	static FEdGraphPinType MakePinType(FName InPinCategory, EPinContainerType PinContainerType = EPinContainerType::None)
	{
		return FEdGraphPinType(InPinCategory, NAME_None, nullptr, PinContainerType, false, FEdGraphTerminalType());
	}

	static FEdGraphPinType MakePinType(FName InPinCategory, UClass* InPinSubCategoryObject, EPinContainerType PinContainerType = EPinContainerType::None)
	{
		return FEdGraphPinType(InPinCategory, NAME_None, InPinSubCategoryObject, PinContainerType, false, FEdGraphTerminalType());
	}


public:
	static bool CreateInputPins(UEdGraphNode* Node, class UFunction* Function);

	static void CreateOutputPins(UEdGraphNode* Node, class FMulticastDelegateProperty* Property, FName PropertyName = NAME_None);
	static void CreateOutputPins(UEdGraphNode* Node, class FMulticastDelegateProperty* Property, UEdGraphPin*& ExecPin, TArray<UEdGraphPin*>& ParameterPins, FName PropertyName = NAME_None);
	static void CreateOutputPins(UEdGraphNode* Node, class UClass* Class);

	static FEdGraphPinType GetWildcardPinType(UEdGraphPin* Pin, EPinContainerType PinContainerType = EPinContainerType::None);
	static FEdGraphPinType GetWildcardPinObjectType(UEdGraphPin* Pin, UClass* ObjectClass, EPinContainerType PinContainerType = EPinContainerType::None);
	static UClass* GetPinTypeClass(UEdGraphPin* Pin, const FEdGraphPinType& PinType);

	static UEdGraphNode* GetLinkedPinNode(UEdGraphPin* Pin);


#if 0
	template <typename T>
	static void ForEachProperty(UClass* ObjectClass, TFunction<void (T*)> Predicate)
	{
		for (TFieldIterator<T> PropertyIt(ObjectClass); PropertyIt; ++PropertyIt)
		{
			Predicate(*PropertyIt);
		}
	}
	void ForEachOutputDelegate(class UClass* ObjectClass, TFunction<void(FMulticastDelegateProperty*)> Predicate);
#endif
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

	bool IsValid() const { return Delegate != nullptr && ExecPin != nullptr; }
};
