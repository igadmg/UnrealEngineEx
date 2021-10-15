#include "UObject/Object.h"
#include "UObject/ScriptInterface.h"
#include "HitProxies.h"


#ifndef IsValid_Common
#define IsValid_Common
static bool IsValid(const FName& Name)
{
	return Name != NAME_None && Name.IsValid();
}

static bool IsValid(const void* Ptr)
{
	return Ptr != nullptr;
}

static bool IsValid(void* Ptr)
{
	return Ptr != nullptr;
}

FORCEINLINE static bool IsValid(UObject* Test)
{
	return Test && !Test->IsPendingKill();
}

template <typename T>
static bool IsValid(const TSharedPtr<T>& SharedPtr)
{
	return SharedPtr.IsValid();
}

template <typename T>
static bool IsValid(const TWeakObjectPtr<T>& WeakObjectPtr)
{
	return WeakObjectPtr.IsValid();
}

template <typename T>
static bool IsValid(const TScriptInterface<T>& ScriptInterface)
{
	return IsValid(ScriptInterface.GetObject());
}

template <typename T>
static bool IsValid(const TSoftObjectPtr<T>& SoftObjectPtr)
{
	return !SoftObjectPtr.IsNull();
}

template <typename T>
static bool IsValid(const TSubclassOf<T>& SubclassOf)
{
	return SubclassOf.Get() != nullptr;
}

static bool IsValid(const FDelegateHandle& DelegateHandle)
{
	return DelegateHandle.IsValid();
}
#endif

#if defined(UNREALENGINEEX_PropertyAccessor_generated_h) && !defined(IsValid_UNREALENGINEEX_PropertyAccessor_generated_h)
#define IsValid_UNREALENGINEEX_PropertyAccessor_generated_h
static bool IsValid(const FUObjectPropertyAccessor& PropertyAccessor)
{
	return PropertyAccessor.IsValid();
}
#endif

#if defined(ENGINE_BodyInstance_generated_h) && !defined(IsValid_ENGINE_BodyInstance_generated_h)
#define IsValid_ENGINE_BodyInstance_generated_h
static bool IsValid(FBodyInstance* BodyInstance)
{
	return BodyInstance && BodyInstance->IsValidBodyInstance();
}
#endif

#if defined(ENGINE_DataTable_generated_h) && !defined(IsValid_ENGINE_DataTable_generated_h)
#define IsValid_ENGINE_DataTable_generated_h
static bool IsValid(const FDataTableRowHandle& DataTableRowHandle)
{
	return !DataTableRowHandle.IsNull();
}
#endif

#if defined(GAMEPLAYABILITIES_GameplayAbilitySpec_generated_h) && !defined(IsValid_GAMEPLAYABILITIES_GameplayAbilitySpec_generated_h)
#define IsValid_GAMEPLAYABILITIES_GameplayAbilitySpec_generated_h
static bool IsValid(const FGameplayAbilitySpecHandle& GameplayAbilitySpecHandle)
{
	return GameplayAbilitySpecHandle.IsValid();
}
#endif
