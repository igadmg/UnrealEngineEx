#include "CoreEditorEx.h"

#include "Interfaces/CookInterface.h"
#include "UObject/ObjectSaveContext.h"
#include "UObject/UObjectIterator.h"
#include "Core.h"
#include "Editor.h"

#include "CoreEditorEx.final.h"


void FCoreEditorExModule::StartupModule()
{
	PreBeginPIEHandle = FEditorDelegates::PreBeginPIE.AddLambda([this](bool bIsSimulating) {
		if (bIsSimulating)
			return;

		TArray<ICookInterface*> ObjectsToSave;
		FCoreEditorEx::GatherCookInterfaceObjects(GWorld, ObjectsToSave);

		for (auto CookInterface : ObjectsToSave)
		{
			if (CookInterface->OnWorldSaved())
				Cast<UObject>(CookInterface)->Modify();
		}
	});

	OnMapOpenedHandle = FEditorDelegates::OnMapOpened.AddLambda([this](const FString& Filename, bool bAsTemplate) {
		TArray<ICookInterface*> ObjectsToSave;
		FCoreEditorEx::GatherCookInterfaceObjects(GWorld, ObjectsToSave);

		for (auto CookInterface : ObjectsToSave)
		{
			if (CookInterface->OnWorldLoaded())
				Cast<UObject>(CookInterface)->Modify();
		}
	});

	PreSaveWorldWithContextHandle = FEditorDelegates::PreSaveWorldWithContext.AddLambda([this](UWorld* World, FObjectPreSaveContext Context) {
		TArray<ICookInterface*> ObjectsToSave;
		FCoreEditorEx::GatherCookInterfaceObjects(World, ObjectsToSave);

		for (auto CookInterface : ObjectsToSave)
		{
			if (CookInterface->OnWorldSaved())
				Cast<UObject>(CookInterface)->Modify();
		}
	});
}

void FCoreEditorExModule::ShutdownModule()
{
	FEditorDelegates::PreBeginPIE.Remove(PreBeginPIEHandle);
	FEditorDelegates::PreSaveWorldWithContext.Remove(PreSaveWorldWithContextHandle);
	FEditorDelegates::OnMapOpened.Remove(OnMapOpenedHandle);
}


IMPLEMENT_MODULE(FCoreEditorExModule, CoreEditorEx)

DEFINE_LOG_CATEGORY(LogCoreEditorEx);
//DEFINE_LOG_CATEGORY(LogCoreEditorExInit);
//DEFINE_LOG_CATEGORY(LogCoreEditorExCriticalErrors);


bool FCoreEditorEx::GatherCookInterfaceObjects(const UWorld* World, TArray<ICookInterface*>& Objects)
{
	for (TObjectIterator<UObject> It; It; ++It)
	{
		if (auto Object = *It)
		{
			if (auto CookInterface = Valid<ICookInterface>(Object))
			{
				if (Object->GetWorld() != World)
					continue;

				Objects.Add(CookInterface);
			}
		}
	}

	return true;
}
