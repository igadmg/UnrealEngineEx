#pragma once

#include "Engine/TriggerVolume.h"
#include "BlueprintTriggerVolume.generated.h"



UINTERFACE(Blueprintable, BlueprintType)
class UNREALENGINEEX_API UBlueprintTriggerVolumeInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALENGINEEX_API IBlueprintTriggerVolumeInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "BlueprintTriggerVolume", BlueprintImplementableEvent)
	void OnActorBeginOverlapVolume(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(Category = "BlueprintTriggerVolume", BlueprintImplementableEvent)
	void OnActorEndOverlapVolume(AActor* OverlappedActor, AActor* OtherActor);
};


UCLASS(showcategories = (Object, Blueprint))
class UNREALENGINEEX_API ABlueprintTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()


public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BlueprintClass;
#endif
	UPROPERTY()
	AActor* BlueprintObject;

	UPROPERTY()
	TScriptInterface<IBlueprintTriggerVolumeInterface> TriggerVolumeInterface;


private:
	UFUNCTION()
	void OnActorBeginOverlapVolume(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorEndOverlapVolume(AActor* OverlappedActor, AActor* OtherActor);


public:
	ABlueprintTriggerVolume(const FObjectInitializer& ObjectInitializer);


public:
	virtual void PreInitializeComponents() override;
	virtual void BeginDestroy() override;

protected:
#if WITH_EDITOR
	void RebuildBlueprintObject();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
