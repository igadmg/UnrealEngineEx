#pragma once

#include "HitProxies.h"
#include "CoreEx.h"

#include <functional>

#include "HitProxy.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType)
class UNREALEDITOREX_API UHitProxy : public UObject
{
	GENERATED_BODY()


private:
	static TMap<HHitProxyType*, TFunction<UHitProxy* (HHitProxy* HitProxy)>> HHitProxyToUHitProxy;


public:
	template <typename THHitProxy, typename TUHitProxy>
	static void RegisterHitProxy()
	{
		HHitProxyToUHitProxy.Add(THHitProxy::StaticGetType()
			, [](auto HitProxy) {
				auto Result = NewObject<TUHitProxy>(GetTransientPackage(), NAME_None, RF_Transient);
				Result->Initialize(HitProxy);
				return Result;
			});
	}

	static UHitProxy* FromHHitProxy(HHitProxy* HitProxy)
	{
		if (!IsValid(HitProxy))
			return nullptr;

		auto FnPtr = HHitProxyToUHitProxy.Find(HitProxy->GetType());
		if (FnPtr == nullptr)
			return nullptr;

		return (*FnPtr)(HitProxy);
	}


public:
	UHitProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	virtual void Initialize(HHitProxy* HitProxy) PURE_VIRTUAL(UHitProxy::Initialize(HHitProxy* HitProxy), )
	virtual HHitProxy* ToHHitProxy() PURE_VIRTUAL(UHitProxy::ToHHitProxy(), return nullptr; )
};

