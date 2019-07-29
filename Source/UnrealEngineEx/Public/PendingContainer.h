#pragma once

#include <functional>
#include "PendingContainer.generated.h"



DECLARE_DYNAMIC_DELEGATE_OneParam(FPendingContainerOnObjectCallback, UObject*, Object);


UCLASS()
class UNREALENGINEEX_API UPendingContainer : public UObject
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TArray<UObject*> PendingObjects;

	UPROPERTY()
	FPendingContainerOnObjectCallback OnAddObject;
	std::function<void(UObject*)> OnAddObjectFp;
	
	UPROPERTY()
	FPendingContainerOnObjectCallback OnRemoveObject;
	std::function<void(UObject*)> OnRemoveObjectFp;


public:
	UFUNCTION(BlueprintCallable)
	void AddObject(UObject* Object);

	UFUNCTION(BlueprintCallable)
	void RemoveObject(UObject* Object);

	UFUNCTION(BlueprintCallable)
	void Consume(FPendingContainerOnObjectCallback OnObjectAdd, FPendingContainerOnObjectCallback OnObjectRemove);

	template <typename T>
	void Consume(std::function<void(T*)> OnObjectAdd, std::function<void(T*)> OnObjectRemove)
	{
		OnAddObjectFp = [OnObjectAdd](UObject* Object) { OnObjectAdd(Cast<T>(Object)); };
		OnRemoveObjectFp = [OnObjectRemove](UObject* Object) { OnObjectRemove(Cast<T>(Object)); };

		ConsumePendingObjects();
	}


public:
	UPendingContainer(const FObjectInitializer& ObjectInitializer);


protected:
	void ConsumePendingObjects();
};
