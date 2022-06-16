#pragma once

#include "Blueprint/UserWidget.h"
#include "UserWidgetEx.generated.h"


/**
 That was a UUserWidget which can prevent Focus Change. But it stopped working after UE dev's hid their internals.
 Now I don't know how to do that.
*/
UCLASS()
class UNREALENGINEEX_API UUserWidgetEx : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(Category = "Interaction", EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EFocusCause"))
	int32 PreventFocusChange;


public:
	UUserWidgetEx(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
};
