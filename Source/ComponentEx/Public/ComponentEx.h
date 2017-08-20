#pragma once


#include "ModuleManager.h"



class FComponentExModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};



#ifdef ENGINE_Actor_generated_h
#include "ActorEx.h"
#endif

#ifdef ENGINE_SkeletalMesh_generated_h
#include "SkeletalMeshEx.h"
#endif



struct FComponentEx
{
	COMPONENTEX_API static void SetActorEnabled(AActor* Actor, bool bIsEnabled);
};
