#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



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
};
