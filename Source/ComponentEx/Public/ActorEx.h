#pragma once

#include "GameFramework/Actor.h"
#include "ValidEx.h"


struct FActorConstEx
{
	FActorConstEx(const AActor* This)
		: This_(This)
	{
	}

	const AActor* This() const { return This_; }

public:
	COMPONENTEX_API class UCameraComponent* GetCameraComponent() const;

	template <typename T>
	TArray<T*> GetComponentsByTag(FName Tag) const
	{
		TInlineComponentArray<T*> ComponentsByClass;
		This()->GetComponents(ComponentsByClass);

		TArray<T*> ComponentsByTag;
		ComponentsByTag.Reserve(ComponentsByClass.Num());
		for (auto Component : ComponentsByClass)
		{
			if (Component->ComponentHasTag(Tag))
			{
				ComponentsByTag.Push(Component);
			}
		}

		return MoveTemp(ComponentsByTag);
	}

	template <typename I>
	TArray<TScriptInterface<I>> GetComponentsOfInterfaceByTag(FName Tag) const
	{
		TArray<TScriptInterface<I>> Components;

		for (UActorComponent* Component : This()->GetComponents())
		{
			if (Component
				&& Component->GetClass()->ImplementsInterface(I::UClassType::StaticClass())
				&& Component->ComponentHasTag(Tag))
			{
				Components.Add(TScriptInterface<I>(Component));
			}
		}

		return Components;
	}

	template <typename T>
	T* GetAttachParentComponent()
	{
		if (auto RootComponent = Valid(This()->GetRootComponent()))
		{
			return Valid<T>(RootComponent->GetAttachParent());
		}

		return nullptr;
	}

	template <typename T>
	void ForEachAttachedActorOfType(TFunctionRef<bool(T*)> Functor) const
	{
		This_->ForEachAttachedActors([&Functor](T* Actor) {
			if (auto TA = Cast<T>(Actor))
			{
				return Functor(TA);
			}

			return true;
		});
	}

	template <typename T>
	void ForEachAttachedActorOfInterface(TFunctionRef<bool(AActor*)> Functor) const
	{
		This_->ForEachAttachedActors([&Functor](AActor* Actor) {
			if (auto TA = ValidInterface<T>(Actor))
			{
				return Functor(TA);
			}

			return true;
		});
	}

private:
	const AActor* This_;
};

struct FActorMutableEx : FActorConstEx
{
	FActorMutableEx(AActor* This)
		: FActorConstEx(This)
	{
	}

	AActor* This() { return const_cast<AActor*>(FActorConstEx::This()); }
public:
};

static FActorConstEx FActorEx(const AActor* This)
{
	return FActorConstEx(This);
}

static FActorMutableEx FActorEx(AActor* This)
{
	return FActorMutableEx(This);
}
