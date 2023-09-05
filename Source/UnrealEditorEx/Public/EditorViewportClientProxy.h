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
	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	TEnumAsByte<ECoordSystem> GetWidgetCoordSystemSpace() const { return EditorViewportClient ? EditorViewportClient->GetWidgetCoordSystemSpace() : COORD_None; }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintCallable, meta = (ExpandEnumAsExecs = "OutWidgetCoordSystemSpace"))
	void SwitchWidgetCoordSystemSpace(TEnumAsByte<ECoordSystem>& OutWidgetCoordSystemSpace) { OutWidgetCoordSystemSpace = GetWidgetCoordSystemSpace(); }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	EWidgetModeBP GetWidgetMode() const { return EditorViewportClient ? (EWidgetModeBP)EditorViewportClient->GetWidgetMode() : EWidgetModeBP::None; }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintCallable, meta = (ExpandEnumAsExecs = "OutWidgetMode"))
	void SwitchWidgetMode(EWidgetModeBP& OutWidgetMode) { OutWidgetMode = GetWidgetMode(); }

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	bool IsAltPressed() const { return EditorViewportClient->IsAltPressed(); }
	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	bool IsCtrlPressed() const { return EditorViewportClient->IsCtrlPressed(); }
	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	bool IsShiftPressed() const { return EditorViewportClient->IsShiftPressed(); }
	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	bool IsCmdPressed() const { return EditorViewportClient->IsCmdPressed(); }


public:
	UEditorViewportClientProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

