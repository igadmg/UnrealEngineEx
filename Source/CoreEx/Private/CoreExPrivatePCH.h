#pragma once

#include "CoreEx.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
DECLARE_STATS_GROUP(TEXT("CoreEx"), STATGROUP_CoreEx, STATCAT_Advanced);


//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogCoreEx, Log, All);

//Logging during module startup
//DECLARE_LOG_CATEGORY_EXTERN(LogCoreExInit, Log, All);

//Logging for Critical Errors that must always be addressed
//DECLARE_LOG_CATEGORY_EXTERN(LogCoreExCriticalErrors, Log, All);
