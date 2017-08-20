#pragma once

#include "Engine.h"
#include "ModuleManager.h"



class FUnrealDeveloperExModule : public IModuleInterface
{
public:
	static inline FUnrealDeveloperExModule& Get() { return FModuleManager::LoadModuleChecked<FUnrealDeveloperExModule>("UnrealDeveloperEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("UnrealDeveloperEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};



struct FUnrealDeveloperEx
{
	// Declare module static functions here
};
