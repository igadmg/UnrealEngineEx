#pragma once

#include "LatentActions.h"


DECLARE_DELEGATE_RetVal(bool, FLambdaPendingLatentActionDelegate);

class FDelegatePendingLatentAction : public FPendingLatentAction
{
public:
	FLambdaPendingLatentActionDelegate Delegate;
	FLatentActionInfo LatentInfo;


	FDelegatePendingLatentAction(const FLambdaPendingLatentActionDelegate& Delegate, const FLatentActionInfo& LatentInfo)
		: Delegate(Delegate)
		, LatentInfo(LatentInfo)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if (!Delegate.IsBound())
			Response.FinishAndTriggerIf(true, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);

		if (Delegate.Execute())
		{
			Response.FinishAndTriggerIf(true, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
		}
	}


public:
	static void RunLatent(UObject* WorldContextObject, FLatentActionInfo LatentInfo, FLambdaPendingLatentActionDelegate ActionDelegate)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		if (!IsValid(World))
			return;

		FLatentActionManager& LatentManager = World->GetLatentActionManager();
		if (LatentManager.FindExistingAction<FDelegatePendingLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FDelegatePendingLatentAction* NewAction = new FDelegatePendingLatentAction(ActionDelegate, LatentInfo);
			LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
		}
	}
};
