#include "UnrealEngineEx.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/EngineVersionComparison.h"
#include "Core.h"
#include "LatentActions.h"

#include "UnrealEngineExStatics.h"
#include "UnrealEngineEx.final.h"


#define LOCTEXT_NAMESPACE "FUnrealEngineExModule"



void FUnrealEngineExModule::StartupModule()
{
}

void FUnrealEngineExModule::ShutdownModule()
{
}


UObject* FUnrealEngineEx::GetOwnerOfType(const UObject* Object, const UClass* Class, const UObject* Unused)
{
	return nullptr;
}

UObject* FUnrealEngineEx::GetAssociatedObject(const UObject* Object, AGameModeBase* Unused)
{
	return UGameplayStatics::GetGameMode(Object);
}

UObject* FUnrealEngineEx::GetAssociatedObject(const UObject* Object, AGameStateBase* Unused)
{
	return UGameplayStatics::GetGameState(Object);
}

UObject* FUnrealEngineEx::GetAssociatedObject(const UObject* Object, AHUD* Unused)
{
	return UUnrealEngineExStatics::GetPlayerHUD(Object);
}

UObject* FUnrealEngineEx::GetAssociatedObject(const UObject* Object, APlayerController* Unused)
{
	return Cast<APlayerController>(UUnrealEngineExStatics::GetController(Object));
}

UObject* FUnrealEngineEx::GetAssociatedObject(const UObject* Object, APlayerState* Unused)
{
	return UUnrealEngineExStatics::GetPlayerState(Object);
}

UObject* FUnrealEngineEx::GetAssociatedObject(const UObject* Object, APawn* Unused)
{
	return UUnrealEngineExStatics::GetPlayerPawn(Object);
}


#if WITH_EDITOR
TSoftObjectPtr<UWorld> FUnrealEngineEx::ConvertLevelPtrToPIE(const TSoftObjectPtr<UWorld>& Level, UWorld* World)
{
#if !UE_VERSION_OLDER_THAN(5, 0, 0)
	int32 PIEInstanceID = World->GetOutermost()->GetPIEInstanceID();
#else
	int32 PIEInstanceID = World->GetOutermost()->PIEInstanceID;
#endif
	FString PIELevelPackageName;
	FString PIELevelObjectName;

	if (PIEInstanceID < 0) // For Standalone runs.
		return Level;

	PIELevelPackageName = UWorld::ConvertToPIEPackageName(FPackageName::ObjectPathToPackageName(Level.ToString()), PIEInstanceID);
	PIELevelObjectName = FPackageName::GetLongPackageAssetName(PIELevelPackageName);
	TSoftObjectPtr<UWorld> NewWorld;
	NewWorld = PIELevelPackageName + TEXT(".") + PIELevelObjectName;
	return NewWorld;
}

TSoftObjectPtr<UWorld> FUnrealEngineEx::ConvertLevelPtrFromPIE(const TSoftObjectPtr<UWorld>& Level, UWorld* World)
{
#if !UE_VERSION_OLDER_THAN(5, 0, 0)
	int32 PIEInstanceID = World->GetOutermost()->GetPIEInstanceID();
#else
	int32 PIEInstanceID = World->GetOutermost()->PIEInstanceID;
#endif

	FString PIELevelPackageName = UWorld::StripPIEPrefixFromPackageName(FPackageName::ObjectPathToPackageName(Level.ToString()), World->StreamingLevelsPrefix);
	FString PIELevelObjectName = UWorld::RemovePIEPrefix(FPackageName::GetLongPackageAssetName(PIELevelPackageName));

	if (PIEInstanceID < 0) // For Standalone runs.
		PIELevelPackageName.RemoveFromStart(TEXT("/Temp/Autosaves"));

	TSoftObjectPtr<UWorld> NewWorld;
	NewWorld = PIELevelPackageName + TEXT(".") + PIELevelObjectName;
	return NewWorld;
}
#endif

TSoftObjectPtr<UWorld> FUnrealEngineEx::GetLevelPtr(UWorld* World)
{
	const FString PackageName = World->GetOutermost()->GetName();

	return GetLevelPtr(PackageName);
}

TSoftObjectPtr<UWorld> FUnrealEngineEx::GetLevelPtr(FName PackageName)
{
	FString LevelObjectName = FPackageName::GetLongPackageAssetName(PackageName.ToString());
	TSoftObjectPtr<UWorld> NewWorld;
	NewWorld = PackageName.ToString() + TEXT(".") + LevelObjectName;
	return NewWorld;
}

TSoftObjectPtr<UWorld> FUnrealEngineEx::GetLevelPtr(FString PackageName)
{
	FString LevelObjectName = FPackageName::GetLongPackageAssetName(PackageName);
	TSoftObjectPtr<UWorld> NewWorld;
	NewWorld = PackageName + TEXT(".") + LevelObjectName;
	return NewWorld;
}

FString FUnrealEngineEx::GetLevelName(const TSoftObjectPtr<UWorld>& Level)
{
	//FPackageName::SearchForPackageOnDisk(LevelName.ToString() + FPackageName::GetMapPackageExtension(), &MapFullName)
	return FPackageName::ObjectPathToObjectName(Level.ToSoftObjectPath().ToString());
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
