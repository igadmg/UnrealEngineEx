#pragma once

#include "K2Node_ConstructObjectFromClass.h"

#include "Blueprint/K2NodeHelpers.h"

#include "K2Node_SpawnActorFromPool.generated.h"


UCLASS()
class UNREALDEVELOPEREX_API UK2Node_SpawnActorFromPool : public UK2Node_ConstructObjectFromClass
{
	GENERATED_BODY()


public:
	DECLARE_PIN(SpawnTransform);
	DECLARE_PIN(CollisionHandlingOverride);
	DECLARE_PIN(Owner);
	DECLARE_PIN(Instigator);


public:
	UK2Node_SpawnActorFromPool(const FObjectInitializer& ObjectInitializer);


protected:
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	virtual void GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const override;
	virtual FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;

	virtual UClass* GetClassPinBaseClass() const override;
	virtual bool IsSpawnVarPin(UEdGraphPin* Pin) const override;

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
};
