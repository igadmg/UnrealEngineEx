#pragma once

#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "Blueprint/K2NodeHelpers.h"
#include "Misc/EngineVersionComparison.h"

#include "K2Node_Cache.generated.h"


UENUM()
enum class ECachingMode : uint8
{
	Auto = 0x00,
	Force,
	None
};


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_Cache
	: public UK2Node
	, public IK2Node_AddPinInterface
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	ECachingMode CachingMode = ECachingMode::Auto;

	UPROPERTY()
	TArray<FEdGraphPinType> InputObjectType;


public:
	DECLARE_PIN(InputObject);
	DECLARE_PIN(OutputObject);
	DECLARE_PIN(Then);

	int GetInputObjectNum() const { return InputObjectType.Num(); }
	UEdGraphPin* GetInputObjectPin(int Index) const;
	UEdGraphPin* GetOutputObjectPin(int Index) const;

	bool ShouldCacheValue(int Index) const;


public:
	UK2Node_Cache(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	virtual bool ShouldShowNodeProperties() const override { return true; }

	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	virtual FText GetMenuCategory() const override;
	virtual void GetMenuActions(class FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual void AllocateDefaultPins() override;
	void AllocateInputObjectPins();
	void AllocateOutputObjectPins();
	void UpdateWildcardPins();

	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void ReconstructNode() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


public: /// IK2Node_AddPinInterface
	void InteractiveAddInputPin();
	virtual void AddInputPin() override;

#if !UE_VERSION_OLDER_THAN(5, 0, 0)
	virtual bool CanRemovePin(const UEdGraphPin* Pin) const override;
	virtual void RemoveInputPin(UEdGraphPin* Pin) override;
#else
	virtual bool CanRemovePin(const UEdGraphPin* Pin) const;
	virtual void RemoveInputPin(UEdGraphPin* Pin);
#endif
};
