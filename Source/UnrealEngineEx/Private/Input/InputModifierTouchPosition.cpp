#include "Input/InputModifierTouchPosition.h"

#include "EnhancedPlayerInput.h"

#include "UnrealEngineEx.final.h"


FInputActionValue UInputModifierTouchPosition::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (auto PlayerController = XX::GetController<APlayerController>(PlayerInput))
	{
		FVector2D TouchPosition;
		bool bIsPressed = false;
		PlayerController->GetInputTouchState(Touch, TouchPosition.X, TouchPosition.Y, bIsPressed);
		return bIsPressed ? FInputActionValue(TouchPosition) : FInputActionValue(FVector2D::ZeroVector);
	}

	return CurrentValue;
}
