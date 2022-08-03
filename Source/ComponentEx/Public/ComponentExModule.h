#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


class FComponentExModule : public IModuleInterface
{
public:
	static inline FComponentExModule& Get() { return FModuleManager::LoadModuleChecked<FComponentExModule>("ComponentEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("ComponentEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
