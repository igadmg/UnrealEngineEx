#include "Blueprint/K2Node_AsyncTask.h"

#include "Blueprint/K2NodeCompilerHelper.h"
#include "Engine/Engine.h"
#include "Async/AsyncTaskStatics.h"
#include "Async/AsyncTask.h"
#include "EdGraphSchema_K2.h"
#include "UnrealEngineExStatics.h"

#include "KismetCompiler.h"
#include "K2Node_AssignmentStatement.h"
#include "Runtime/Launch/Resources/Version.h"

#include "CoreEx.h"

#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


void UK2Node_AsyncTask::FAsyncTaskHelper::HandleDelegateImplementation(
	FMulticastDelegateProperty* CurrentProperty, const TArray<FAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs,
	UEdGraphPin* ProxyObjectPin, UClass* ProxyClass,
	FK2NodeCompilerHelper& Compiler)
{
	const UEdGraphSchema_K2* Schema = Compiler.CompilerContext.GetSchema();
	check(CurrentProperty && ProxyObjectPin && Schema);

	UEdGraphPin* PinForCurrentDelegateProperty = Compiler.Node->FindPin(CurrentProperty->GetFName());
	if (!PinForCurrentDelegateProperty || (UEdGraphSchema_K2::PC_Exec != PinForCurrentDelegateProperty->PinType.PinCategory))
	{
		//FText ErrorMessage = FText::Format(LOCTEXT("WrongDelegateProperty", "BaseAsyncTask: Cannot find execution pin for delegate "), FText::FromString(CurrentProperty->GetName()));
		//CompilerContext.MessageLog.Error(*ErrorMessage.ToString(), CurrentNode);
		Compiler.bIsErrorFree = false;
		return;
	}

	UK2Node_CustomEvent* CurrentCENode = Compiler.SpawnIntermediateEventNode<UK2Node_CustomEvent>(PinForCurrentDelegateProperty
		, FName(*FString::Printf(TEXT("%s_%s"), *CurrentProperty->GetName(), *Compiler.CompilerContext.GetGuid(Compiler.Node))));
	{
		UK2Node_AddDelegate* AddDelegateNode = Compiler.SpawnIntermediateNode<UK2Node_AddDelegate>(CurrentProperty, ProxyObjectPin);

		Compiler.bIsErrorFree &= FBaseAsyncTaskHelper::CreateDelegateForNewFunction(AddDelegateNode->GetDelegatePin(), CurrentCENode->GetFunctionName(), Cast<UK2Node>(Compiler.Node), Compiler.SourceGraph, Compiler.CompilerContext);
		Compiler.bIsErrorFree &= FBaseAsyncTaskHelper::CopyEventSignature(CurrentCENode, AddDelegateNode->GetDelegateSignature(), Schema);
	}

	UEdGraphPin* LastActivatedNodeThen = CurrentCENode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	for (const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& OutputPair : VariableOutputs) // CREATE CHAIN OF ASSIGMENTS
	{
		UEdGraphPin* PinWithData = CurrentCENode->FindPin(OutputPair.OutputPin->PinName);
		if (PinWithData == nullptr)
		{
			/*FText ErrorMessage = FText::Format(LOCTEXT("MissingDataPin", "ICE: Pin @@ was expecting a data output pin named {0} on @@ (each delegate must have the same signature)"), FText::FromString(OutputPair.OutputPin->PinName));
			CompilerContext.MessageLog.Error(*ErrorMessage.ToString(), OutputPair.OutputPin, CurrentCENode);
			return false;*/
			continue;
		}

		auto AssignNode = Compiler.SpawnIntermediateNode<UK2Node_AssignmentStatement>();
		Compiler.bIsErrorFree &= Schema->TryCreateConnection(LastActivatedNodeThen, AssignNode->GetExecPin());
		Compiler.bIsErrorFree &= Schema->TryCreateConnection(OutputPair.TempVar->GetVariablePin(), AssignNode->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetVariablePin());
		Compiler.bIsErrorFree &= Schema->TryCreateConnection(AssignNode->GetValuePin(), PinWithData);
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetValuePin());

		LastActivatedNodeThen = AssignNode->GetThenPin();
	}

	/*
	auto CallExitStateNode = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(ProxyObjectPin, ProxyClass, GET_FUNCTION_NAME_CHECKED(IStateInterface, ExitState));

	bIsErrorFree &= Schema->TryCreateConnection(LastActivatedNodeThen, CallExitStateNode->GetExecPin());
	LastActivatedNodeThen = CallExitStateNode->GetThenPin();
	*/

	Compiler.ConnectPins(PinForCurrentDelegateProperty, LastActivatedNodeThen);
}

