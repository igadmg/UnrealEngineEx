#pragma once

#include "Engine.h"
#include "ModuleManager.h"



class FUnrealEditorExModule : public IModuleInterface
{
public:
	static inline FUnrealEditorExModule& Get() { return FModuleManager::LoadModuleChecked<FUnrealEditorExModule>("UnrealEditorEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("UnrealEditorEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};



struct FUnrealEditorEx
{
	// Declare module static functions here
};
