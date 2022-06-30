#include "UserWidgetEx.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"
#include "Components/CanvasPanelSlot.h"

#include "UnrealEngineEx.final.h"


UUserWidgetEx::UUserWidgetEx(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bGeometryUpdated(false)
{
}

void UUserWidgetEx::MarkGeometryDirty_Implementation()
{
	bGeometryUpdated = false;
}

void UUserWidgetEx::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	if (PreventFocusChange % InFocusEvent.GetCause() && PreviousFocusPath.GetLastWidget() == TakeWidget())
	{
		int32 UserIndex = InFocusEvent.GetUser();
		FSlateUser* SlateUser = nullptr;
		FSlateApplication::Get().ForEachUser([UserIndex, &SlateUser](FSlateUser& User) { if (UserIndex == User.GetUserIndex()) SlateUser = &User; });

		if (SlateUser)
		{
			// TODO: not working
			//SlateUser->IncrementFocusVersion();
			return; // success on preventing focus switch.
		}
	}

	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
}

int32 UUserWidgetEx::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	auto Result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FVector2D CurrentWidgetSize;
	if (TSharedPtr<SWidget> CachedWidget = GetCachedWidget())
	{
		CurrentWidgetSize = CachedWidget->GetCachedGeometry().GetAbsoluteSize();
	}

	if (!bGeometryUpdated || (PrevWidgetSize != CurrentWidgetSize))
	{
		UUserWidgetEx* Widget = const_cast<UUserWidgetEx*>(this);
		Widget->bGeometryUpdated = true;
		Widget->PrevWidgetSize = CurrentWidgetSize;
		GetWorld()->GetTimerManager().SetTimerForNextTick(Widget, &UUserWidgetEx::OnGeometryTickHandler);
	}

	return Result;
}

void UUserWidgetEx::OnGeometryTickHandler()
{
	OnGeometryUpdated.Broadcast(GetCachedGeometry());
}
