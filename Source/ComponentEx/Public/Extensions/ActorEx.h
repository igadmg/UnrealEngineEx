#pragma once

#include "ExPrologue.h"
#include "GameFramework/Actor.h"
#include "ValidEx.h"


DECLARE_CONST_EXTENSION(AActor)
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
};

DECLARE_MUTABLE_EXTENSION(AActor)
	COMPONENTEX_API	void SetActorEnabled(bool bIsEnabled);
};

DECLARE_EXTENSION(AActor);