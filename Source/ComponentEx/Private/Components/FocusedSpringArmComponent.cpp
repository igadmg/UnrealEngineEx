#include "Components/FocusedSpringArmComponent.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"


UFocusedSpringArmComponent::UFocusedSpringArmComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FTransform UFocusedSpringArmComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	auto SocketTransform = Super::GetSocketTransform(InSocketName, TransformSpace);

	return SocketTransform;
}

#undef LOCTEXT_NAMESPACE
