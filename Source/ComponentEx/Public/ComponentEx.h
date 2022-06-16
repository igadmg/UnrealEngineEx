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



#ifdef ENGINE_Actor_generated_h
#include "Extensions/ActorEx.h"
#endif

#ifdef ENGINE_PlayerCameraManager_generated_h
#include "Extensions/PlayerCameraManagerEx.h"
#endif

#ifdef ENGINE_PlayerController_generated_h
#include "Extensions/PlayerControllerEx.h"
#endif

#ifdef ENGINE_ArrowComponent_generated_h
#include "Extensions/ArrowComponentEx.h"
#endif

#ifdef ENGINE_SkeletalMesh_generated_h
#include "Extensions/SkeletalMeshEx.h"
#endif

#ifdef ENGINE_World_generated_h
#include "Extensions/WorldEx.h"
#endif
