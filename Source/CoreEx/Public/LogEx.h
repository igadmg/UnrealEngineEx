#pragma once

#include "EnumEx.h"


#define TEXT_BOOL(Bool) \
	((Bool) ? TEXT("true") : TEXT("false"))

#define TEXT_ENUM(Enum) \
	*EnumToString(Enum)

#define UE_LOG_EX(CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, TEXT("%s[%s] %s %s: ") Format, TEXT_ENUM(::GetNetMode(this)), TEXT_ENUM(::GetNetRole(this)), *this->GetName(), TEXT(__FUNCTION__), ##__VA_ARGS__)

#define UE_LOG_EX_WCO(CategoryName, Verbosity, WorldContextObject, Format, ...) \
	UE_LOG(CategoryName, Verbosity, TEXT("%s[%s] %s %s: ") Format, TEXT_ENUM(::GetNetMode(WorldContextObject)), TEXT_ENUM(::GetNetRole(WorldContextObject)), *WorldContextObject->GetName(), TEXT(__FUNCTION__), ##__VA_ARGS__)
