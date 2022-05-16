#include "DebugEx.h"
#include "Core.h"

#include "DebugEx.final.h"


void FDebugExModule::StartupModule()
{
}

void FDebugExModule::ShutdownModule()
{
}


IMPLEMENT_MODULE(FDebugExModule, DebugEx)

DEFINE_LOG_CATEGORY(LogDebugEx);
//DEFINE_LOG_CATEGORY(LogDebugExInit);
//DEFINE_LOG_CATEGORY(LogDebugExCriticalErrors);


