#pragma once

#include "Blueprint/UserWidget.h"
#include "UserWidgetEx.generated.h"



UCLASS()
class UNREALENGINEEX_API UUserWidgetEx : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (Bitmask, BitmaskEnum = "EFocusCause"))
	int32 PreventFocusChange;


public:
	UUserWidgetEx(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
};
