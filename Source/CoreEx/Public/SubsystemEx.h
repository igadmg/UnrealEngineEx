#if defined(ENGINE_GameInstanceSubsystem_generated_h) && !defined(GetSubsystem_ENGINE_GameInstanceSubsystem_generated_h)
#define GetSubsystem_ENGINE_GameInstanceSubsystem_generated_h

#include "Kismet/GameplayStatics.h"

namespace XX
{
	template <typename TSubsystemClass = UGameInstanceSubsystem>
	inline TSubsystemClass* GetSubsystem(const UObject* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<UGameInstanceSubsystem, TSubsystemClass>>* = nullptr)
	{
		auto GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		return GameInstance ? GameInstance->GetSubsystem<TSubsystemClass>() : nullptr;
	}

	template <typename TSubsystemClass = UGameInstanceSubsystem>
	inline TSubsystemClass* GetSubsystem(const UGameInstance* GameInstance, typename std::enable_if_t<std::is_base_of_v<UGameInstanceSubsystem, TSubsystemClass>>* = nullptr)
	{
		return GameInstance ? GameInstance->GetSubsystem<TSubsystemClass>() : nullptr;
	}

	template <typename TSubsystemClass = UGameInstanceSubsystem>
	inline TSubsystemClass* GetSubsystem(const UGameInstanceSubsystem* GameInstanceSubsystem, typename std::enable_if_t<std::is_base_of_v<UGameInstanceSubsystem, TSubsystemClass>>* = nullptr)
	{
		auto GameInstance = GameInstanceSubsystem ? GameInstanceSubsystem->GetGameInstance() : nullptr;
		return GameInstance ? GameInstance->GetSubsystem<TSubsystemClass>() : nullptr;
	}

	template <typename TSubsystemClass = UGameInstanceSubsystem>
	inline const TArray<TSubsystemClass*>& GetSubsystemArray(const UObject* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<UGameInstanceSubsystem, TSubsystemClass>>* = nullptr)
	{
		static TArray<TSubsystemClass*> EmptyArray;
		auto GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		return GameInstance ? GameInstance->GetSubsystemArray<TSubsystemClass>() : EmptyArray;
	}

	template <typename TSubsystemClass = UGameInstanceSubsystem>
	inline const TArray<TSubsystemClass*>& GetSubsystemArray(const UGameInstance* GameInstance, typename std::enable_if_t<std::is_base_of_v<UGameInstanceSubsystem, TSubsystemClass>>* = nullptr)
	{
		static TArray<TSubsystemClass*> EmptyArray;
		return GameInstance ? GameInstance->GetSubsystemArray<TSubsystemClass>() : EmptyArray;
	}

	template <typename TSubsystemClass = UGameInstanceSubsystem>
	inline const TArray<TSubsystemClass*>& GetSubsystemArray(const UGameInstanceSubsystem* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<UGameInstanceSubsystem, TSubsystemClass>>* = nullptr)
	{
		static TArray<TSubsystemClass*> EmptyArray;
		auto GameInstance = WorldContextObject ? WorldContextObject->GetGameInstance() : nullptr;
		return GameInstance ? GameInstance->GetSubsystemArray<TSubsystemClass>() : EmptyArray;
	}
}
#endif

#if defined(ENGINE_WorldSubsystem_generated_h) && !defined(GetSubsystem_ENGINE_WorldSubsystem_generated_h)
#define GetSubsystem_ENGINE_WorldSubsystem_generated_h
namespace XX
{
	template <typename TSubsystemClass = UWorldSubsystem>
	inline TSubsystemClass* GetSubsystem(const UObject* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<UWorldSubsystem, TSubsystemClass>>* = nullptr)
	{
		auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		return World ? World->GetSubsystem<TSubsystemClass>() : nullptr;
	}

	template <typename TSubsystemClass = UWorldSubsystem>
	inline const TArray<TSubsystemClass*>& GetSubsystemArray(const UObject* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<UWorldSubsystem, TSubsystemClass>>* = nullptr)
	{
		static TArray<TSubsystemClass*> EmptyArray;
		auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		return World ? World->GetSubsystemArray<TSubsystemClass>() : nullptr;
	}
}
#endif

#if defined(ENGINE_LocalPlayerSubsystem_generated_h) && !defined(GetSubsystem_ENGINE_LocalPlayerSubsystem_generated_h)
#define GetSubsystem_ENGINE_LocalPlayerSubsystem_generated_h

#include "Engine/LocalPlayer.h"

namespace XX
{
	template <typename TSubsystemClass = ULocalPlayerSubsystem>
	inline TSubsystemClass* GetSubsystem(const UObject* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<ULocalPlayerSubsystem, TSubsystemClass>>* = nullptr)
	{
		auto LocalPlayer = XX::GetPlayer<ULocalPlayer>(WorldContextObject);
		return LocalPlayer ? LocalPlayer->GetSubsystem<TSubsystemClass>() : nullptr;
	}

	template <typename TSubsystemClass = ULocalPlayerSubsystem>
	inline const TArray<TSubsystemClass*>& GetSubsystemArray(const UObject* WorldContextObject, typename std::enable_if_t<std::is_base_of_v<ULocalPlayerSubsystem, TSubsystemClass>>* = nullptr)
	{
		static TArray<TSubsystemClass*> EmptyArray;
		auto LocalPlayer = XX::GetPlayer<ULocalPlayer>(WorldContextObject);
		return LocalPlayer ? LocalPlayer->GetSubsystemArray<TSubsystemClass>() : EmptyArray;
	}
}
#endif
