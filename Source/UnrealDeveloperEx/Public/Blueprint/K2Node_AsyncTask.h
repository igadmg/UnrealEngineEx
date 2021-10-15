#pragma once

#include "K2Node_BaseAsyncTask.h"

#include "K2Node_AsyncTask.generated.h"


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_AsyncTask : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()


	struct UNREALDEVELOPEREX_API FAsyncTaskHelper : public FBaseAsyncTaskHelper
	{
		static bool HandleDelegateImplementation(
			FMulticastDelegateProperty* CurrentProperty, const TArray<FAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs,
			UEdGraphPin* ProxyObjectPin, UClass* ProxyClass, UEdGraphPin*& InOutLastThenPin,
			class FK2NodeCompilerHelper& Compiler);
	};


protected:
	UPROPERTY(Category = "AsyncTask", EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAsyncTask> AsyncTaskClass;


public:
	UK2Node_AsyncTask(const FObjectInitializer& ObjectInitializer);


protected:
	virtual bool ShouldShowNodeProperties() const override { return true; }

	class UEdGraphPin* GetAsyncTaskClassPin();

	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetMenuCategory() const override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
