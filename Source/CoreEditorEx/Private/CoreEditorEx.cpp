#include "CoreEditorEx.h"

#include "Interfaces/CookInterface.h"
#include "UObject/ObjectSaveContext.h"
#include "Core.h"
#include "Editor.h"

#include "CoreEditorEx.final.h"


void FCoreEditorExModule::StartupModule()
{
	OnMapOpenedHandle = FEditorDelegates::OnMapOpened.AddLambda([this](const FString& Filename, bool bAsTemplate) {
		for (TObjectIterator<UObject> It; It; ++It)
		{
			if (auto Object = *It)
			{
				if (Object->GetWorld() != GWorld)
					continue;

				if (auto CookInterface = Valid<ICookInterface>(Object))
				{
					if (CookInterface->OnWorldLoaded())
						Object->Modify();
				}
			}
		}
	});

	PreSaveWorldWithContextHandle = FEditorDelegates::PreSaveWorldWithContext.AddLambda([this](UWorld* World, FObjectPreSaveContext Context) {
		TArray<ICookInterface*> ObjectsToSave;
		for (TObjectIterator<UObject> It; It; ++It)
		{
			if (auto Object = *It)
			{
				if (Object->GetWorld() != World)
					continue;

				if (auto CookInterface = Valid<ICookInterface>(Object))
				{
					ObjectsToSave.Add(CookInterface);
				}
			}
		}

		for (auto CookInterface : ObjectsToSave)
		{
			if (!IsValid(CookInterface))
				return;

			if (CookInterface->OnWorldSaved())
				Cast<UObject>(CookInterface)->Modify();

		}
	});
}

void FCoreEditorExModule::ShutdownModule()
{
	FEditorDelegates::PreSaveWorldWithContext.Remove(PreSaveWorldWithContextHandle);
	FEditorDelegates::OnMapOpened.Remove(OnMapOpenedHandle);
}


IMPLEMENT_MODULE(FCoreEditorExModule, CoreEditorEx)

DEFINE_LOG_CATEGORY(LogCoreEditorEx);
//DEFINE_LOG_CATEGORY(LogCoreEditorExInit);
//DEFINE_LOG_CATEGORY(LogCoreEditorExCriticalErrors);


