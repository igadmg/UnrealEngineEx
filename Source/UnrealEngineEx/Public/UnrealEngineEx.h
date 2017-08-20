#pragma once


#include "Engine.h"
#include "ModuleManager.h"

#include "UnrealEngineExTypes.h"



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

	UNREALENGINEEX_API static void SetActorEnabled(class AActor* Actor, bool bIsEnabled);

	UNREALENGINEEX_API static void FinishLatentAction(FLatentActionManager& LatentManager, const FLatentActionInfo& LatentInfo);
};
