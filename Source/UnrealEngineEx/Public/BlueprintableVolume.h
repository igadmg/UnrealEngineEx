#pragma once

#include "GameFramework/Volume.h"
#include "BlueprintableVolume.generated.h"



UCLASS(abstract, blueprintable, BlueprintType, showcategories = (Blueprint))
class UNREALENGINEEX_API ABlueprintableVolume : public AVolume
{
	GENERATED_BODY()


public:
	ABlueprintableVolume(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void PostActorCreated() override;

#if WITH_EDITOR
	virtual void PreEditChange(UProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR


protected:
	void InitializeBrush();
};
