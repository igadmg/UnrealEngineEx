#include "HitProxies/ComponentVisProxy.h"

#include "ComponentVisualizer.h"

#include "CoreEx.h"



UComponentVisProxy::UComponentVisProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UComponentVisProxy::Initialize(HHitProxy* HitProxy)
{
	if (auto MyHitProxy = Valid<HComponentVisProxy>(HitProxy))
	{
		Component = Valid(MyHitProxy->Component);
		Priority = MyHitProxy->Priority;
	}
}

HHitProxy* UComponentVisProxy::ToHHitProxy()
{
	return new HComponentVisProxy(Component, Priority);
}

