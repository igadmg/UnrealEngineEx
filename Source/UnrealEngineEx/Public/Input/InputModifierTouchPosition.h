#pragma once

#include "InputModifiers.h"
#include "InputModifierTouchPosition.generated.h"


UCLASS(meta = (DisplayName = "Touch Position"))
class UNREALENGINEEX_API UInputModifierTouchPosition : public UInputModifier
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TEnumAsByte<ETouchIndex::Type> Touch = ETouchIndex::Touch1;


protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
