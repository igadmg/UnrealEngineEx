#ifndef COREEX_CoreEx_h
#define COREEX_CoreEx_h

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"


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
