#pragma once

#include "K2Node.h"
#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_ForEachLoop.generated.h"

#define WITH_NOTIFY_GRAPH_PATCH true

UENUM(BlueprintType)
enum class ELoopDirection : uint8
{
	Forward,
	Backward,
};

UENUM(BlueprintType)
enum class ELoopFunction : uint8
{
	Loop,
	Transfrom,
	Filter,
	Accumulate,
};

UCLASS()
class UNREALDEVELOPEREX_API UK2Node_ForEachLoop : public UK2Node
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	ELoopDirection LoopDirection = ELoopDirection::Forward;

	UPROPERTY(EditAnywhere)
	ELoopFunction LoopFunction = ELoopFunction::Loop;

	UPROPERTY(EditAnywhere)
	bool bHaveBreak = false;

	UPROPERTY(EditAnywhere)
	bool bExposeArrayIndex = true;

	UPROPERTY(EditAnywhere)
	bool bExposeCompleted = true;


protected:
	FEdGraphPinType ArrayType;
	FEdGraphPinType ArrayElementType;
	FEdGraphPinType OutputArrayType;
	FEdGraphPinType OutputElementType;

	void UpdateArrayTypes();
	void UpdateOutputTypes();


public:
	DECLARE_PIN(Array);
	DECLARE_PIN(LoopBody);
	DECLARE_PIN(ArrayElement);
	DECLARE_PIN(ArrayIndex);
	DECLARE_PIN(BreakFlag);
	DECLARE_PIN(OutputElement);
	DECLARE_PIN(FilterFlag);
	DECLARE_PIN(Completed);
	DECLARE_PIN(ResultArray);


public:
	UK2Node_ForEachLoop(const FObjectInitializer& ObjectInitializer);


public:
	virtual bool ShouldShowNodeProperties() const override { return true; }

	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	virtual FText GetMenuCategory() const override;
	virtual void GetMenuActions(class FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void ReconstructNode() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
#if WITH_NOTIFY_GRAPH_PATCH
	virtual void NotifyGraphChanged(const struct FEdGraphEditAction& InAction) override;
#endif

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	bool bIsExpandingNode = false;
};