UK2Node_AsyncTask::UK2Node_AsyncTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAsyncTaskStatics, CreateAsyncTask);
	ProxyFactoryClass = UUnrealEngineExStatics::StaticClass();
}

UEdGraphPin* UK2Node_AsyncTask::GetAsyncTaskClassPin()
{
	static FName NAME_AsyncTaskClassPin(TEXT("AsyncTaskClass"));
	return FindPin(NAME_AsyncTaskClassPin);
}

FText UK2Node_AsyncTask::GetTooltipText() const
{
	return LOCTEXT("K2Node_AsyncTask_Tooltip", "Launches async task.");
}

FText UK2Node_AsyncTask::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return IsValid(AsyncTaskClass)
		? FText::Format(LOCTEXT("K2Node_AsyncTask_Title", "AsyncTask {0}"), FText::FromString(AsyncTaskClass->GetName()))
		: LOCTEXT("K2Node_AsyncTask_TitleUndefined", "AsyncTask <undefined>");
}

FText UK2Node_AsyncTask::GetMenuCategory() const
{
	return LOCTEXT("K2Node_AsyncTask_MenuCategory", "AsyncTask");
}

UObject* UK2Node_AsyncTask::GetJumpTargetForDoubleClick() const
{
	return IsValid(AsyncTaskClass) ? AsyncTaskClass->ClassGeneratedBy : nullptr;
}

void UK2Node_AsyncTask::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	for (TFieldIterator<FProperty> PropertyIt(ProxyClass); PropertyIt; ++PropertyIt)
	{
		if (auto AsMulticastDelegateProperty = Valid<FMulticastDelegateProperty>(*PropertyIt))
		{
			FK2NodeHelpers::CreateOutputPins(this, AsMulticastDelegateProperty);
		}
	}

	bool bAllPinsGood = true;
	UFunction* Function = ProxyFactoryClass ? ProxyFactoryClass->FindFunctionByName(ProxyFactoryFunctionName) : nullptr;
	if (Function)
	{
		bAllPinsGood = FK2NodeHelpers::CreateInputPins(this, Function);
	}

	UK2Node::AllocateDefaultPins();

	if (UEdGraphPin* AsyncTaskClassPin = GetAsyncTaskClassPin())
	{
		AsyncTaskClassPin->bHidden = true;
		AsyncTaskClassPin->DefaultObject = AsyncTaskClass;
	}

	FK2NodeHelpers::CreateOutputPins(this, AsyncTaskClass);
}

void UK2Node_AsyncTask::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	UK2Node::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph, GetExecPin());

	auto CallCreateAsyncTask = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(ProxyFactoryClass, ProxyFactoryFunctionName);
	if (CallCreateAsyncTask->GetTargetFunction() == nullptr)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 20 || ENGINE_MAJOR_VERSION >= 5
		const FText ClassName = ProxyFactoryClass ? FText::FromString(ProxyFactoryClass->GetName()) : LOCTEXT("MissingClassString", "Unknown Class");
		const FString FormattedMessage = FText::Format(
			LOCTEXT("AsyncTaskErrorFmt", "AsyncTask: Missing function {0} from class {1} for async task @@"),
			FText::FromString(ProxyFactoryFunctionName.GetPlainNameString()),
			ClassName
		).ToString();
