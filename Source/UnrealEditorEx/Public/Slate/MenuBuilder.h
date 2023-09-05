#pragma once

#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "MenuBuilder.generated.h"


DECLARE_DYNAMIC_DELEGATE(FSlateExecuteAction);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSlateCanExecuteAction, bool&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSlateIsActionChecked, bool&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSlateGetActionCheckState, ECheckBoxState&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSlateIsActionButtonVisible, bool&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSlateNewMenuDelegate, class UMenuBuilder*, MenuBuilder);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAddArrayMenuEntriesAction, UObject*, Object);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FAddArrayNewMenuDelegate, class UMenuBuilder*, MenuBuilder, UObject*, Object);


USTRUCT(BlueprintType)
struct UNREALEDITOREX_API FSlateUIAction
{
	GENERATED_BODY()


	/** Holds a delegate that is executed when this action is activated. */
	UPROPERTY(Category = "SlateUIAction", BlueprintReadWrite)
	FSlateExecuteAction ExecuteAction;

	/** Holds a delegate that is executed when determining whether this action can execute. */
	UPROPERTY(Category = "SlateUIAction", BlueprintReadWrite)
	FSlateCanExecuteAction CanExecuteAction;

	/** Holds a delegate that is executed when determining the check state of this action. */
	UPROPERTY(Category = "SlateUIAction", BlueprintReadWrite)
	FSlateGetActionCheckState GetActionCheckState;

	/** Holds a delegate that is executed when determining whether this action is visible. */
	UPROPERTY(Category = "SlateUIAction", BlueprintReadWrite)
	FSlateIsActionButtonVisible IsActionVisibleDelegate;


	FUIAction ToSlateObject() const
	{
		FUIAction UIAction;
		if (ExecuteAction.IsBound())
		{
			auto LocalAction = ExecuteAction;
			UIAction.ExecuteAction = FExecuteAction::CreateLambda([LocalAction]() {
				LocalAction.Execute();
			});
		}
		if (CanExecuteAction.IsBound())
		{
			auto LocalAction = CanExecuteAction;
			UIAction.CanExecuteAction = FCanExecuteAction::CreateLambda([LocalAction]() {
				bool Result = true;
				LocalAction.Execute(Result);
				return Result;
			});
		}
		if (GetActionCheckState.IsBound())
		{
			auto LocalAction = GetActionCheckState;
			UIAction.GetActionCheckState = FGetActionCheckState::CreateLambda([LocalAction]() {
				ECheckBoxState Result = ECheckBoxState::Unchecked;
				LocalAction.Execute(Result);
				return Result;
			});
		}
		if (IsActionVisibleDelegate.IsBound())
		{
			auto LocalAction = IsActionVisibleDelegate;
			UIAction.IsActionVisibleDelegate = FIsActionButtonVisible::CreateLambda([LocalAction]() {
				bool Result = true;
				LocalAction.Execute(Result);
				return Result;
			});
		}

		return UIAction;
	}
};


UCLASS(BlueprintType)
class UNREALEDITOREX_API UMenuBuilder : public UObject
{
	GENERATED_BODY()


public:
	FMenuBuilder* MenuBuilder;
	UMenuBuilder* Init(FMenuBuilder* InMenuBuilder) { MenuBuilder = InMenuBuilder; return this; }
	static UMenuBuilder* FromMenuBuilder(FMenuBuilder* InMenuBuilder);


public:
	UFUNCTION(Category = "MenuBuilder", BlueprintCallable)
	void BeginSection(FName InExtensionHook, FText InHeadingText);

	UFUNCTION(Category = "MenuBuilder", BlueprintCallable)
	void EndSection();

	UFUNCTION(Category = "MenuBuilder", BlueprintCallable)
	void AddMenuEntry(FText InLabel, FText InToolTip, /*const FSlateIcon& InIcon,*/ FSlateUIAction UIAction);

	UFUNCTION(Category = "MenuBuilder", BlueprintCallable)
	void AddArrayMenuEntries(const TArray<UObject*>& Objects, FAddArrayMenuEntriesAction ObjectAction);

	UFUNCTION(Category = "MenuBuilder", BlueprintCallable)
	void AddSubMenu(FText InMenuLabel, FText InToolTip, FSlateNewMenuDelegate SubMenuDelegate);

	UFUNCTION(Category = "MenuBuilder", BlueprintCallable)
	void AddArraySubMenues(const TArray<UObject*>& Objects, FAddArrayNewMenuDelegate SubMenuDelegate);


public:
	UMenuBuilder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

