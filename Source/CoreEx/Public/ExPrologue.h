#pragma once

#define TYPENAME(X) typename X
#define TYPENAME_N(...) FOLD_N(TYPENAME, __VA_ARGS__)

template <typename T>
struct TEx
{
};

template <typename T>
inline typename TEx<T>::ConstExType ex(const T* O)
{
	return typename TEx<T>::ConstExType(O);
}

template <typename T>
inline typename TEx<T>::MutableExType ex(T* O)
{
	return typename TEx<T>::MutableExType(O);
}

template <typename T, ESPMode InMode>
inline typename TEx<T>::ConstExType ex(const TSharedPtr<const T, InMode>& O)
{
	return typename TEx<T>::ConstExType(O.Get());
}

template <typename T, ESPMode InMode>
inline typename TEx<T>::MutableExType ex(const TSharedPtr<T, InMode>& O)
{
	return typename TEx<T>::MutableExType(O.Get());
}

template <typename T>
inline typename TEx<T>::ConstExRefType const_ex(const T& O)
{
	return typename TEx<T>::ConstExRefType(O);
}

template <typename T>
inline typename TEx<T>::ConstExRefType ex(const T& O)
{
	return typename TEx<T>::ConstExRefType(O);
}

template <typename T>
inline typename TEx<T>::MutableExRefType ex(T& O)
{
	return typename TEx<T>::MutableExRefType(O);
}


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

#define DECLARE_MUTABLE_EXTENSION(TypeName) \
struct F##TypeName##MutableEx : F##TypeName##ConstEx\
{\
	F##TypeName##MutableEx(TypeName* This)\
		: F##TypeName##ConstEx(This)\
	{\
	}\
\
	TypeName* This() { return const_cast<TypeName*>(F##TypeName##ConstEx::This()); }\
public:

#define DECLARE_EXTENSION(TypeName) \
static F##TypeName##ConstEx F##TypeName##Ex(const TypeName* This) { return F##TypeName##ConstEx(This); }\
static F##TypeName##MutableEx F##TypeName##Ex(TypeName* This) { return F##TypeName##MutableEx(This); }\
template <> struct TEx<TypeName>\
{\
	typedef F##TypeName##ConstEx ConstExType;\
	typedef F##TypeName##MutableEx MutableExType;\
}


#define DECLARE_CONST_EXTENSION_TEMPLATE_REF(TypeName, ...) \
template <TYPENAME_N(__VA_ARGS__)>\
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
template <TYPENAME_N(__VA_ARGS__)>\
struct F##TypeName##MutableEx : public F##TypeName##ConstEx<__VA_ARGS__>\
{\
public:\
	F##TypeName##MutableEx(TypeName<__VA_ARGS__>& This)\
		: F##TypeName##ConstEx<__VA_ARGS__>(This)\
	{\
	}\
	TypeName<__VA_ARGS__>& This() { return const_cast<TypeName<__VA_ARGS__>&>(F##TypeName##ConstEx<__VA_ARGS__>::This()); }\
public:


#define EXTENSION_TEMPLATE_REF(TypeName, ...)\
template <TYPENAME_N(__VA_ARGS__)>\
F##TypeName##ConstEx<__VA_ARGS__> F##TypeName##Ex(const TypeName<__VA_ARGS__>& This) { return F##TypeName##ConstEx<__VA_ARGS__>(This); }\
template <TYPENAME_N(__VA_ARGS__)>\
F##TypeName##MutableEx<__VA_ARGS__> F##TypeName##Ex(TypeName<__VA_ARGS__>& This) { return F##TypeName##MutableEx<__VA_ARGS__>(This); }\
template <TYPENAME_N(__VA_ARGS__)> struct TEx<TypeName<__VA_ARGS__>>\
{\
	typedef F##TypeName##ConstEx<__VA_ARGS__> ConstExRefType;\
	typedef F##TypeName##MutableEx<__VA_ARGS__> MutableExRefType;\
}

