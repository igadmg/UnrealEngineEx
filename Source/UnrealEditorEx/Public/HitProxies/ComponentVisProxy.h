#pragma once

#include "HitProxy.h"

#include "ComponentVisProxy.generated.h"



UCLASS()
class UNREALEDITOREX_API UComponentVisProxy : public UHitProxy
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "ComponentVisProxy", BlueprintReadOnly)
	const class UActorComponent* Component;

	UPROPERTY(Category = "ComponentVisProxy", BlueprintReadOnly)
	TEnumAsByte<EHitProxyPriority> Priority;


public:
	UComponentVisProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	virtual void Initialize(HHitProxy* HitProxy) override;
	virtual HHitProxy* ToHHitProxy() override;
};

