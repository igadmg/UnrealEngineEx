#pragma once

#include "EditorViewportClient.h"

#include "EditorViewportClientProxy.generated.h"


#if !CPP
UENUM(BlueprintType)
enum ECoordSystem
{
	COORD_None = -1,
	COORD_World = 0,
	COORD_Local,
	COORD_Max,
};
#endif

UENUM(BlueprintType)
enum class EWidgetModeBP : uint8
{
	None = 0xFF,
	Translate = 0,
	TranslateRotateZ,
	Mode2D,
	Rotate,
	Scale
};


UCLASS()
class UNREALEDITOREX_API UEditorViewportClientProxy : public UObject
{
	GENERATED_BODY()


public:
	const FEditorViewportClient* EditorViewportClient;
	UEditorViewportClientProxy* Init(const FEditorViewportClient* InEditorViewportClient) { EditorViewportClient = InEditorViewportClient; return this; }


public:
	UFUNCTION(BlueprintPure)
	TEnumAsByte<ECoordSystem> GetWidgetCoordSystemSpace() const { return EditorViewportClient ? EditorViewportClient->GetWidgetCoordSystemSpace() : COORD_None; }

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "OutWidgetCoordSystemSpace"))
	void SwitchWidgetCoordSystemSpace(TEnumAsByte<ECoordSystem>& OutWidgetCoordSystemSpace) { OutWidgetCoordSystemSpace = GetWidgetCoordSystemSpace(); }

	UFUNCTION(BlueprintPure)
	EWidgetModeBP GetWidgetMode() const { return EditorViewportClient ? (EWidgetModeBP)EditorViewportClient->GetWidgetMode() : EWidgetModeBP::None; }

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "OutWidgetMode"))
	void SwitchWidgetMode(EWidgetModeBP& OutWidgetMode) { OutWidgetMode = GetWidgetMode(); }

	UFUNCTION(BlueprintPure)
	bool IsAltPressed() const { return EditorViewportClient->IsAltPressed(); }
	UFUNCTION(BlueprintPure)
	bool IsCtrlPressed() const { return EditorViewportClient->IsCtrlPressed(); }
	UFUNCTION(BlueprintPure)
	bool IsShiftPressed() const { return EditorViewportClient->IsShiftPressed(); }
	UFUNCTION(BlueprintPure)
	bool IsCmdPressed() const { return EditorViewportClient->IsCmdPressed(); }


public:
	UEditorViewportClientProxy(const FObjectInitializer& ObjectInitializer);
};

