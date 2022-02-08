#include "UnrealDeveloperEx.h"
#include "Logging/MessageLog.h"
#include "Logging/LogMacros.h"
#include "CoreMinimal.h"



#define LOCTEXT_NAMESPACE "FUnrealDeveloperExModule"



void FUnrealDeveloperExModule::StartupModule()
{
}

void FUnrealDeveloperExModule::ShutdownModule()
{
}



#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FUnrealDeveloperExModule, UnrealDeveloperEx)

//DEFINE_LOG_CATEGORY(LogUnrealDeveloperEx)
//DEFINE_LOG_CATEGORY(LogUnrealDeveloperExInit)
//DEFINE_LOG_CATEGORY(LogUnrealDeveloperExCriticalErrors)
