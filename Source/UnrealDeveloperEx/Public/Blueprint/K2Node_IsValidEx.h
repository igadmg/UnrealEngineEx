#pragma once

#include "K2Node.h"
#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_IsValidEx.generated.h"


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_IsValidEx : public UK2Node
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UObject> UpcastType;

	UPROPERTY()
	UClass* InputObjectType;


public:
	DECLARE_PIN(InputObject);
	DECLARE_PIN(ValidObject);
	DECLARE_PIN(IsValid);
	DECLARE_PIN(IsNotValid);


public:
	UK2Node_IsValidEx(const FObjectInitializer& ObjectInitializer);


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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

