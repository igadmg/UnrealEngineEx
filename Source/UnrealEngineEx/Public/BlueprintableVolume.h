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
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
#elif ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 0
	virtual void PreEditChange(UProperty* PropertyAboutToChange) override;
#endif
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR


protected:
	void InitializeBrush();
};
