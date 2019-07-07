#pragma once

#include "UnrealEditorEx.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.

//#include "Engine.h"

#if WITH_EDITOR
//#include "UnrealEd.h"
#endif



//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEditorEx, Log, All);

//Logging during module startup
//DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEditorExInit, Log, All);

//Logging for Critical Errors that must always be addressed
//DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEditorExCriticalErrors, Log, All);