#else
		const FString ClassName = ProxyFactoryClass ? ProxyFactoryClass->GetName() : LOCTEXT("MissingClassString", "Unknown Class").ToString();
		const FString RawMessage = LOCTEXT("AsyncTaskError", "State: Missing function %s from class %s for async task @@").ToString();
		const FString FormattedMessage = FString::Printf(*RawMessage, *ProxyFactoryFunctionName.GetPlainNameString(), *ClassName);
#endif

		CompilerContext.MessageLog.Error(*FormattedMessage, this);
		return;
	}

	for (auto CurrentPin : Pins)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19 || ENGINE_MAJOR_VERSION >= 5
		if (FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Input))
#else
		if (FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Input, Schema))
#endif
		{
			Compiler.ConnectPins(CurrentPin, CallCreateAsyncTask->FindPin(CurrentPin->PinName), true);
		}
	}

	auto AsyncTaskClassPin = CallCreateAsyncTask->FindPin(TEXT("AsyncTaskClass"));
	AsyncTaskClassPin->DefaultObject = AsyncTaskClass;

	UEdGraphPin* AsyncTaskPin = CallCreateAsyncTask->GetReturnValuePin();
	UEdGraphPin* OutputAsyncTaskProxy = FindPin(FBaseAsyncTaskHelper::GetAsyncTaskProxyName());
	check(AsyncTaskPin);

	Compiler.ConnectPins(OutputAsyncTaskProxy, AsyncTaskPin, true);

	// GATHER OUTPUT PARAMETERS AND PAIR THEM WITH LOCAL VARIABLES
	TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable> VariableOutputs;
	for (auto CurrentPin : Pins)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19 || ENGINE_MAJOR_VERSION >= 5
		if ((OutputAsyncTaskProxy != CurrentPin) && FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Output))
#else
		if ((OutputAsyncTaskProxy != CurrentPin) && FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Output, Schema))
#endif
		{
			auto TempVarOutput = Compiler.SpawnInternalVariable(CurrentPin->PinType);
			Compiler.ConnectPins(CurrentPin, TempVarOutput->GetVariablePin());
			VariableOutputs.Add(FBaseAsyncTaskHelper::FOutputPinAndLocalVariable(CurrentPin, TempVarOutput));
		}
	}

	auto ValidateProxyNode = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(
		Compiler.SpawnIsValidNode(AsyncTaskPin)->GetReturnValuePin());

	auto UpCastNode = Compiler.SpawnIntermediateNode<UK2Node_DynamicCast>(AsyncTaskClass, AsyncTaskPin);
	AsyncTaskPin = UpCastNode->GetCastResultPin();

	for (auto SpawnVarPin : Pins)
	{
		if (SpawnVarPin->LinkedTo.Num() == 0)
			continue;

		Compiler.ConnectSetVariable(SpawnVarPin, AsyncTaskPin);
	}

	for (TFieldIterator<FMulticastDelegateProperty> PropertyIt(AsyncTaskClass, EFieldIteratorFlags::IncludeSuper); PropertyIt && Compiler.bIsErrorFree; ++PropertyIt)
	{
		FAsyncTaskHelper::HandleDelegateImplementation(*PropertyIt, VariableOutputs, AsyncTaskPin, AsyncTaskClass, Compiler);
	}

	auto CallAsyncTaskRun = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(AsyncTaskPin, AsyncTaskClass, GET_FUNCTION_NAME_CHECKED(UAsyncTask, Run));

	Compiler.bIsErrorFree &= CompilerContext.CopyPinLinksToIntermediate(*Compiler.LastThenPin, *ValidateProxyNode->GetElsePin()).CanSafeConnect();
}

#if WITH_EDITOR
void UK2Node_AsyncTask::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

#undef LOCTEXT_NAMESPACE
