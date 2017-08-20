#pragma once

#include "PeekEditorObject.generated.h"



UCLASS()
class UNREALEDITOREX_API UPeekEditorObject : public UObject
{
	GENERATED_BODY()



public:
	UPROPERTY(Category = "Trace", EditAnywhere, NonTransactional, meta = (DisplayName = "Trace Channel", ShowForTools = "Place"))
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(Category = "Trace", EditAnywhere, NonTransactional, meta = (DisplayName = "Hit Point", ShowForTools = "Place"))
	FTransform HitPont;

	UPROPERTY(Category = "Trace", EditAnywhere, NonTransactional, meta = (DisplayName = "Actor To Move", ShowForTools = "Place"))
	TLazyObjectPtr<AActor> ActorToMove;



public:
	UPeekEditorObject(const FObjectInitializer& ObjectInitializer);



public:
	void SetParent(class FPeekEdMode* Parent)
	{
		ParentMode = Parent;
	}



protected:
	class FPeekEdMode* ParentMode;
};
