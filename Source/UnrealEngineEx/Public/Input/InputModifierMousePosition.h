#pragma once

#include "InputModifiers.h"
#include "InputModifierMousePosition.generated.h"


UCLASS(meta = (DisplayName = "Mouse Position"))
class UNREALENGINEEX_API UInputModifierMousePosition : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
