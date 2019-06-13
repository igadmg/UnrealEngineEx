#pragma once

#include "ModuleManager.h"

#include "UnrealEngineExTypes.h"

#include <type_traits>


class FUnrealEngineExModule : public IModuleInterface
{
public:
	static inline FUnrealEngineExModule& Get() { return FModuleManager::LoadModuleChecked<FUnrealEngineExModule>("UnrealEngineEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("UnrealEngineEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;



public:
	TArray<TWeakObjectPtr<class UAsyncTask>> AsyncTasks;

	void AddAsyncTask(class UAsyncTask* AsyncTask);
	void RemoveAsyncTask(class UAsyncTask* AsyncTask);
};


struct FUnrealEngineEx
{
	template <typename I>
	static bool DoesImplementInterface(const UObject* Object)
	{
		checkf(I::StaticClass()->IsChildOf(UInterface::StaticClass()), TEXT("Interface parameter %s is not actually an interface."), *I::StaticClass()->GetName());

		if (!IsValid(Object))
			return false;

		return Object->GetClass()->ImplementsInterface(I::StaticClass());
	}

	template <typename T> static T* GetAssociatedObject(const UObject* Object) { return Cast<T>(GetAssociatedObject(Object, (T*)nullptr)); }
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class AGameModeBase* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class AGameStateBase* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class AHUD* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class APawn* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class APlayerController* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class APlayerState* Unused = nullptr);

#if WITH_EDITOR
	UNREALENGINEEX_API static TAssetPtr<UWorld> ConvertLevelPtrToPIE(const TAssetPtr<UWorld>& Level, class UWorld* World);
	UNREALENGINEEX_API static TAssetPtr<UWorld> ConvertLevelPtrFromPIE(const TAssetPtr<UWorld>& Level, class UWorld* World);
#else
	UNREALENGINEEX_API static TAssetPtr<UWorld> ConvertLevelPtrToPIE(const TAssetPtr<UWorld>& Level, class UWorld* World) { return Level; }
	UNREALENGINEEX_API static TAssetPtr<UWorld> ConvertLevelPtrFromPIE(const TAssetPtr<UWorld>& Level, class UWorld* World) { return Level; }
#endif

	UNREALENGINEEX_API static TAssetPtr<UWorld> GetLevelPtr(UWorld* World);
	UNREALENGINEEX_API static TAssetPtr<UWorld> GetLevelPtr(FName PackageName);
	UNREALENGINEEX_API static TAssetPtr<UWorld> GetLevelPtr(FString PackageName);
	UNREALENGINEEX_API static FString GetLevelName(const TAssetPtr<UWorld>& Level);

	UNREALENGINEEX_API static void FinishLatentAction(FLatentActionManager& LatentManager, const FLatentActionInfo& LatentInfo);
};

template <typename UT, typename T>
const T* ValidInterface(const T* v) { return IsValid(v) && FUnrealEngineEx::DoesImplementInterface<UT>(v) ? v : nullptr; }

template <typename UT, typename T>
T* ValidInterface(T* v) { return IsValid(v) && FUnrealEngineEx::DoesImplementInterface<UT>(v) ? v : nullptr; }

#define if_Implements(Type, Name, Expression) \
if (auto Name = ValidInterface<U ## Type>(Expression))

#define if_ImplementsT(UType, Name, Expression) \
if (auto Name = ValidInterface<UType>(Expression))

#define if_CanExecuteCosmeticEvents(WorldContextObject) \
if (!UKismetSystemLibrary::IsDedicatedServer(WorldContextObject))


static ENetRole GetNetRole(const AActor* Actor)
{
	return Actor->Role;
}

static ENetRole GetNetRole(const UActorComponent* ActorComponent)
{
	return ActorComponent->GetOwnerRole();
}

#define if_HasAuthority(WorldContextObject) \
if (GetNetRole(WorldContextObject) == ROLE_Authority)

#define if_HasNoAuthority(WorldContextObject) \
if (GetNetRole(WorldContextObject) < ROLE_Authority)


template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline bool operator ! (E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<T>(rhs) == 0;
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator & (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(lhs & (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator & (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) & (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator | (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(lhs | (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator | (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) | (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator ^ (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(lhs ^ (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator ^ (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) ^ (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline bool operator % (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<T>(lhs & rhs) == (1 << static_cast<T>(rhs));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline bool operator % (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<T>(lhs & rhs) == (1 << static_cast<T>(rhs));
}
