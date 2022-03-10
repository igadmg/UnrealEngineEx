#pragma once

#include <functional>
#include "PendingContainer.generated.h"



DECLARE_DYNAMIC_DELEGATE_OneParam(FPendingContainerOnObjectCallback, UObject*, Object);


UCLASS()
class UNREALENGINEEX_API UPendingContainer : public UObject
{
	GENERATED_BODY()


protected:
	UPROPERTY(Category = "PendingContainer", EditAnywhere)
	TArray<UObject*> PendingObjects;

	UPROPERTY(Category = "PendingContainer", EditAnywhere)
	FPendingContainerOnObjectCallback OnAddObject;
	TFunction<void(UObject*)> OnAddObjectFp;
	
	UPROPERTY(Category = "PendingContainer", EditAnywhere)
	FPendingContainerOnObjectCallback OnRemoveObject;
	TFunction<void(UObject*)> OnRemoveObjectFp;


public:
	UFUNCTION(Category = "UnrealEditorEx|PendingContainer", BlueprintCallable)
	void AddObject(UObject* Object);

	UFUNCTION(Category = "UnrealEditorEx|PendingContainer", BlueprintCallable)
	void RemoveObject(UObject* Object);

	UFUNCTION(Category = "UnrealEditorEx|PendingContainer", BlueprintCallable)
	void Consume(FPendingContainerOnObjectCallback OnObjectAdd, FPendingContainerOnObjectCallback OnObjectRemove);

	template <typename T>
	void Consume(TFunction<void(T*)> OnObjectAdd, TFunction<void(T*)> OnObjectRemove)
	{
		OnAddObjectFp = [OnObjectAdd](UObject* Object) { OnObjectAdd(Cast<T>(Object)); };
		OnRemoveObjectFp = [OnObjectRemove](UObject* Object) { OnObjectRemove(Cast<T>(Object)); };

		ConsumePendingObjects();
	}

	template <typename T>
	void Consume(TArray<T*>& Array)
	{
		OnAddObjectFp = [&Array](UObject* Object) { Array.Add(Cast<T>(Object)); };
		OnRemoveObjectFp = [&Array](UObject* Object) { Array.Remove(Cast<T>(Object)); };

		ConsumePendingObjects();
	}

	void Release()
	{
		OnAddObjectFp = nullptr;
		OnRemoveObjectFp = nullptr;
	}


public:
	UPendingContainer(const FObjectInitializer& ObjectInitializer);


protected:
	void ConsumePendingObjects();
};
