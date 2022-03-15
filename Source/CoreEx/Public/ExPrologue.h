#pragma once

#define DECLARE_CONST_EXTENSION(TypeName) \
struct F##TypeName##ConstEx\
{\
protected:\
	const TypeName* This_;\
public:\
	F##TypeName##ConstEx(const TypeName* This)\
		: This_(This)\
	{\
	}\
	const TypeName* This() const { return This_; }\
public:


#define DECLARE_CONST_EXTENSION_TEMPLATE_REF(TypeName, ...) \
template <EXPAND(EXPAND_4(__VA_ARGS__))>\
struct F##TypeName##ConstEx\
{\
protected:\
	typedef TypeName<__VA_ARGS__> ThisType;\
	const TypeName<__VA_ARGS__>& This_;\
public:\
	F##TypeName##ConstEx(const TypeName<__VA_ARGS__>& This)\
		: This_(This)\
	{\
	}\
	const TypeName<__VA_ARGS__>& This() const { return This_; }\
public:

#define DECLARE_MUTABLE_EXTENSION_TEMPLATE_REF(TypeName, ...) \
template <EXPAND(EXPAND_4(__VA_ARGS__))>\
struct F##TypeName##MutableEx : public F##TypeName##ConstEx<__VA_ARGS__>\
{\
public:\
	F##TypeName##MutableEx(TypeName<__VA_ARGS__>& This)\
		: F##TypeName##ConstEx<__VA_ARGS__>(This)\
	{\
	}\
	TypeName<__VA_ARGS__>& This() { return const_cast<TypeName<__VA_ARGS__>&>(F##TypeName##ConstEx::This()); }\
public:

