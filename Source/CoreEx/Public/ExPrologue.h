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


#define EXPAND(x) x
#define EXPAND_1(X) typename X
#define EXPAND_2(X, ...) typename X, EXPAND(EXPAND_1(__VA_ARGS__))
#define EXPAND_3(X, ...) typename X, EXPAND(EXPAND_2(__VA_ARGS__))
#define EXPAND_4(X, ...) typename X, EXPAND(EXPAND_3(__VA_ARGS__))

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

