#pragma once

#include "EditorViewportClient.h"

#include "ViewportProxy.generated.h"


UCLASS(BlueprintType)
class UNREALEDITOREX_API UViewportProxy : public UObject
{
	GENERATED_BODY()


public:
	UViewportProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS(BlueprintType)
class UNREALEDITOREX_API UViewportClick : public UObject
{
	GENERATED_BODY()


public:
	const FViewportClick* Click; // NOTE: That should work for proxy.
	UViewportClick* Init(const FViewportClick& InClick) { Click = &InClick; return this; }


public:
	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	FKey GetKey() const { return Click->GetKey(); }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	bool IsKey(FKey Key) const { return Click->GetKey() == Key; }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	EInputEvent GetEvent() const { return Click->GetEvent(); }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	bool IsControlDown() const { return Click->IsControlDown(); }


public:
	UViewportClick(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
