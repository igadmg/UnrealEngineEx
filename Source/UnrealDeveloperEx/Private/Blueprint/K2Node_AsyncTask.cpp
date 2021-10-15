#include "Blueprint/K2Node_AsyncTask.h"

#include "Blueprint/K2NodeHelpers.h"
#include "AsyncTask.h"
#include "EdGraphSchema_K2.h"
#include "UnrealEngineExStatics.h"

#include "KismetCompiler.h"
#include "K2Node_AssignmentStatement.h"

#include "CoreEx.h"

#define LOCTEXT_NAMESPACE "UnrealDeveloperEx"


bool UK2Node_AsyncTask::FAsyncTaskHelper::HandleDelegateImplementation(
	FMulticastDelegateProperty* CurrentProperty, const TArray<FAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs,
	UEdGraphPin* ProxyObjectPin, UClass* ProxyClass, UEdGraphPin*& InOutLastThenPin,
	FK2NodeCompilerHelper& Compiler)
{
	bool bIsErrorFree = true;
	const UEdGraphSchema_K2* Schema = Compiler.CompilerContext.GetSchema();
	check(CurrentProperty && ProxyObjectPin && InOutLastThenPin && Schema);

	UEdGraphPin* PinForCurrentDelegateProperty = Compiler.Node->FindPin(CurrentProperty->GetFName());
	if (!PinForCurrentDelegateProperty || (UEdGraphSchema_K2::PC_Exec != PinForCurrentDelegateProperty->PinType.PinCategory))
	{
		//FText ErrorMessage = FText::Format(LOCTEXT("WrongDelegateProperty", "BaseAsyncTask: Cannot find execution pin for delegate "), FText::FromString(CurrentProperty->GetName()));
		//CompilerContext.MessageLog.Error(*ErrorMessage.ToString(), CurrentNode);
		return true;
	}

	UK2Node_CustomEvent* CurrentCENode = Compiler.SpawnIntermediateEventNode<UK2Node_CustomEvent>(PinForCurrentDelegateProperty
		, FName(*FString::Printf(TEXT("%s_%s"), *CurrentProperty->GetName(), *Compiler.CompilerContext.GetGuid(Compiler.Node))));
	{
		UK2Node_AddDelegate* AddDelegateNode = Compiler.SpawnIntermediateNode<UK2Node_AddDelegate>(CurrentProperty, ProxyObjectPin);
		bIsErrorFree &= Schema->TryCreateConnection(InOutLastThenPin, AddDelegateNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute));
		InOutLastThenPin = AddDelegateNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

		bIsErrorFree &= FBaseAsyncTaskHelper::CreateDelegateForNewFunction(AddDelegateNode->GetDelegatePin(), CurrentCENode->GetFunctionName(), Cast<UK2Node>(Compiler.Node), Compiler.SourceGraph, Compiler.CompilerContext);
		bIsErrorFree &= FBaseAsyncTaskHelper::CopyEventSignature(CurrentCENode, AddDelegateNode->GetDelegateSignature(), Schema);
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
		bIsErrorFree &= Schema->TryCreateConnection(LastActivatedNodeThen, AssignNode->GetExecPin());
		bIsErrorFree &= Schema->TryCreateConnection(OutputPair.TempVar->GetVariablePin(), AssignNode->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetVariablePin());
		bIsErrorFree &= Schema->TryCreateConnection(AssignNode->GetValuePin(), PinWithData);
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetValuePin());

		LastActivatedNodeThen = AssignNode->GetThenPin();
	}

	/*
	auto CallExitStateNode = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(ProxyClass, GET_FUNCTION_NAME_CHECKED(IStateInterface, ExitState));

	UEdGraphPin* SelfInputPin = CallExitStateNode->FindPinChecked(TEXT("self"));
	bIsErrorFree &= Schema->TryCreateConnection(ProxyObjectPin, SelfInputPin);

	bIsErrorFree &= Schema->TryCreateConnection(LastActivatedNodeThen, CallExitStateNode->GetExecPin());
	LastActivatedNodeThen = CallExitStateNode->GetThenPin();
	*/

	bIsErrorFree &= Compiler.MovePinLinksToIntermediate(PinForCurrentDelegateProperty, LastActivatedNodeThen);
	return bIsErrorFree;
}

UK2Node_AsyncTask::UK2Node_AsyncTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UUnrealEngineExStatics, CreateAsyncTask);
	ProxyFactoryClass = UUnrealEngineExStatics::StaticClass();
}

UEdGraphPin* UK2Node_AsyncTask::GetAsyncTaskClassPin()
{
	static FName NAME_AsyncTaskClassPin(TEXT("AsyncTaskClass"));
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->GetFName() == NAME_AsyncTaskClassPin)
		{
			return Pin;
		}
	}

	return nullptr;
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
	auto K2Schema = GetDefault<UEdGraphSchema_K2>();

	CreatePin(EGPD_Input, K2Schema->PC_Exec, FName(), nullptr, K2Schema->PN_Execute);

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

	UEdGraphPin* AsyncTaskClassPin = GetAsyncTaskClassPin();
	check(AsyncTaskClassPin);

	AsyncTaskClassPin->bHidden = true;
	AsyncTaskClassPin->DefaultObject = AsyncTaskClass;

	FK2NodeHelpers::CreateOutputPins(this, AsyncTaskClass);
}

