#include "Components/UserWidgetPoolComponent.h"

#include "ComponentEx.final.h"


#define LOCTEXT_NAMESPACE "ComponentEx"

UUserWidgetPoolComponent::UUserWidgetPoolComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUserWidgetPoolComponent::OnUnregister()
{
	UserWidgetPools.Empty();

	Super::OnUnregister();
}

UUserWidget* UUserWidgetPoolComponent::CreateWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	auto& Pool = ex(UserWidgetPools).FindOrAdd(WidgetClass, [this](auto Key) {
		auto Pool = FUserWidgetPool();
		Pool.SetWorld(GetWorld());
		Pool.SetDefaultPlayerController(XX::GetController<APlayerController>(this));
		return Pool;
	});
	return Pool.GetOrCreateInstance(WidgetClass);
}

void UUserWidgetPoolComponent::ReleaseWidget(UUserWidget* Widget)
{
	if (auto PoolPtr = UserWidgetPools.Find(Widget->GetClass()))
	{
		PoolPtr->Release(Widget);
	}
}

#undef LOCTEXT_NAMESPACE
