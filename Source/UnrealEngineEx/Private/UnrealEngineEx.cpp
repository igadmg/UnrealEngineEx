#include "UnrealEngineExPrivatePCH.h"
#include "Core.h"
#include "ModuleManager.h"



#define LOCTEXT_NAMESPACE "FUnrealEngineExModule"



void FUnrealEngineExModule::StartupModule()
{
}

void FUnrealEngineExModule::ShutdownModule()
{
}



#if WITH_EDITOR
TAssetPtr<UWorld> FUnrealEngineEx::ConvertLevelPtrToPIE(const TAssetPtr<UWorld>& Level, UWorld* World)
{
	int32 PIEInstanceID = World->GetOutermost()->PIEInstanceID;
	FString PIELevelPackageName;
	FString PIELevelObjectName;

	if (PIEInstanceID < 0) // For Standalone runs.
		return Level;

	PIELevelPackageName = UWorld::ConvertToPIEPackageName(FPackageName::ObjectPathToPackageName(Level.ToString()), PIEInstanceID);
	PIELevelObjectName = FPackageName::GetLongPackageAssetName(PIELevelPackageName);
	TAssetPtr<UWorld> NewWorld;
	NewWorld = PIELevelPackageName + TEXT(".") + PIELevelObjectName;
	return NewWorld;
}

TAssetPtr<UWorld> FUnrealEngineEx::ConvertLevelPtrFromPIE(const TAssetPtr<UWorld>& Level, class UWorld* World)
{
	FString PIELevelPackageName = UWorld::StripPIEPrefixFromPackageName(FPackageName::ObjectPathToPackageName(Level.ToString()), World->StreamingLevelsPrefix);
	FString PIELevelObjectName = UWorld::RemovePIEPrefix(FPackageName::GetLongPackageAssetName(PIELevelPackageName));

	int32 PIEInstanceID = World->GetOutermost()->PIEInstanceID;
	if (PIEInstanceID < 0) // For Standalone runs.
		PIELevelPackageName.RemoveFromStart(TEXT("/Temp/Autosaves"));

	TAssetPtr<UWorld> NewWorld;
	NewWorld = PIELevelPackageName + TEXT(".") + PIELevelObjectName;
	return NewWorld;
}
#endif

TAssetPtr<UWorld> FUnrealEngineEx::GetLevelPtr(UWorld* World)
{
	const FString PackageName = World->GetOutermost()->GetName();

	return GetLevelPtr(PackageName);
}

TAssetPtr<UWorld> FUnrealEngineEx::GetLevelPtr(FName PackageName)
{
	FString LevelObjectName = FPackageName::GetLongPackageAssetName(PackageName.ToString());
	TAssetPtr<UWorld> NewWorld;
	NewWorld = PackageName.ToString() + TEXT(".") + LevelObjectName;
	return NewWorld;
}

TAssetPtr<UWorld> FUnrealEngineEx::GetLevelPtr(FString PackageName)
{
	FString LevelObjectName = FPackageName::GetLongPackageAssetName(PackageName);
	TAssetPtr<UWorld> NewWorld;
	NewWorld = PackageName + TEXT(".") + LevelObjectName;
	return NewWorld;
}

FString FUnrealEngineEx::GetLevelName(const TAssetPtr<UWorld>& Level)
{
	//FPackageName::SearchForPackageOnDisk(LevelName.ToString() + FPackageName::GetMapPackageExtension(), &MapFullName)
	return FPackageName::ObjectPathToObjectName(Level.ToStringReference().ToString());
}

void FUnrealEngineEx::SetActorEnabled(AActor* Actor, bool bIsEnabled)
{
	Actor->SetActorHiddenInGame(!bIsEnabled);
	Actor->SetActorEnableCollision(bIsEnabled);
	Actor->SetActorTickEnabled(bIsEnabled);
	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (MeshComponent != NULL)
	{
		if (bIsEnabled)
		{
			UNavigationSystem::OnComponentRegistered(MeshComponent);
		}
		else
		{
			UNavigationSystem::OnComponentUnregistered(MeshComponent);
		}
	}
}

void FUnrealEngineEx::FinishLatentAction(FLatentActionManager& LatentManager, const FLatentActionInfo& LatentInfo)
{
	class FFinishLatentAction : public FPendingLatentAction
	{
	public:
		FLatentActionInfo LatentInfo;

		FFinishLatentAction(const FLatentActionInfo& InLatentInfo)
			: LatentInfo(InLatentInfo)
		{
		}

		virtual void UpdateOperation(FLatentResponse& Response) override
		{
			Response.FinishAndTriggerIf(true, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
		}
	};



	if (LatentManager.FindExistingAction<FFinishLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
	{
		LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FFinishLatentAction(LatentInfo));
	}
}



#undef LOCTEXT_NAMESPACE



IMPLEMENT_MODULE(FUnrealEngineExModule, UnrealEngineEx)

DEFINE_LOG_CATEGORY(LogUnrealEngineEx);
//DEFINE_LOG_CATEGORY(LogUnrealEngineExInit);
//DEFINE_LOG_CATEGORY(LogUnrealEngineExCriticalErrors);
