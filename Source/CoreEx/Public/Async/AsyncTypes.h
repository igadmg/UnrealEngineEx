#pragma once

#include "AsyncTypes.generated.h"


UENUM(BlueprintType)
enum class EAsyncTaskResult : uint8
{
	Complete,
	Abort,
	Timeout,
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FCoreExOnAsyncTaskFinishedDelegate, EAsyncTaskResult, Result);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FCoreExOnAsyncTaskTickDelegate, float, DeltaTime);
DECLARE_DELEGATE_RetVal_OneParam(bool, FCoreExOnAsyncTaskTickFunction, float);


template <typename UserPolicy>
inline uint32 GetTypeHash(const TDelegateBase<UserPolicy>& A)
{
	return GetTypeHash(A.GetHandle());
}

template <typename UserPolicy>
inline bool operator== (const TDelegateBase<UserPolicy>& A, const TDelegateBase<UserPolicy>& B)
{
	return A.GetHandle() == B.GetHandle();
}
