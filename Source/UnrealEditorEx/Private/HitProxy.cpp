#include "HitProxy.h"


TMap<HHitProxyType*, TFunction<UHitProxy* (HHitProxy* HitProxy)>> UHitProxy::HHitProxyToUHitProxy;

UHitProxy::UHitProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}



