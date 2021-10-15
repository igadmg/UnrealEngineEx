#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

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
	template <typename T> static T* GetOwnerOfType(const UObject* Object) { return Cast<T>(GetOwnerOfType(Object, T::StaticClass(), (T*)nullptr)); }
	static UObject* GetOwnerOfType(const UObject* Object, const UClass* Class, const UObject* Unused = nullptr);

	template <typename T> static T* GetAssociatedObject(const UObject* Object) { return Cast<T>(GetAssociatedObject(Object, (T*)nullptr)); }
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class AGameModeBase* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class AGameStateBase* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class AHUD* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class APawn* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class APlayerController* Unused = nullptr);
	UNREALENGINEEX_API static UObject* GetAssociatedObject(const UObject* Object, class APlayerState* Unused = nullptr);

#if WITH_EDITOR
	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> ConvertLevelPtrToPIE(const TSoftObjectPtr<UWorld>& Level, class UWorld* World);
	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> ConvertLevelPtrFromPIE(const TSoftObjectPtr<UWorld>& Level, class UWorld* World);
#else
	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> ConvertLevelPtrToPIE(const TSoftObjectPtr<UWorld>& Level, class UWorld* World) { return Level; }
	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> ConvertLevelPtrFromPIE(const TSoftObjectPtr<UWorld>& Level, class UWorld* World) { return Level; }
#endif

	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> GetLevelPtr(UWorld* World);
	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> GetLevelPtr(FName PackageName);
	UNREALENGINEEX_API static TSoftObjectPtr<UWorld> GetLevelPtr(FString PackageName);
	UNREALENGINEEX_API static FString GetLevelName(const TSoftObjectPtr<UWorld>& Level);

	UNREALENGINEEX_API static void FinishLatentAction(FLatentActionManager& LatentManager, const FLatentActionInfo& LatentInfo);
};

#if WITH_EDITOR
#define EDITOR_MODIFY() do { Modify(); } while(0)
#else
#define EDITOR_MODIFY() do {} while(0)
#endif