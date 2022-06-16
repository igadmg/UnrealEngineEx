#include "ComponentEx.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

#include "ComponentEx.final.h"



#define LOCTEXT_NAMESPACE "ComponentEx"



void FComponentExModule::StartupModule()
{
}

void FComponentExModule::ShutdownModule()
{
}



IMPLEMENT_MODULE(FComponentExModule, ComponentEx)

//DEFINE_LOG_CATEGORY(LogCoordinateSystemEx);

#undef LOCTEXT_NAMESPACE
