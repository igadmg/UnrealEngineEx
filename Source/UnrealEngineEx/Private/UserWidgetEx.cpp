#include "UnrealEngineExPrivatePCH.h"
#include "UserWidgetEx.h"

#include "Framework/Application/SlateApplication.h"



UUserWidgetEx::UUserWidgetEx(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUserWidgetEx::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	if (PreventFocusChange % InFocusEvent.GetCause() && PreviousFocusPath.GetLastWidget() == TakeWidget())
	{
		int32 UserIndex = InFocusEvent.GetUser();
		FSlateUser* SlateUser = nullptr;
		FSlateApplication::Get().ForEachUser([UserIndex, &SlateUser](FSlateUser* User) { if (UserIndex == User->GetUserIndex()) SlateUser = User; });

		if (SlateUser)
		{
			SlateUser->UpdateFocusVersion();
			return; // success on preventing focus switch.
		}
	}
	
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
}
