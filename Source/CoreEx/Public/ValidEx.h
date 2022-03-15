#include "IsValidEx.h"

/**
 Functions to do Casts with Valid checks of different objects of different UE types.

 Smart pointers vill be cast to types of their objects.
 TSmartPtr<AActor> ptrA;
 AActor* A = Valid(ptrA);
 AMyActor* mA = Valid<AMyActor>(ptrA);

 Objects references can be cast to theit types as well.
 TSoftObjectPtr<AActor> ptrA;
 AActor* A = Valid(ptrA);
 AMyActor* mA = Valid<AMyActor>(ptrA);
*/

#ifndef IsValid_Templates
#define IsValid_Templates

template <typename T, typename Enable = void>
struct DoValidPtr
{
	static const T* Valid(const T* Ptr)
	{
		return IsValid(Ptr) ? Ptr : nullptr;
	}

	static T* Valid(T* Ptr)
	{
		return IsValid(Ptr) ? Ptr : nullptr;
	}
};

#define DO_VALID_PTR(Type)\
template <typename T>\
struct DoValidPtr<T, typename std::enable_if<\
		std::is_base_of<Type, T>::value\
	>::type>


template <typename T, typename U, typename Enable = void>
struct DoValidPtrCast
{
	static const T* Valid(const U* Ptr)
	{
		return IsValid(Ptr) ? Cast<T>(Ptr) : nullptr;
	}

	static T* Valid(U* Ptr)
	{
		return IsValid(Ptr) ? Cast<T>(Ptr) : nullptr;
	}
};

#define DO_VALID_PTR_CAST(Type)\
template <typename T, typename U>\
struct DoValidPtrCast<T, U, typename std::enable_if<\
		std::is_base_of<Type, T>::value\
	>::type>

template <typename T>
T* Valid(T* v) { return DoValidPtr<T>::Valid(v); }

template <typename T>
const T* Valid(const T* v) { return DoValidPtr<T>::Valid(v); }

template <typename T, typename U>
T* Valid(U* v) { return DoValidPtrCast<T, U>::Valid(v); }

template <typename T, typename U>
const T* Valid(const U* v) { return DoValidPtrCast<T, U>::Valid(v); }

DO_VALID_PTR_CAST(FField)
{
	static const T* Valid(const U* Ptr)
	{
		return IsValid(Ptr) ? CastField<T>(Ptr) : nullptr;
	}

	static T* Valid(U* Ptr)
	{
		return IsValid(Ptr) ? CastField<T>(Ptr) : nullptr;
	}
};

DO_VALID_PTR_CAST(HHitProxy)
{
	static const T* Valid(const U* Ptr)
	{
		return IsValid(Ptr) ? HitProxyCast<T>(Ptr) : nullptr;
	}

	static T* Valid(U* Ptr)
	{
		return IsValid(Ptr) ? HitProxyCast<T>(Ptr) : nullptr;
	}
};

#if !UE_VERSION_OLDER_THAN(5, 0, 0)
template <typename T>
T* Valid(const TObjectPtr<T>& v) { return IsValid(v) ? v.Get() : nullptr; }
#endif

template <typename T>
T* Valid(const TWeakObjectPtr<T>& v) { return IsValid(v) ? v.Get() : nullptr; }

template <typename T>
const T* Valid(const TWeakObjectPtr<const T>& v) { return IsValid(v) ? v.Get() : nullptr; }

template <typename T, typename U>
T* Valid(const TWeakObjectPtr<U>& v) { return IsValid(v) ? Cast<T>(v.Get()) : nullptr; }

template <typename T, typename U>
const T* Valid(const TWeakObjectPtr<const U>& v) { return IsValid(v) ? Cast<T>(v.Get()) : nullptr; }

template <typename T>
T* Valid(const TScriptInterface<T>& v) { return IsValid(v) ? (T*)(v.GetInterface()) : nullptr; }

template <typename T>
T* Valid(const TSoftObjectPtr<T>& v) { return IsValid(v) ? v.LoadSynchronous() : nullptr; }

template <typename T>
const T* Valid(const TSoftObjectPtr<const T>& v) { return IsValid(v) ? v.LoadSynchronous() : nullptr; }

template <typename T, typename U>
T* Valid(const TSoftObjectPtr<U>& v) { return IsValid(v) ? Cast<T>(v.LoadSynchronous()) : nullptr; }

template <typename T, typename U>
const T* Valid(const TSoftObjectPtr<const U>& v) { return IsValid(v) ? Cast<T>(v.LoadSynchronous()) : nullptr; }

template <typename T>
T* Valid(const FSoftObjectPath& v) { return Cast<T>(v.TryLoad()); }

template <typename T>
typename TSubclassOf<T>::TClassType* Valid(const TSubclassOf<T>& v) { return IsValid(v) ? v.Get() : nullptr; }


template <typename T, typename U, typename Enable = void>
struct TValid
{
	static T* Valid(U* Object)
	{
		return ::Valid<T, U>(Object);
	}
};

template <typename T, typename U>
struct TValid<T, U, typename std::enable_if<std::is_same<T, U>::value>::type>
{
	static T* Valid(U* Object)
	{
		return ::Valid(Object);
	}
};


#define if_Valid(Expression) if (auto if_Valid_Expression = Valid(Expression)) if_Valid_Expression


struct FCoreEx
{
	template <typename UI>
	static bool DoesImplementInterface(const UObject* Object)
	{
		checkf(UI::StaticClass()->IsChildOf(UInterface::StaticClass()), TEXT("Interface parameter %s is not actually an interface."), *UI::StaticClass()->GetName());

		if (!IsValid(Object))
			return false;

		return Object->GetClass()->ImplementsInterface(UI::StaticClass());
	}

	static bool DoesImplementInterface(const UObject* Object, UClass* SomeInterface)
	{
		if (!IsValid(Object))
			return false;

		return Object->GetClass()->ImplementsInterface(SomeInterface);
	}

	static COREEX_API bool IsObjectReinst(UObject* Object);
};

template <typename UT, typename T>
const T* ValidInterface(const T* v) { return IsValid(v) && FCoreEx::DoesImplementInterface<UT>(v) ? v : nullptr; }

template <typename UT, typename T>
T* ValidInterface(T* v) { return IsValid(v) && FCoreEx::DoesImplementInterface<UT>(v) ? v : nullptr; }

#define if_Implements(...) EXPAND(GET_MACRO_2_3(__VA_ARGS__, if_Implements3, if_Implements2)(__VA_ARGS__))

#define if_Implements2(TypeAndName, Expression) \
if (auto TypeAndName = ValidInterface<U ## TypeAndName>(Expression))

#define if_Implements3(Type, Name, Expression) \
if (auto Name = ValidInterface<U ## Type>(Expression))

#define if_ImplementsT(UType, Name, Expression) \
if (auto Name = ValidInterface<UType>(Expression))

#endif
