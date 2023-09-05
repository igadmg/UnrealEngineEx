#pragma once

#include "K2Node.h"
#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_SwitchCast.generated.h"


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_SwitchCast : public UK2Node
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, meta = (AllowAbstract))
	TArray<TSubclassOf<class UObject>> TargetTypes;


public:
	UK2Node_SwitchCast(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	class UEdGraphPin* GetObjectPin() const;
	class UEdGraphPin* GetInvalidCastPin() const;


public:
	virtual bool ShouldShowNodeProperties() const override { return true; }

	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	virtual FText GetMenuCategory() const override;
	virtual void GetMenuActions(class FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void PostReconstructNode() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

