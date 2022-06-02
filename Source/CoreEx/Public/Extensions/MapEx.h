#pragma once

#include "ExPrologue.h"
#include "Containers/Map.h"


DECLARE_CONST_EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs)
	FORCEINLINE ValueType FindRef(typename ThisType::KeyConstPointerType Key, const ValueType& DefaultValue)
	{
		if (auto ValuePtr = This().Find(Key))
		{
			return *ValuePtr;
		}

		return DefaultValue;
	}
};

DECLARE_MUTABLE_EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs)
	FORCEINLINE ValueType& FindOrAdd(const KeyType& Key, TFunction<ValueType(const KeyType&)> ValueConstructor)
	{
		auto KeyHash = KeyFuncs::GetKeyHash(Key);
		if (auto* ValuePtr = This().FindByHash(KeyHash, Key))
		{
			return *ValuePtr;
		}

		return This().AddByHash(KeyHash, Key, ValueConstructor(Key));
	}

	FORCEINLINE ValueType& FindOrAddConditionally(const KeyType& Key, TFunction<ValueType(const KeyType&, bool&)> ValueConstructor)
	{
		auto KeyHash = KeyFuncs::GetKeyHash(Key);
		if (auto* ValuePtr = This().FindByHash(KeyHash, Key))
		{
			return *ValuePtr;
		}

		bool bAdd = true;
		static ValueType Value;
		Value = ValueConstructor(Key, bAdd);
		if (bAdd)
		{
			return This().AddByHash(KeyHash, Key, Value);
		}

		return Value;
	}
};

EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs);
