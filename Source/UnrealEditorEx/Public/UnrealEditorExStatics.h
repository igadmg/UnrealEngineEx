#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Slate/MenuBuilder.h"

#include "UnrealEditorExStatics.generated.h"



UCLASS()
class UNREALEDITOREX_API UUnrealEditorExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "UnrealEditorEx", BlueprintCallable)
	static void SelectActorComponent(UActorComponent* InActorComponent);


	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	static FSlateUIAction MakeUIAction(FSlateExecuteAction ExecuteAction)
	{
		return FSlateUIAction { ExecuteAction };
	}

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	static FSlateUIAction MakeUIActionC(FSlateExecuteAction ExecuteAction, FSlateCanExecuteAction CanExecuteAction)
	{
		return FSlateUIAction { ExecuteAction, CanExecuteAction };
	}

	UFUNCTION(Category = "UnrealEditorEx", BlueprintCallable)
	static void FixDefaultObject(UClass* Class);
};
