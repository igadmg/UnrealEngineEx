#pragma once

#include "Blueprint/UserWidget.h"
#include "UserWidgetEx.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetGeometryUpdated, const FGeometry&, Geometry);

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

	UPROPERTY(BlueprintAssignable)
	FWidgetGeometryUpdated OnGeometryUpdated;


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MarkGeometryDirty();


protected:
	UFUNCTION()
	void OnGeometryTickHandler();


public:
	UUserWidgetEx(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	//~ Begin UUserWidget interface
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	//~ End UUserWidget interface


protected:
	bool bGeometryUpdated;
	FVector2D PrevWidgetSize;
};
