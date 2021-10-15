#include "HitProxy.h"


TMap<HHitProxyType*, std::function<UHitProxy* (HHitProxy* HitProxy)>> UHitProxy::HHitProxyToUHitProxy;

UHitProxy::UHitProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}



