#pragma once

#include "K2Node.h"
#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_Cache.generated.h"


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_Cache : public UK2Node
{
	GENERATED_BODY()


protected:
	FEdGraphPinType InputObjectType;


public:
	DECLARE_PIN(InputObject);
	DECLARE_PIN(OutputObject);
	DECLARE_PIN(Then);

	bool ShouldCacheValue() const;


public:
	UK2Node_Cache(const FObjectInitializer& ObjectInitializer);


public:
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	virtual FText GetMenuCategory() const override;
	virtual void GetMenuActions(class FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void ReconstructNode() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
