#pragma once

#include "K2Node.h"
#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_DispatchEvents.generated.h"


UENUM(BlueprintType)
enum class EEventDispatchType : uint8
{
	Disabled,
	Once,
	Permanent
};


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_DispatchEvents : public UK2Node
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	TMap<FName, EEventDispatchType> ExposedEvents;
	TMap<FName, class FMulticastDelegateProperty*> EventsByName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObject> FilterByClass;

	UPROPERTY()
	UClass* InputObjectType;


public:
	DECLARE_PIN(InputObject);

	class UClass* GetInputObjectPinType() const;


public:
	UK2Node_DispatchEvents(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	virtual bool ShouldShowNodeProperties() const override { return true; }

	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

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

