#pragma once

#include "HitProxy.h"

#include "ComponentVisProxy.generated.h"



UCLASS()
class UNREALEDITOREX_API UComponentVisProxy : public UHitProxy
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly)
	const class UActorComponent* Component;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EHitProxyPriority> Priority;


public:
	UComponentVisProxy(const FObjectInitializer& ObjectInitializer);


public:
	virtual void Initialize(HHitProxy* HitProxy) override;
	virtual HHitProxy* ToHHitProxy() override;
};

