#pragma once

#include "Components/Widget.h"
#include "Engine/Canvas.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "HitProxies/ComponentVisProxy.h"
#include "Slate/MenuBuilder.h"
#include "UObject/GCObject.h"
#include "UObject/UObjectGlobals.h"
#include "ComponentVisualizer.h"
#include "EditorViewportClientProxy.h"
#include "HitProxy.h"
#include "PrimitiveDrawInterface.h"
#include "ViewportProxy.h"

#include "CoreEx.h"

#include <functional>
#include <type_traits>

#include "BlueprintComponentVisualizer.generated.h"



UCLASS(Abstract, Blueprintable, BlueprintType)
class UNREALEDITOREX_API UBlueprintComponentVisualizer : public UObject
{
	GENERATED_BODY()

public:
	virtual class UWorld* GetWorld() const override;

	UBlueprintComponentVisualizer* Init(FComponentVisualizer* InRawComponentVisualizer)
	{
		RawComponentVisualizer = InRawComponentVisualizer;
		return this;
	}

	UBlueprintComponentVisualizer* Init(const FComponentVisualizer* InRawComponentVisualizer) const
	{
		RawComponentVisualizer = const_cast<FComponentVisualizer*>(InRawComponentVisualizer);
		return const_cast<UBlueprintComponentVisualizer*>(this);
	}


public:
	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool DrawVisualization(const UActorComponent* Component, UPrimitiveDrawInterface* PDI);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool DrawVisualizationHUD(const UActorComponent* Component, /*const class FViewport* Viewport, */class UCanvas* Canvas);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool GetComponentLocation(UEditorViewportClientProxy* ViewportClient, FVector& OutLocation);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool GetCustomInputCoordinateSystem(UEditorViewportClientProxy* ViewportClient, FTransform& OutTransform);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool VisProxyHandleClick(UEditorViewportClientProxy* ViewportClient, UComponentVisProxy* VisProxy, class UViewportClick* Click);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool HandleInputDelta(UEditorViewportClientProxy* ViewportClient, FVector DeltaTranslate, FRotator DeltaRotate, FVector DeltaScale);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool HandleInputKey(UEditorViewportClientProxy* ViewportClient, FKey Key, EInputEvent Event);

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent, BlueprintPure)
	UActorComponent* GetEditedComponent();
	bool bIsRecursiveCall_GetEditedComponent = false;

	UFUNCTION(Category = "BlueprintComponentVisualizer", BlueprintNativeEvent)
	bool GenerateContextMenu(class UMenuBuilder* MenuBuilder);


protected:
	mutable FComponentVisualizer* RawComponentVisualizer = nullptr;
};


template <typename TBaseClass>
class TAddGCObject : public TBaseClass, public FGCObject
{
public:
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
	}
};

template <typename TBaseClass>
class TBlueprintComponentVisualizer : public TBaseClass
{
	TSubclassOf<UBlueprintComponentVisualizer> ComponentVisualizerClass;
	UBlueprintComponentVisualizer* ComponentVisualizer = nullptr;
	UPrimitiveDrawInterface* uPDI = nullptr;
	UCanvas* uCanvas = nullptr;
	UViewportProxy* uViewport = nullptr;
	UViewportClick* uViewportClick = nullptr;
	UEditorViewportClientProxy* uViewportClient = nullptr;
	UMenuBuilder* uMenuBuilder = nullptr;


public:
	TBlueprintComponentVisualizer(TSubclassOf<UBlueprintComponentVisualizer> ComponentVisualizerClass)
		: TBaseClass()
		, ComponentVisualizerClass(ComponentVisualizerClass)
	{
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		TBaseClass::AddReferencedObjects(Collector);

		if (IsValid(ComponentVisualizer))
		{
			Collector.AddReferencedObject(ComponentVisualizer);

			if (IsValid(uPDI)) Collector.AddReferencedObject(uPDI);
			if (IsValid(uCanvas)) Collector.AddReferencedObject(uCanvas);
			if (IsValid(uViewport)) Collector.AddReferencedObject(uViewport);
			if (IsValid(uViewportClick)) Collector.AddReferencedObject(uViewportClick);
			if (IsValid(uViewportClient)) Collector.AddReferencedObject(uViewportClient);
			if (IsValid(uMenuBuilder)) Collector.AddReferencedObject(uMenuBuilder);
		}
	}

	static bool IsValid(const UObject* Object)
	{
		return ::IsValid(Object) && Object->IsValidLowLevelFast();
	}

