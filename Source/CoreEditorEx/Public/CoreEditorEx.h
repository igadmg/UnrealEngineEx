#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FCoreEditorExModule : public IModuleInterface
{
public:
	static inline FCoreEditorExModule& Get() { return FModuleManager::LoadModuleChecked<FCoreEditorExModule>("CoreEditorEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("CoreEditorEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


protected:
	FDelegateHandle PreBeginPIEHandle;
	FDelegateHandle OnMapOpenedHandle;
	FDelegateHandle PreSaveWorldWithContextHandle;
};



struct FCoreEditorEx
{
	static bool GatherCookInterfaceObjects(const UWorld* World, TArray<class ICookInterface*>& Objects);
};
