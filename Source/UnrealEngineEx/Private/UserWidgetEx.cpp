#include "UserWidgetEx.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"

#include "UnrealEngineEx.final.h"


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