void UK2Node_AsyncTask::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	auto Schema = CompilerContext.GetSchema();
	bool bIsErrorFree = true;

	UK2Node::ExpandNode(CompilerContext, SourceGraph);

	FK2NodeCompilerHelper Compiler(this, CompilerContext, SourceGraph);

	auto CallCreateAsyncTask = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(ProxyFactoryClass, ProxyFactoryFunctionName);
	if (CallCreateAsyncTask->GetTargetFunction() == nullptr)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 20
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

	bIsErrorFree &= Compiler.MovePinLinksToIntermediate(this, Schema->PN_Execute, CallCreateAsyncTask, Schema->PN_Execute);

	for (auto CurrentPin : Pins)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19
		if (FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Input))
#else
		if (FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Input, Schema))
#endif
		{
			bIsErrorFree &= Compiler.MovePinLinksToIntermediate(CurrentPin, CallCreateAsyncTask->FindPin(CurrentPin->PinName), true);
		}
	}

	auto AsyncTaskClassPin = CallCreateAsyncTask->FindPin(TEXT("AsyncTaskClass"));
	AsyncTaskClassPin->DefaultObject = AsyncTaskClass;

	UEdGraphPin* AsyncTaskPin = CallCreateAsyncTask->GetReturnValuePin();
	UEdGraphPin* OutputAsyncTaskProxy = FindPin(FBaseAsyncTaskHelper::GetAsyncTaskProxyName());
	check(AsyncTaskPin);

	bIsErrorFree &= Compiler.MovePinLinksToIntermediate(OutputAsyncTaskProxy, AsyncTaskPin, true);

	// GATHER OUTPUT PARAMETERS AND PAIR THEM WITH LOCAL VARIABLES
	TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable> VariableOutputs;
	for (auto CurrentPin : Pins)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19
		if ((OutputAsyncTaskProxy != CurrentPin) && FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Output))
#else
		if ((OutputAsyncTaskProxy != CurrentPin) && FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Output, Schema))
#endif
		{
			auto TempVarOutput = Compiler.SpawnInternalVariable(CurrentPin->PinType);
			bIsErrorFree &= Compiler.MovePinLinksToIntermediate(CurrentPin, TempVarOutput->GetVariablePin());
			VariableOutputs.Add(FBaseAsyncTaskHelper::FOutputPinAndLocalVariable(CurrentPin, TempVarOutput));
		}
	}

	UEdGraphPin* LastThenPin = CallCreateAsyncTask->FindPinChecked(Schema->PN_Then);

	auto ValidateProxyNode = Compiler.SpawnIntermediateNode<UK2Node_IfThenElse>(
		Compiler.SpawnIsValidNode(AsyncTaskPin)->GetReturnValuePin());

	bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, ValidateProxyNode->GetExecPin());
	LastThenPin = ValidateProxyNode->GetThenPin();

	auto UpCastNode = Compiler.SpawnIntermediateNode<UK2Node_DynamicCast>(AsyncTaskClass, AsyncTaskPin);
	AsyncTaskPin = UpCastNode->GetCastResultPin();

	bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, UpCastNode->GetExecPin());
	LastThenPin = UpCastNode->GetValidCastPin();

	// Create 'set var by name' nodes and hook them up
	for (auto SpawnVarPin : Pins)
	{
		// Only create 'set param by name' node if this pin is linked to something
		if (SpawnVarPin->LinkedTo.Num() > 0)
		{
			bIsErrorFree &= Compiler.CreateSetParamByNameNodes(AsyncTaskPin, SpawnVarPin, LastThenPin);
		}
	}

	for (TFieldIterator<FMulticastDelegateProperty> PropertyIt(AsyncTaskClass, EFieldIteratorFlags::IncludeSuper); PropertyIt && bIsErrorFree; ++PropertyIt)
	{
		bIsErrorFree &= FAsyncTaskHelper::HandleDelegateImplementation(*PropertyIt, VariableOutputs, AsyncTaskPin, AsyncTaskClass, LastThenPin, Compiler);
	}

	auto CallAsyncTaskRun = Compiler.SpawnIntermediateNode<UK2Node_CallFunction>(AsyncTaskPin, AsyncTaskClass, GET_FUNCTION_NAME_CHECKED(UAsyncTask, Run));
	bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, CallAsyncTaskRun->GetExecPin());
	LastThenPin = CallAsyncTaskRun->GetThenPin();

	bIsErrorFree &= CompilerContext.CopyPinLinksToIntermediate(*LastThenPin, *ValidateProxyNode->GetElsePin()).CanSafeConnect();

	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "BaseAsyncTask: Internal connection error. @@").ToString(), this);
	}

	BreakAllNodeLinks();
}

#if WITH_EDITOR
void UK2Node_AsyncTask::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

#undef LOCTEXT_NAMESPACE
