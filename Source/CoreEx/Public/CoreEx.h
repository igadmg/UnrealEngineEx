#ifndef COREEX_CoreEx_h
#define COREEX_CoreEx_h

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "CoreTypesEx.h"

#include <type_traits>


#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define EXPAND(x) x
#define GET_MACRO_1_2(_1,_2,NAME,...) NAME
#define GET_MACRO_2_3(_1,_2,_3,NAME,...) NAME
#define GET_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME

#define FOLD_1(A, X) EXPAND(A(X))
#define FOLD_2(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_1(A, __VA_ARGS__))
#define FOLD_3(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_2(A, __VA_ARGS__))
#define FOLD_4(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_3(A, __VA_ARGS__))
#define FOLD_5(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_4(A, __VA_ARGS__))
#define FOLD_6(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_5(A, __VA_ARGS__))
#define FOLD_7(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_6(A, __VA_ARGS__))
#define FOLD_8(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_7(A, __VA_ARGS__))
#define FOLD_9(A, X, ...) EXPAND(A(X)), EXPAND(FOLD_8(A, __VA_ARGS__))
#define FOLD_N(A, ...) EXPAND(GET_N(__VA_ARGS__, FOLD_9, FOLD_8, FOLD_7, FOLD_6, FOLD_5, FOLD_4, FOLD_3, FOLD_2, FOLD_1)(A, __VA_ARGS__))


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

#define if_HasAuthority(WorldContextObject) \
if (GetNetRole(WorldContextObject) == ROLE_Authority)

#define if_HasNoAuthority(WorldContextObject) \
if (GetNetRole(WorldContextObject) < ROLE_Authority)

static ENetMode GetNetMode(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	return World ? World->GetNetMode() : NM_Standalone;
}

#define if_NotClassDefaultObject(WorldContextObject) \
if ((WorldContextObject->GetFlags() & RF_ClassDefaultObject) == 0)

static TEnumAsByte<EWorldType::Type> GetWorldType(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	return IsValid(World) ? (EWorldType::Type)World->WorldType : EWorldType::None;
}

static bool IsInGame(const UObject* WorldContextObject)
{
	auto WorldType = GetWorldType(WorldContextObject);

	return WorldType == EWorldType::Game
		|| WorldType == EWorldType::PIE
		|| WorldType == EWorldType::GamePreview
		|| WorldType == EWorldType::GameRPC;
}


template <typename RefType, typename AssignedType = RefType>
TGuardValue<RefType, AssignedType> MakeGuardValue(RefType& ReferenceValue, const AssignedType& NewValue)
{
	return TGuardValue<RefType, AssignedType>(ReferenceValue, NewValue);
}


#if WITH_EDITOR

#define if_PropertyChanged(...) EXPAND(GET_MACRO_1_2(__VA_ARGS__, if_PropertyChanged2, if_PropertyChanged1)(__VA_ARGS__))

#define if_PropertyChanged1(MemberName) \
if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, MemberName))

#define if_PropertyChanged2(ClassName, MemberName) \
if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ClassName, MemberName))

#define if_MemberPropertyChanged(ClassName, MemberName) \
if (PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(ClassName, MemberName))

#endif

#include "SubsystemEx.h"

#include "LogEx.h"
#include "EnumEx.h"
#include "Throttle.h"

#include "Extensions/ArrayEx.h"
#include "Extensions/MapEx.h"
#include "Extensions/ClassEx.h"

#endif

#include "IsValidEx.h"
#include "ValidEx.h"
