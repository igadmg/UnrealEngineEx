#include "UnrealEditorExStatics.h"

#include "Engine/Selection.h"
#include "Components/ActorComponent.h"
#include "Settings/LevelEditorPlaySettings.h"
#include "UObject/LinkerLoad.h"
#include "UObject/Package.h"
#include "Editor.h"



void UUnrealEditorExStatics::SelectActorComponent(UActorComponent* InActorComponent)
{
	GEditor->GetSelectedComponents()->DeselectAll();
	if (IsValid(InActorComponent))
	{
		GEditor->SelectComponent(InActorComponent, true, true, true);
	}
}

UObject* UClass_CreateDefaultObject(UClass* Class)
{
	UObject* ClassDefaultObject{};
	//ensureMsgf(!bLayoutChanging, TEXT("Class named %s creating its CDO while changing its layout"), *GetName());

	UClass* ParentClass = Class->GetSuperClass();
	UObject* ParentDefaultObject = NULL;
	if (ParentClass != NULL)
	{
		UObjectForceRegistration(ParentClass);
		ParentDefaultObject = ParentClass->GetDefaultObject(); // Force the default object to be constructed if it isn't already
		check(GConfig);
		if (GEventDrivenLoaderEnabled && EVENT_DRIVEN_ASYNC_LOAD_ACTIVE_AT_RUNTIME)
		{
			check(ParentDefaultObject && !ParentDefaultObject->HasAnyFlags(RF_NeedLoad));
		}
	}

	if ((ParentDefaultObject != NULL) || (Class == UObject::StaticClass()))
	{
		// If this is a class that can be regenerated, it is potentially not completely loaded.  Preload and Link here to ensure we properly zero memory and read in properties for the CDO
		if (Class->HasAnyClassFlags(CLASS_CompiledFromBlueprint) && (Class->PropertyLink == NULL) && !GIsDuplicatingClassForReinstancing)
		{
			auto ClassLinker = Class->GetLinker();
			if (ClassLinker)
			{
				if (!GEventDrivenLoaderEnabled)
				{
					UField* FieldIt = Class->Children;
					while (FieldIt && (FieldIt->GetOuter() == Class))
					{
						// If we've had cyclic dependencies between classes here, we might need to preload to ensure that we load the rest of the property chain
						if (FieldIt->HasAnyFlags(RF_NeedLoad))
						{
							ClassLinker->Preload(FieldIt);
						}
						FieldIt = FieldIt->Next;
					}
				}

				Class->StaticLink(true);
			}
		}

		// in the case of cyclic dependencies, the above Preload() calls could end up 
		// invoking this method themselves... that means that once we're done with  
		// all the Preload() calls we have to make sure ClassDefaultObject is still 
		// NULL (so we don't invalidate one that has already been setup)
		if (ClassDefaultObject == NULL)
		{
			// RF_ArchetypeObject flag is often redundant to RF_ClassDefaultObject, but we need to tag
			// the CDO as RF_ArchetypeObject in order to propagate that flag to any default sub objects.
			ClassDefaultObject = StaticAllocateObject(Class, Class->GetOuter(), NAME_None, EObjectFlags(RF_Public | RF_ClassDefaultObject | RF_ArchetypeObject));
			check(ClassDefaultObject);
			// Register the offsets of any sparse delegates this class introduces with the sparse delegate storage
			for (TFieldIterator<FMulticastSparseDelegateProperty> SparseDelegateIt(Class, EFieldIteratorFlags::ExcludeSuper, EFieldIteratorFlags::ExcludeDeprecated); SparseDelegateIt; ++SparseDelegateIt)
			{
				const FSparseDelegate& SparseDelegate = SparseDelegateIt->GetPropertyValue_InContainer(ClassDefaultObject);
				USparseDelegateFunction* SparseDelegateFunction = CastChecked<USparseDelegateFunction>(SparseDelegateIt->SignatureFunction);
				FSparseDelegateStorage::RegisterDelegateOffset(ClassDefaultObject, SparseDelegateFunction->DelegateName, (size_t)&SparseDelegate - (size_t)ClassDefaultObject);
			}
			EObjectInitializerOptions InitOptions = EObjectInitializerOptions::None;
			if (!Class->HasAnyClassFlags(CLASS_Native | CLASS_Intrinsic))
			{
				// Blueprint CDOs have their properties always initialized.
				InitOptions |= EObjectInitializerOptions::InitializeProperties;
			}
			(*Class->ClassConstructor)(FObjectInitializer(ClassDefaultObject, ParentDefaultObject, InitOptions));
			if (Class->GetOutermost()->HasAnyPackageFlags(PKG_CompiledIn) && !Class->GetOutermost()->HasAnyPackageFlags(PKG_RuntimeGenerated))
			{
				TCHAR PackageName[FName::StringBufferSize];
				TCHAR CDOName[FName::StringBufferSize];
				Class->GetOutermost()->GetFName().ToString(PackageName);
				Class->GetDefaultObjectName().ToString(CDOName);
				//Class->NotifyRegistrationEvent(PackageName, CDOName, ENotifyRegistrationType::NRT_ClassCDO, ENotifyRegistrationPhase::NRP_Finished, nullptr, false, ClassDefaultObject);
			}
			ClassDefaultObject->PostCDOContruct();
		}
	}

	return ClassDefaultObject;
}

void UUnrealEditorExStatics::FixDefaultObject(UClass* Class)
{
	// DOES IT HELP or BP DUPLICATION HELPED?
	auto NewDefualtSubobject = UClass_CreateDefaultObject(Class);
	Class->ClassDefaultObject = NewDefualtSubobject;
}
