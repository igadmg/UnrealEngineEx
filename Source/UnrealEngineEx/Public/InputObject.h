#pragma once

#include "UObject/Object.h"

#include "InputObject.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType)
class UNREALENGINEEX_API UInputObject : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	bool bBlockInput;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	int32 InputPriority;

	UPROPERTY(DuplicateTransient)
	TObjectPtr<class UInputComponent> InputComponent;


public:
	UFUNCTION(BlueprintCallable)
	void EnableInput(class APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void DisableInput(class APlayerController* PlayerController);


public:
	UInputObject(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) { /* No bindings by default.*/ }
};

