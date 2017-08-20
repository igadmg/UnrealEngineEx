#pragma once

#include "GameFramework/Volume.h"
#include "BlueprintableVolume.generated.h"



UCLASS(abstract, blueprintable, BlueprintType, showcategories = (Blueprint))
class UNREALENGINEEX_API ABlueprintableVolume : public AVolume
{
	GENERATED_BODY()



public:
	ABlueprintableVolume(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
