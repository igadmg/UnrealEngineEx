#pragma once


#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FCoordinateSystemExModule : public IModuleInterface
{
public:
	static inline FCoordinateSystemExModule& Get() { return FModuleManager::LoadModuleChecked<FCoordinateSystemExModule>("CoordinateSystemEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("CoordinateSystemEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
