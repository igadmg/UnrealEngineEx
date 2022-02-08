#ifndef COREEX_CoreEx_h
#define COREEX_CoreEx_h

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"


#define EXPAND(x) x
#define GET_MACRO_1_2(_1,_2,NAME,...) NAME
#define GET_MACRO_2_3(_1,_2,_3,NAME,...) NAME


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

#define if_CanExecuteCosmeticEvents(WorldContextObject) \
if (!UKismetSystemLibrary::IsDedicatedServer(WorldContextObject))


#if defined(ENGINE_Actor_generated_h) && !defined(GetNetRole_ENGINE_Actor_generated_h)
#define GetNetRole_ENGINE_Actor_generated_h
static ENetRole GetNetRole(const AActor* Actor)
{
	return Actor->GetLocalRole();
}
#endif

#if defined(ENGINE_ActorComponent_generated_h) && !defined(GetNetRole_ENGINE_ActorComponent_generated_h)
#define GetNetRole_ENGINE_ActorComponent_generated_h
static ENetRole GetNetRole(const UActorComponent* ActorComponent)
{
	return ActorComponent->GetOwnerRole();
}
#endif

static ENetRole GetNetRole(const UObject* WorldContextObject)
{
	return ROLE_None;
}

static ENetMode GetNetMode(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	return World ? World->GetNetMode() : NM_Standalone;
}

#define if_HasAuthority(WorldContextObject) \
if (GetNetRole(WorldContextObject) == ROLE_Authority)

#define if_HasNoAuthority(WorldContextObject) \
if (GetNetRole(WorldContextObject) < ROLE_Authority)

#define if_NotClassDefaultObject(WorldContextObject) \
if ((WorldContextObject->GetFlags() & RF_ClassDefaultObject) == 0)


#if WITH_EDITOR

#define if_PropertyChanged(...) EXPAND(GET_MACRO_1_2(__VA_ARGS__, if_PropertyChanged2, if_PropertyChanged1)(__VA_ARGS__))

#define if_PropertyChanged1(MemberName) \
if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, MemberName))

#define if_PropertyChanged2(ClassName, MemberName) \
if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ClassName, MemberName))

#define if_MemberPropertyChanged(ClassName, MemberName) \
if (PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(ClassName, MemberName))

#endif

#include "LogEx.h"
#include "EnumEx.h"
#include "Throttle.h"

#include "ArrayEx.h"

#endif

#include "IsValidEx.h"
#include "ValidEx.h"
