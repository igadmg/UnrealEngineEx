#pragma once

#include "Components/ActorComponent.h"
#include "VisualizerComponent.generated.h"



UCLASS(blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UNREALEDITOREX_API UVisualizerComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Visualizer", EditAnywhere)
	FString ComponentName;


public:
	UFUNCTION(Category = "Visualizer", BlueprintPure)
	TArray<UActorComponent*> GetVisualizedComponents() const;


public:
	UVisualizerComponent(const FObjectInitializer& ObjectInitializer);

	void ForEachVisualizedComponent(TFunction<void(UActorComponent*)> IFunc, TFunction<void(UActorComponent*)> EFunc) const;
};
