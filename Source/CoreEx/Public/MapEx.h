#pragma once

#include "ExPrologue.h"
#include "Containers/Map.h"


DECLARE_CONST_EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs)
	typename ValueType FindRef(typename ThisType::KeyConstPointerType Key, const ValueType& DefualtValue)
	{
		if (auto ValuePtr = This().Find(Key))
		{
			return *ValuePtr;
		}

		return DefualtValue;
	}
};

#define EXTENSION_TEMPLATE_REF(TypeName, ...)\
template <EXPAND(EXPAND_4(__VA_ARGS__))>\
F##TypeName##ConstEx<__VA_ARGS__> F##TypeName##Ex(const TypeName<__VA_ARGS__>& This) { return F##TypeName##ConstEx<__VA_ARGS__>(This); }

EXTENSION_TEMPLATE_REF(TMap, KeyType, ValueType, SetAllocator, KeyFuncs);
