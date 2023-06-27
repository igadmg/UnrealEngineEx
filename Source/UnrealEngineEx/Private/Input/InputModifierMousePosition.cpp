#include "Input/InputModifierMousePosition.h"

#include "GameFramework/PlayerController.h"
#include "EnhancedPlayerInput.h"

#include "UnrealEngineEx.final.h"


FInputActionValue UInputModifierMousePosition::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (auto PlayerController = XX::GetController<APlayerController>(PlayerInput))
	{
		FVector2D MousePosition;
		if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			return FInputActionValue(MousePosition);
		}
	}

	return CurrentValue;
}