	static bool IsCallable(const UObject* Object)
	{
		if (IsValid(Object))
		{
			return !Object->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed);
		}
		return false;
	}

	const UBlueprintComponentVisualizer* GetComponentVisualizer() const
	{
		if (IsCallable(ComponentVisualizer))
			return ComponentVisualizer;

		return nullptr;
	}

	UBlueprintComponentVisualizer* GetComponentVisualizer()
	{
		if (IsCallable(ComponentVisualizer))
			return ComponentVisualizer;

		if (IsValid(ComponentVisualizerClass))
		{
			ComponentVisualizer = Valid(NewObject<UBlueprintComponentVisualizer>(GetTransientPackage(), ComponentVisualizerClass, NAME_None, RF_Transient));
			uPDI = NewObject<UPrimitiveDrawInterface>(GetTransientPackage(), NAME_None, RF_Transient);
			uCanvas = NewObject<UCanvas>(GetTransientPackage(), NAME_None, RF_Transient);
			uViewport = NewObject<UViewportProxy>(GetTransientPackage(), NAME_None, RF_Transient);
			uViewportClick = NewObject<UViewportClick>(GetTransientPackage(), NAME_None, RF_Transient);
			uViewportClient = NewObject<UEditorViewportClientProxy>(GetTransientPackage(), NAME_None, RF_Transient);
			uMenuBuilder = NewObject<UMenuBuilder>(GetTransientPackage(), NAME_None, RF_Transient);

			ComponentVisualizer->Init(this);
		}

		return ComponentVisualizer;
	}

protected:
	virtual void DrawVisualizationHUD(const UActorComponent* Component, const class FViewport* Viewport, const class FSceneView* View, class FCanvas* Canvas) override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			uCanvas->Init(Viewport->GetSizeXY().X, Viewport->GetSizeXY().Y, const_cast<FSceneView*>(View), Canvas);
			if (ComponentVisualizer->Init(this)->DrawVisualizationHUD(Component, uCanvas))
				return;
		}

		TBaseClass::DrawVisualizationHUD(Component, Viewport, View, Canvas);
	}

	virtual void DrawVisualization(const UActorComponent* Component, const class FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			if (ComponentVisualizer->Init(this)->DrawVisualization(Component, uPDI->Init(PDI)))
				return;
		}

		TBaseClass::DrawVisualization(Component, View, PDI);
	}

	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			if (ComponentVisualizer->Init(this)->GetComponentLocation(uViewportClient->Init(ViewportClient), OutLocation))
				return true;
		}

		return TBaseClass::GetWidgetLocation(ViewportClient, OutLocation);
	}

	virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			FTransform Transform;
			if (ComponentVisualizer->Init(this)->GetCustomInputCoordinateSystem(uViewportClient->Init(ViewportClient), Transform))
			{
				OutMatrix = Transform.ToMatrixWithScale();
				return true;
			}
		}

		return TBaseClass::GetCustomInputCoordinateSystem(ViewportClient, OutMatrix);
	}

	virtual bool VisProxyHandleClick(FEditorViewportClient* ViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			if (ComponentVisualizer->Init(this)->VisProxyHandleClick(uViewportClient->Init(ViewportClient), Valid<UComponentVisProxy>(UHitProxy::FromHHitProxy(VisProxy)), uViewportClick->Init(Click)))
				return true;
		}

		return TBaseClass::VisProxyHandleClick(ViewportClient, VisProxy, Click);
	}

	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, class FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			if (ComponentVisualizer->Init(this)->HandleInputDelta(uViewportClient->Init(ViewportClient), DeltaTranslate, DeltaRotate, DeltaScale))
				return true;
		}

		return TBaseClass::HandleInputDelta(ViewportClient, Viewport, DeltaTranslate, DeltaRotate, DeltaScale);
	}

	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			if (ComponentVisualizer->Init(this)->HandleInputKey(uViewportClient->Init(ViewportClient), Key, Event))
				return true;
		}

		return TBaseClass::HandleInputKey(ViewportClient, Viewport, Key, Event);
	}

	virtual UActorComponent* GetEditedComponent() const override
	{
		if (IsCallable(GetComponentVisualizer()) && !ComponentVisualizer->bIsRecursiveCall_GetEditedComponent)
		{
			return ComponentVisualizer->Init(this)->GetEditedComponent();
		}

		return TBaseClass::GetEditedComponent();
	}

	virtual TSharedPtr<SWidget> GenerateContextMenu() const override
	{
		if (IsCallable(GetComponentVisualizer()))
		{
			FMenuBuilder ContextMenuBuilder(true, nullptr);
			if (ComponentVisualizer->Init(this)->GenerateContextMenu(uMenuBuilder->Init(&ContextMenuBuilder)))
			{
				return ContextMenuBuilder.MakeWidget();
			}
		}

		return TBaseClass::GenerateContextMenu();
	}
};


struct FBlueprintComponentVisualizer
{
	/*
	static TSharedPtr<TBlueprintComponentVisualizer<TAddGCObject<FComponentVisualizer>>> Make(TSubclassOf<UBlueprintComponentVisualizer> ComponentVisualizerClass)
	{
		return MakeShareable(new TBlueprintComponentVisualizer<TAddGCObject<FComponentVisualizer>>(ComponentVisualizerClass));
	}
	*/

	template <typename TBaseClass>
	static TSharedPtr<TBlueprintComponentVisualizer<TBaseClass>> Make(TSubclassOf<UBlueprintComponentVisualizer> ComponentVisualizerClass)
	{
		return MakeShareable(new TBlueprintComponentVisualizer<TBaseClass>(ComponentVisualizerClass));
	}
};
