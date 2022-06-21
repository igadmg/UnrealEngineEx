#include "Blueprint/K2Node_SpawnActorFromPool.h"

#include "Kismet2/BlueprintEditorUtils.h"


#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"



IMPLEMENT_PIN(UK2Node_SpawnActorFromPool, SpawnTransform, "SpawnTransform");
IMPLEMENT_PIN(UK2Node_SpawnActorFromPool, CollisionHandlingOverride, "CollisionHandlingOverride");
IMPLEMENT_PIN(UK2Node_SpawnActorFromPool, Owner, "Owner");

UK2Node_SpawnActorFromPool::UK2Node_SpawnActorFromPool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("UK2Node_SpawnActorFromPool_Tooltip", "Attempts to spawn a new Actor with the specified transform");
}

FSlateIcon UK2Node_SpawnActorFromPool::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.SpawnActor_16x");
	return Icon;
}

FText UK2Node_SpawnActorFromPool::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText NodeTitle = LOCTEXT("UK2Node_SpawnActorFromPool_BaseTitle", "Spawn Actor from Pool");
	if (TitleType != ENodeTitleType::MenuTitle)
	{
		if (UEdGraphPin* ClassPin = GetClassPin())
		{
			if (ClassPin->LinkedTo.Num() > 0)
			{
				// Blueprint will be determined dynamically, so we don't have the name in this case
				NodeTitle = LOCTEXT("UK2Node_SpawnActorFromPool_Title_Unknown", "SpawnActorFromPool");
			}
			else if (ClassPin->DefaultObject == nullptr)
			{
				NodeTitle = LOCTEXT("UK2Node_SpawnActorFromPool_Title_NONE", "SpawnActorFromPool NONE");
			}
			else
			{
				if (CachedNodeTitle.IsOutOfDate(this))
				{
					FText ClassName;
					if (UClass* PickedClass = Cast<UClass>(ClassPin->DefaultObject))
					{
						ClassName = PickedClass->GetDisplayNameText();
					}

					FFormatNamedArguments Args;
					Args.Add(TEXT("ClassName"), ClassName);

					// FText::Format() is slow, so we cache this to save on performance
					CachedNodeTitle.SetCachedText(FText::Format(LOCTEXT("UK2Node_SpawnActorFromPool_Title_Class", "SpawnActorFromPool {ClassName}"), Args), this);
				}
				NodeTitle = CachedNodeTitle;
			}
		}
		else
		{
			NodeTitle = LOCTEXT("UK2Node_SpawnActorFromPool_Title_NONE", "SpawnActorFromPool NONE");
		}
	}
	return NodeTitle;
}

bool UK2Node_SpawnActorFromPool::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	return Super::IsCompatibleWithGraph(TargetGraph) && (!Blueprint || (FBlueprintEditorUtils::FindUserConstructionScript(Blueprint) != TargetGraph && Blueprint->GeneratedClass->GetDefaultObject()->ImplementsGetWorld()));
}

void UK2Node_SpawnActorFromPool::GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const
{
	UClass* ClassToSpawn = GetClassToSpawn();
	const FString ClassToSpawnStr = ClassToSpawn ? ClassToSpawn->GetName() : TEXT("InvalidClass");
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Type"), TEXT("SpawnActorFromPool")));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Class"), GetClass()->GetName()));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Name"), GetName()));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("ActorClass"), ClassToSpawnStr));
}

FNodeHandlingFunctor* UK2Node_SpawnActorFromPool::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new FNodeHandlingFunctor(CompilerContext);
}


UClass* UK2Node_SpawnActorFromPool::GetClassPinBaseClass() const
{
	return AActor::StaticClass();
}

bool UK2Node_SpawnActorFromPool::IsSpawnVarPin(UEdGraphPin* Pin) const
{
	UEdGraphPin* ParentPin = Pin->ParentPin;
	while (ParentPin)
	{
		if (ParentPin->PinName == PN_SpawnTransform)
			return false;

		ParentPin = ParentPin->ParentPin;
	}

	return(Super::IsSpawnVarPin(Pin) &&
		Pin->PinName != PN_CollisionHandlingOverride &&
		Pin->PinName != PN_SpawnTransform &&
		Pin->PinName != PN_Owner);
}

void UK2Node_SpawnActorFromPool::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// Transform pin
	UScriptStruct* TransformStruct = TBaseStructure<FTransform>::Get();
	UEdGraphPin* TransformPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TransformStruct, PN_SpawnTransform);

	// Collision handling method pin
	UEnum* const MethodEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("ESpawnActorCollisionHandlingMethod"), true);
	UEdGraphPin* const CollisionHandlingOverridePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, MethodEnum, PN_CollisionHandlingOverride);
	CollisionHandlingOverridePin->DefaultValue = MethodEnum->GetNameStringByValue(static_cast<int>(ESpawnActorCollisionHandlingMethod::Undefined));

	UEdGraphPin* OwnerPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), PN_Owner);
	OwnerPin->bAdvancedView = true;

	if (ENodeAdvancedPins::NoPins == AdvancedPinDisplay)
	{
		AdvancedPinDisplay = ENodeAdvancedPins::Hidden;
	}
}



#undef LOCTEXT_NAMESPACE
