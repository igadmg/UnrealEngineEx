#pragma once

#include "K2Node.h"
#include "K2Node_AddPinInterface.h"

#include "Blueprint/K2NodeHelpers.h"
#include "Misc/EngineVersionComparison.h"

#include "K2Node_IsValidEx.generated.h"


USTRUCT()
struct FIsValidExOutput
{
	GENERATED_BODY()


		//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		//	FTex
};


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_IsValidEx
	: public UK2Node
	, public IK2Node_AddPinInterface
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, meta = (AllowAbstract))
	TArray<UClass*> UpcastType;

	UPROPERTY(EditAnywhere)
	bool bHideNotValidPin = false;

	UPROPERTY()
	TArray<FEdGraphPinType> InputObjectType;


public:
	DECLARE_PIN(InputObject);
	DECLARE_PIN(ValidObject);
	DECLARE_PIN(IsValid);
	DECLARE_PIN(IsNotValid);

	int GetInputObjectNum() const { return InputObjectType.Num(); }
	UEdGraphPin* GetInputObjectPin(int Index) const;
	UEdGraphPin* GetValidObjectPin(int Index) const;


public:
	UK2Node_IsValidEx(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


public:
	virtual bool ShouldShowNodeProperties() const override { return true; }

	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	virtual FText GetMenuCategory() const override;
	virtual void GetMenuActions(class FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual void AllocateDefaultPins() override;
	virtual void AllocateInputObjectPins();
	virtual void AllocateValidObjectPins();
	virtual void UpdateWildcardPins();

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

