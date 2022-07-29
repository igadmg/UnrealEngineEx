#include "InputObject.h"

#include "Engine/InputDelegateBinding.h"
#include "GameFramework/InputSettings.h"

#include "UnrealEngineEx.final.h"



UInputObject::UInputObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInputObject::EnableInput(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController) || !PlayerController->IsLocalController())
		return;

	// If it doesn't exist create it and bind delegates
	if (!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(PlayerController, UInputSettings::GetDefaultInputComponentClass());
		InputComponent->RegisterComponent();
		InputComponent->bBlockInput = bBlockInput;
		InputComponent->Priority = InputPriority;
	}
	else
	{
		// Make sure we only have one instance of the InputComponent on the stack
		PlayerController->PopInputComponent(InputComponent);
	}

	SetupPlayerInputComponent(InputComponent);
	UInputDelegateBinding::BindInputDelegates(GetClass(), InputComponent, this);

	PlayerController->PushInputComponent(InputComponent);
}

void UInputObject::DisableInput(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController) || !PlayerController->IsLocalController())
		return;

	if (!IsValid(InputComponent))
		return;

	PlayerController->PopInputComponent(InputComponent);
}
