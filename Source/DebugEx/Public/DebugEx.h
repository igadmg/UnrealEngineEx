#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FDebugExModule : public IModuleInterface
{
public:
	static inline FDebugExModule& Get() { return FModuleManager::LoadModuleChecked<FDebugExModule>("DebugEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("DebugEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};



struct FDebugEx
{
	// Declare module static functions here
};
