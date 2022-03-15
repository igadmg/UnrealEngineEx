#pragma once

#include "ExPrologue.h"
#include "Containers/Map.h"


DECLARE_CONST_EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs)
	FORCEINLINE typename ValueType FindRef(typename ThisType::KeyConstPointerType Key, const ValueType& DefualtValue)
	{
		if (auto ValuePtr = This().Find(Key))
		{
			return *ValuePtr;
		}

		return DefualtValue;
	}
};

DECLARE_MUTABLE_EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs)
	FORCEINLINE ValueType FindOrAdd(const KeyType& Key, TFunction<ValueType(const KeyType&)> ValueConstructor)
	{
		auto KeyHash = KeyFuncs::GetKeyHash(Key);
		if (auto* ValuePtr = This().FindByHash(KeyHash, Key))
		{
			return *ValuePtr;
		}

		return This().AddByHash(KeyHash, Key, ValueConstructor(Key));
	}

	FORCEINLINE ValueType FindOrAddConditionally(const KeyType& Key, TFunction<ValueType(const KeyType&, bool&)> ValueConstructor)
	{
		auto KeyHash = KeyFuncs::GetKeyHash(Key);
		if (auto* ValuePtr = This().FindByHash(KeyHash, Key))
		{
			return *ValuePtr;
		}

		bool bAdd = true;
		auto Value = ValueConstructor(Key, bAdd);
		if (bAdd)
		{
			return This().AddByHash(KeyHash, Key, Value);
		}
		
		return Value;
	}
};

#define EXTENSION_TEMPLATE_REF(TypeName, ...)\
template <EXPAND(EXPAND_4(__VA_ARGS__))>\
F##TypeName##ConstEx<__VA_ARGS__> F##TypeName##Ex(const TypeName<__VA_ARGS__>& This) { return F##TypeName##ConstEx<__VA_ARGS__>(This); }\
template <EXPAND(EXPAND_4(__VA_ARGS__))>\
F##TypeName##MutableEx<__VA_ARGS__> F##TypeName##Ex(TypeName<__VA_ARGS__>& This) { return F##TypeName##MutableEx<__VA_ARGS__>(This); }

EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs);
