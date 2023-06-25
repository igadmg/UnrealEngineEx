#pragma once

#include "Components/ActorComponent.h"
#include "Blueprint/UserWidgetPool.h"

#include "UserWidgetPoolComponent.generated.h"


UCLASS(meta = (BlueprintSpawnableComponent))
class COMPONENTEX_API UUserWidgetPoolComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY(Transient)
	TMap<TSubclassOf<UUserWidget>, FUserWidgetPool> UserWidgetPools;


public:
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "WidgetClass"))
	UUserWidget* CreateWidget(TSubclassOf<UUserWidget> WidgetClass);

	template <typename TWidget>
	TWidget* CreateWidget(TSubclassOf<TWidget> WidgetClass) { return Cast<TWidget>(CreateWidget(TSubclassOf<UUserWidget>(WidgetClass))); }

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "WidgetClass"))
	void ReleaseWidget(UUserWidget* Widget);


public:
	UUserWidgetPoolComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void OnUnregister() override;
};
