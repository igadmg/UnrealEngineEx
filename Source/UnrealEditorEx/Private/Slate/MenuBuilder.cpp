#include "Slate/MenuBuilder.h"

#include "UObject/Package.h"


UMenuBuilder::UMenuBuilder(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMenuBuilder* UMenuBuilder::FromMenuBuilder(FMenuBuilder* InMenuBuilder)
{
	return NewObject<UMenuBuilder>(GetTransientPackage(), NAME_None, RF_Transient)->Init(InMenuBuilder);
}

void UMenuBuilder::BeginSection(FName InExtensionHook, FText InHeadingText)
{
	MenuBuilder->BeginSection(InExtensionHook, InHeadingText);
}

void UMenuBuilder::EndSection()
{
	MenuBuilder->EndSection();
}

void UMenuBuilder::AddMenuEntry(FText InLabel, FText InToolTip, /*const FSlateIcon& InIcon,*/ FSlateUIAction UIAction)
{
	MenuBuilder->AddMenuEntry(InLabel, InToolTip, FSlateIcon(), UIAction.ToSlateObject());
}

void UMenuBuilder::AddArrayMenuEntries(const TArray<UObject*>& Objects, FAddArrayMenuEntriesAction ObjectAction)
{
	for (auto Object : Objects)
	{
		auto LocalObject = Object;
		FUIAction Action;
		Action.ExecuteAction = FExecuteAction::CreateLambda([ObjectAction, LocalObject]() {
			ObjectAction.Execute(LocalObject);
		});
		MenuBuilder->AddMenuEntry(FText::FromString(LocalObject->GetName()), FText(), FSlateIcon(), Action);
	}
}

void UMenuBuilder::AddSubMenu(FText InMenuLabel, FText InToolTip, FSlateNewMenuDelegate SubMenuDelegate)
{
	MenuBuilder->AddSubMenu(InMenuLabel, InToolTip
		, FNewMenuDelegate::CreateLambda([SubMenuDelegate](FMenuBuilder& InMenuBuilder) { SubMenuDelegate.Execute(FromMenuBuilder(&InMenuBuilder)); }));
}

void UMenuBuilder::AddArraySubMenues(const TArray<UObject*>& Objects, FAddArrayNewMenuDelegate SubMenuDelegate)
{
	for (auto Object : Objects)
	{
		auto LocalObject = Object;
		MenuBuilder->AddSubMenu(FText::FromString(LocalObject->GetName()), FText()
			, FNewMenuDelegate::CreateLambda([SubMenuDelegate, LocalObject](FMenuBuilder& InMenuBuilder) { SubMenuDelegate.Execute(FromMenuBuilder(&InMenuBuilder), LocalObject); }));
	}
}
