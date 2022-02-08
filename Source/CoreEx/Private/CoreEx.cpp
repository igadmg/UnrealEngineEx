#include "CoreEx.h"
#include "CoreExPrivatePCH.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"


class FCoreExModule : public IModuleInterface
{
public:
	static inline FCoreExModule& Get() { return FModuleManager::LoadModuleChecked<FCoreExModule>("CoreEx"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("CoreEx"); }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


void FCoreExModule::StartupModule()
{
}

void FCoreExModule::ShutdownModule()
{
}


bool FCoreEx::IsObjectReinst(UObject* Object)
{
	if (!IsValid(Object))
		return false;

	if (auto AsActorComponent = Cast<UActorComponent>(Object))
	{
		return IsObjectReinst(AsActorComponent->GetOwner());
	}
	if (auto AsActor = Cast<AActor>(Object))
	{
		return AsActor->GetName().StartsWith(TEXT("REINST_"));
	}

	return false;
}

IMPLEMENT_MODULE(FCoreExModule, CoreEx)

DEFINE_LOG_CATEGORY(LogCoreEx);
//DEFINE_LOG_CATEGORY(LogCoreExInit);
//DEFINE_LOG_CATEGORY(LogCoreExCriticalErrors);


