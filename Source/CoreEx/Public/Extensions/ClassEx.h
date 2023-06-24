#pragma once

#include "ExPrologue.h"

#include "UObject/Class.h"
#include "ValidEx.h"


DECLARE_CONST_EXTENSION(UClass)
	bool IsAssignableFrom(UClass* Class)
	{
		return IsValid(This_) && IsValid(Class)
			&& (Class->IsChildOf(This_) || Class->ImplementsInterface(This_));
	}

	void ForEachOutputDelegate(TFunction<void (FMulticastDelegateProperty*)> Predicate)
	{
		for (TFieldIterator<FProperty> PropertyIt(This_); PropertyIt; ++PropertyIt)
		{
			auto a = DoValidPtrCast<FMulticastDelegateProperty, FProperty>::Valid(*PropertyIt);
			/*if (auto AsMulticastDelegateProperty = Valid<FMulticastDelegateProperty>(*PropertyIt))
			{
				Predicate(AsMulticastDelegateProperty);
			}*/
		}
	}
};

DECLARE_MUTABLE_EXTENSION(UClass)
};

DECLARE_EXTENSION(UClass);
