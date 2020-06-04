#pragma once

#include "UnrealEngineEx.h"

#if WITH_EDITOR
#include "UnrealEd.h"
#endif



//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEngineEx, Log, All);

//Logging during module startup
//DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEngineExInit, Log, All);

//Logging for Critical Errors that must always be addressed
//DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEngineExCriticalErrors, Log, All);
