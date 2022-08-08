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

	template<class T>
	T* FindComponentByClass(TSubclassOf<T> ComponetClass) const
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UActorComponent>::Value, "'T' template parameter to FindComponentByClass must be derived from UActorComponent");

		return Valid<T>(This()->FindComponentByClass(ComponetClass));
	}

	template <typename T>
	void ForEachAttachedActorOfType(TFunctionRef<bool(T*)> Functor) const
	{
		This()->ForEachAttachedActors([&Functor](T* Actor) {
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
		This()->ForEachAttachedActors([&Functor](AActor* Actor) {
			if (auto TA = ValidInterface<T>(Actor))
			{
				return Functor(TA);
			}

			return true;
		});
	}

	bool ActorHasAnyTag(const TArray<FName>& Tags) const
	{
		for (auto Tag : Tags)
			if (This()->ActorHasTag(Tag))
				return true;
		return false;
	}
};

DECLARE_MUTABLE_EXTENSION(AActor)
	COMPONENTEX_API	void SetActorEnabled(bool bIsEnabled);

	template <typename T>
	T* AddComponentByClass(bool bManualAttachment = false, const FTransform& RelativeTransform = FTransform::Identity, bool bDeferredFinish = false)
	{
		return AddComponentByClass<T>(T::StaticClass(), bManualAttachment, RelativeTransform, bDeferredFinish);
	}

	template <typename T>
	T* AddComponentByClass(TSubclassOf<T> ComponentClass, bool bManualAttachment = false, const FTransform& RelativeTransform = FTransform::Identity, bool bDeferredFinish = false)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UActorComponent>::Value, "'T' template parameter to AddComponentByClass must be derived from UActorComponent");

		return Valid<T>(This()->AddComponentByClass(ComponentClass, bManualAttachment, RelativeTransform, bDeferredFinish));
	}

	template <typename T>
	T* AddComponentByClass(bool bManualAttachment, const FTransform& RelativeTransform, TFunction<void (T*)> InitizlieFn)
	{
		return AddComponentByClass<T>(T::StaticClass(), bManualAttachment, RelativeTransform, InitizlieFn);
	}

	template <typename T>
	T* AddComponentByClass(TSubclassOf<T> ComponentClass, TFunction<void(T*)> InitizlieFn)
	{
		return AddComponentByClass(ComponentClass, false, FTransform::Identity, InitizlieFn);
	}

	template <typename T>
	T* AddComponentByClass(TSubclassOf<T> ComponentClass, bool bManualAttachment, const FTransform& RelativeTransform, TFunction<void(T*)> InitizlieFn)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UActorComponent>::Value, "'T' template parameter to AddComponentByClass must be derived from UActorComponent");

		if (auto Component = Valid<T>(This()->AddComponentByClass(ComponentClass, bManualAttachment, RelativeTransform, true)))
		{
			InitizlieFn(Component);

			This()->FinishAddComponent(Component, bManualAttachment, RelativeTransform);

			return Component;
		}

		return nullptr;
	}

	template <typename T>
	T* AddComponent(T* Component, bool bManualAttachment = false, const FTransform& RelativeTransform = FTransform::Identity)
	{
		This()->AddOwnedComponent(Component);
		This()->FinishAddComponent(Component, bManualAttachment, RelativeTransform);

		return Component;
	}

	void RemoveComponent(UActorComponent* Component)
	{
		if (Component->IsBeingDestroyed()) return;

		if (Component->HasBegunPlay())
		{
			Component->EndPlay(EEndPlayReason::Destroyed);
		}

		// Ensure that we call UninitializeComponent before we destroy this component
		if (Component->HasBeenInitialized())
		{
			Component->UninitializeComponent();
		}

		// Unregister if registered
		if (Component->IsRegistered())
		{
			Component->UnregisterComponent();
		}

		// Then remove from Components array, if we have an Actor
		This()->RemoveOwnedComponent(Component);
		if (This()->GetRootComponent() == Component)
		{
			This()->SetRootComponent(nullptr);
		}

		Component->OnComponentDestroyed(false);
	}
};

DECLARE_EXTENSION(AActor);