#include "UnrealEngineExStatics.h"
#include "UnrealEngineExPrivatePCH.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Engine/CoreSettings.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LocalPlayer.h"
#include "Engine/NetConnection.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsyncTask.h"
#include "AsyncTaskManager.h"
#include "DebugDrawHelpersEx.h"
#include "EngineUtils.h"
#include "GameMapsSettings.h"
#include "LatentActions.h"
#include "UnrealEngineEx.h"

#if WITH_EDITOR
#include "Settings/LevelEditorPlaySettings.h"
#endif

#include "CoreEx.h"
#include "CoordinateFrame.h"


struct FFindStreamingLevelBy {
	const TSoftObjectPtr<UWorld>& LevelAssetPtr;
	FFindStreamingLevelBy(const TSoftObjectPtr<UWorld>& LevelAssetPtr) : LevelAssetPtr(LevelAssetPtr) {}
	bool operator ()(const ULevelStreaming* Level) const { return FUnrealEngineEx::ConvertLevelPtrFromPIE(Level->GetWorldAsset(), Level->GetWorld()) == LevelAssetPtr; }
};


TEnumAsByte<EWorldType::Type> UUnrealEngineExStatics::GetWorldType(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	return IsValid(World) ? (EWorldType::Type)World->WorldType : EWorldType::None;
}

void UUnrealEngineExStatics::WorldType(const UObject* WorldContextObject, TEnumAsByte<EWorldType::Type>& OutWorldType)
{
	OutWorldType = GetWorldType(WorldContextObject);
}

void UUnrealEngineExStatics::NetMode(const UObject* WorldContextObject, TEnumAsByte<ENetMode>& OutNetMode)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	OutNetMode = World ? World->GetNetMode() : NM_Standalone;
}


float UUnrealEngineExStatics::GetServerWorldTimeSeconds(const UObject* WorldContextObject)
{
	AGameStateBase* GameState = UGameplayStatics::GetGameState(WorldContextObject);
	return IsValid(GameState) ? GameState->GetServerWorldTimeSeconds() : 0.f;
}

bool UUnrealEngineExStatics::IsListenServer(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	return IsValid(World) ? (World->GetNetMode() == NM_ListenServer) : false;
}

UObject* UUnrealEngineExStatics::GetDefaultObject(TSubclassOf<UObject> ObjectClass)
{
	return ObjectClass.GetDefaultObject();
}

UGameInstance* UUnrealEngineExStatics::GetDefaultGameInstance()
{
	FSoftClassPath GameInstanceClassName = GetDefault<UGameMapsSettings>()->GameInstanceClass;
	UClass* GameInstanceClass = GameInstanceClassName.IsValid()
		? LoadObject<UClass>(NULL, *GameInstanceClassName.ToString())
		: UGameInstance::StaticClass();
	return GameInstanceClass->GetDefaultObject<UGameInstance>();
}

int32 UUnrealEngineExStatics::GetPlayerControllerIndex(APlayerController* PlayerController)
{
	UWorld* World = GEngine->GetWorldFromContextObject(PlayerController, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return INDEX_NONE;

	uint32 Index = 0;
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator, ++Index)
	{
		APlayerController* PC = Iterator->Get();
		if (PlayerController == PC)
		{
			return Index;
		}
	}

	return INDEX_NONE;
}

APlayerStart* UUnrealEngineExStatics::FindPlayerStartByTag(const UObject* WorldContextObject, FName StartTag)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* Start = *It;
		if (Start && Start->PlayerStartTag == StartTag)
		{
			return Start;
		}
	}

	return nullptr;
}

void UUnrealEngineExStatics::RestartPlayer(AController* PlayerController)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(PlayerController);

	if (!IsValid(GameMode))
		return;

	APawn* Pawn = PlayerController->GetPawn();
	PlayerController->UnPossess();
	if (Pawn != nullptr)
		Pawn->Destroy();

	//PlayerController->InitPlayerState();
	GameMode->RestartPlayer(PlayerController);
}

void UUnrealEngineExStatics::RestartPlayerByState(APlayerState* PlayerState)
{
	AController* Controller = Cast<AController>(PlayerState->GetOwner());

	if (IsValid(Controller))
	{
		RestartPlayer(Controller);
	}
}


APlayerController* UUnrealEngineExStatics::GetLocalPlayerController(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	return World->GetFirstPlayerController();
}

AHUD* UUnrealEngineExStatics::GetLocalPlayerHUD(const UObject* WorldContextObject)
{
	return GetPlayerHUD(GetLocalPlayerController(WorldContextObject));
}

template <typename TResult>
TResult* ForEachOwningActor(UObject* Object, TFunction<TResult* (AActor*)> Function)
{
	if (!IsValid(Object))
		return nullptr;

	if (auto AsActor = Cast<AActor>(Object))
	{
		if (auto Result = Function(AsActor))
			return Result;

		if (auto ParentActor = Valid(AsActor->GetParentActor()))
		{
			return ForEachOwningActor(ParentActor, Function);
		}

		if (auto ActorOwner = AsActor->GetOwner())
		{
			return ForEachOwningActor(ActorOwner, Function);
		}
	}
	else
	{
		if (auto AsActorComponent = Cast<UActorComponent>(Object))
		{
			return ForEachOwningActor(AsActorComponent->GetOwner(), Function);
		}

		if (auto AsUserWidget = Cast<UUserWidget>(Object))
		{
			return ForEachOwningActor(AsUserWidget->GetOwningPlayer(), Function);
		}
	}

	return ForEachOwningActor(Object->GetOuter(), Function);
}

AActor* UUnrealEngineExStatics::GetOwningActor(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UUnrealEngineExStatics::GetOwningActor"), STAT_UnrealEngineExStaticsGetOwningActor, STATGROUP_UnrealEngineEx);

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object), [](auto Actor) { return Actor; });
}

AActor* UUnrealEngineExStatics::GetOwningActorByClass(const UObject* Object, TSubclassOf<AActor> ActorClass)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UUnrealEngineExStatics::GetOwningActorByClass"), STAT_UnrealEngineExStaticsGetOwningActorByClass, STATGROUP_UnrealEngineEx);

	if (!IsValid(ActorClass))
		return nullptr;

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object)
		, [&ActorClass](auto Actor) {
			return Actor->IsA(ActorClass) ? Actor : nullptr;
		});
}

AActor* UUnrealEngineExStatics::GetControlledActor(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UUnrealEngineExStatics::GetControlledActor"), STAT_UnrealEngineExStaticsGetControlledActor, STATGROUP_UnrealEngineEx);

	if (!IsValid(Object))
		return nullptr;

	return ForEachOwningActor<AActor>(const_cast<UObject*>(Object)
		, [](auto Actor) -> AActor* {
		if (auto AsController = Cast<AController>(Actor))
		{
			return XX::GetPawnOrSpectator(AsController);
		}

		if (auto AsHud = Cast<AHUD>(Actor))
		{
			return XX::GetPawnOrSpectator(AsHud);
		}
		
		return Actor;
	});
}

AHUD* UUnrealEngineExStatics::GetPlayerHUD(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UUnrealEngineExStatics::GetPlayerHUD"), STAT_UnrealEngineExStaticsGetPlayerHUD, STATGROUP_UnrealEngineEx);

	if (auto AsHUD = Cast<AHUD>(GetOwningActorByClass(Object, AHUD::StaticClass())))
	{
		return AsHUD;
	}
	if (auto AsPlayerController = Cast<APlayerController>(GetController(Object)))
	{
		return AsPlayerController->GetHUD();
	}

	return nullptr;
}

APlayerState* UUnrealEngineExStatics::GetPlayerState(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UUnrealEngineExStatics::GetPlayerState"), STAT_UnrealEngineExStaticsGetPlayerState, STATGROUP_UnrealEngineEx);

	return ForEachOwningActor<APlayerState>(const_cast<UObject*>(Object)
		, [](auto Actor) -> APlayerState* {
			if (auto AsPlayerState = Cast<APlayerState>(Actor))
			{
				return AsPlayerState;
			}

			if (auto AsPawn = Cast<APawn>(Actor))
			{
				return AsPawn->GetPlayerState();
			}

			if (auto AsController = Cast<AController>(Actor))
			{
				return AsController->PlayerState;
			}

			return nullptr;
		});


	return GetPlayerState(Object->GetOuter());
}

float UUnrealEngineExStatics::GetPlayerScore(const UObject* Object)
{
	if (auto PlayerState = UUnrealEngineExStatics::GetPlayerState(Object))
	{
		return PlayerState->GetScore();
	}

	return 0;
}

APawn* UUnrealEngineExStatics::GetPawnOrSpectator(const UObject* Object)
{
	if (auto Pawn = GetPlayerPawn(Object))
	{
		return Pawn;
	}

	return GetSpectatorPawn(Object);
}

APawn* UUnrealEngineExStatics::GetPlayerPawn(const UObject* Object)
{
	if (auto AsPawn = Cast<APawn>(GetOwningActorByClass(Object, APawn::StaticClass())))
	{
		return AsPawn;
	}

	if (auto Controller = GetController(Object))
	{
		return Controller->GetPawn();
	}

	if (auto PlayerState = GetPlayerState(Object))
	{
		return PlayerState->GetPawn();
	}

	return nullptr;
}

ASpectatorPawn* UUnrealEngineExStatics::GetSpectatorPawn(const UObject* Object)
{
	if (auto AsSpectatorPawn = Cast<ASpectatorPawn>(GetOwningActorByClass(Object, ASpectatorPawn::StaticClass())))
	{
		return AsSpectatorPawn;
	}

	if (auto Controller = Cast<APlayerController>(GetController(Object)))
	{
		return Controller->GetSpectatorPawn();
	}

	return nullptr;
}

AController* UUnrealEngineExStatics::GetController(const UObject* Object)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UUnrealEngineExStatics::GetController"), STAT_UnrealEngineExStaticsGetController, STATGROUP_UnrealEngineEx);

	if (auto AsGameInstance = Cast<UGameInstance>(Object))
	{
		return GetLocalPlayerController(AsGameInstance);
	}

	return ForEachOwningActor<AController>(const_cast<UObject*>(Object)
		, [](auto Actor) -> AController* {
			if (auto AsController = Cast<AController>(Actor))
			{
				return const_cast<AController*>(AsController);
			}

			if (auto AsPawn = Cast<APawn>(Actor))
			{
				return AsPawn->GetController();
			}

			return nullptr;
		});
}

APlayerController* UUnrealEngineExStatics::GetPlayerController(const UObject* Object)
{
	return Valid<APlayerController>(GetController(Object));
}

APlayerCameraManager* UUnrealEngineExStatics::GetPlayerCameraManager(const UObject* Object)
{
	const APlayerController* Controller = Cast<APlayerController>(GetController(Object));
	if (!IsValid(Controller))
		return nullptr;

	return Controller->PlayerCameraManager;
}

UCameraComponent* UUnrealEngineExStatics::GetPlayerActiveCamera(const UObject* Object)
{
	const APlayerController* Controller = Cast<APlayerController>(GetController(Object));
	if (!IsValid(Controller))
		return nullptr;

	if (!IsValid(Controller->PlayerCameraManager))
		return nullptr;

	return nullptr;
}

UCharacterMovementComponent* UUnrealEngineExStatics::GetCharacterMovementComponent(const UObject* Object)
{
	if (!IsValid(Object))
		return nullptr;

	auto AsCharacter = Cast<ACharacter>(Object);
	if (AsCharacter)
	{
		return AsCharacter->GetCharacterMovement();
	}

	auto AsActorComponent = Cast<UActorComponent>(Object);
	if (AsActorComponent)
	{
		return GetCharacterMovementComponent(AsActorComponent->GetOwner());
	}

	return nullptr;
}


FViewSpaceDescriptor UUnrealEngineExStatics::GetViewSpaceDescriptor(ULocalPlayer* Player, FVector Locaton)
{
	UWorld* World = GEngine->GetWorldFromContextObject(Player, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return FViewSpaceDescriptor();

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		Player->ViewportClient->Viewport
		, World->Scene
		, Player->ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(true));

	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = Player->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, Player->ViewportClient->Viewport);
	if (SceneView == nullptr)
		return FViewSpaceDescriptor();

	return FViewSpaceDescriptor(SceneView, Locaton);
}

FViewFrustum UUnrealEngineExStatics::GetViewFrustum(class ULocalPlayer* Player)
{
	if (!IsValid(Player) || !IsValid(Player->ViewportClient) || Player->ViewportClient->Viewport == nullptr)
		return FViewFrustum();

	UWorld* World = GEngine->GetWorldFromContextObject(Player, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return FViewFrustum();

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		Player->ViewportClient->Viewport
		, World->Scene
		, Player->ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(true));

	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = Player->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, Player->ViewportClient->Viewport);
	if (SceneView != nullptr)
	{
		return FViewFrustum(SceneView->ViewFrustum);
	}

	return FViewFrustum();
}

TSoftObjectPtr<UWorld> UUnrealEngineExStatics::GetCurrentLevelAssetPtr(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	return FUnrealEngineEx::ConvertLevelPtrFromPIE(FUnrealEngineEx::GetLevelPtr(World), World);
}

FString UUnrealEngineExStatics::GetLevelName(const TSoftObjectPtr<UWorld>& Level)
{
	return FUnrealEngineEx::GetLevelName(Level);
}

AActor* UUnrealEngineExStatics::GetLevelScriptActor(const UObject* WorldContextObject, int32 LevelIndex)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	ULevel* Level = World->GetLevel(LevelIndex);
	if (!IsValid(Level))
		return nullptr;

	AActor* LevelScriptActor = Level->GetLevelScriptActor();
	if (!IsValid(LevelScriptActor))
		return nullptr;

	return LevelScriptActor;
}

AActor* UUnrealEngineExStatics::GetLevelScriptActorFromStreamingLevel(const UObject* WorldContextObject, ULevelStreaming* StreamingLevel)
{
	if (!IsValid(StreamingLevel))
		return nullptr;

	if (!StreamingLevel->IsLevelLoaded())
		return nullptr;

	AActor* LevelScriptActor = StreamingLevel->GetLevelScriptActor();
	if (!IsValid(LevelScriptActor))
		return nullptr;

	return LevelScriptActor;
}

UObject* UUnrealEngineExStatics::FindLevelScriptObject(const UObject* WorldContextObject, UClass* ObjectClass)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	AActor* LevelScriptActor = GetLevelScriptActor(WorldContextObject);
	if (UKismetSystemLibrary::DoesImplementInterface(LevelScriptActor, ObjectClass))
		return LevelScriptActor;

	for (ULevelStreaming* Sublevel : World->GetStreamingLevels())
	{
		AActor* SublevelScriptActor = GetLevelScriptActorFromStreamingLevel(WorldContextObject, Sublevel);
		if (UKismetSystemLibrary::DoesImplementInterface(SublevelScriptActor, ObjectClass))
			return SublevelScriptActor;
	}

	return nullptr;
}

bool UUnrealEngineExStatics::FindLevelScriptObjects(const UObject* WorldContextObject, UClass* ObjectClass, TArray<UObject*>& Objects)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return false;

	AActor* LevelScriptActor = GetLevelScriptActor(WorldContextObject);
	if (UKismetSystemLibrary::DoesImplementInterface(LevelScriptActor, ObjectClass))
		Objects.Add(LevelScriptActor);

	for (ULevelStreaming* Sublevel : World->GetStreamingLevels())
	{
		AActor* SublevelScriptActor = GetLevelScriptActorFromStreamingLevel(WorldContextObject, Sublevel);
		if (UKismetSystemLibrary::DoesImplementInterface(SublevelScriptActor, ObjectClass))
			Objects.Add(SublevelScriptActor);
	}

	return Objects.Num() != 0;
}

ULevelStreaming* UUnrealEngineExStatics::AddStreamingLevel(UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	auto LevelStreamingPtr = World->GetStreamingLevels().FindByPredicate(FFindStreamingLevelBy(Level));
	if (LevelStreamingPtr != nullptr)
		return *LevelStreamingPtr;

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 21
	ULevelStreaming* AsyncLevel = NewObject<ULevelStreaming>(World, ULevelStreamingDynamic::StaticClass(), NAME_None, RF_Transient, NULL);
#else
	ULevelStreaming* AsyncLevel = NewObject<ULevelStreaming>(World, ULevelStreamingKismet::StaticClass(), NAME_None, RF_Transient, NULL);
#endif
	AsyncLevel->SetWorldAsset(Level);
	AsyncLevel->SetShouldBeLoaded(false);
	AsyncLevel->SetShouldBeVisible(false);
	AsyncLevel->LevelTransform = FTransform::Identity;

#if WITH_EDITOR
	int32 PIEInstanceID = World->GetOutermost()->PIEInstanceID;
	if (World->WorldType == EWorldType::PIE && PIEInstanceID != -1)
	{
		AsyncLevel->RenameForPIE(World->GetOutermost()->PIEInstanceID);
	}
#endif

	World->AddStreamingLevel(AsyncLevel);

	return AsyncLevel;
}

// Stream Level Action
class FStreamLevelStreamingListAction : public FPendingLatentAction
{
public:
	TArray<ULevelStreaming*> LevelList;
	FLatentActionInfo LatentInfo;
	UWorld* World;

	bool bLoading;
	bool bMakeVisibleAfterLoad;
	bool bShouldBlockOnLoad;
	ULevelStreaming* Level;

	FUnrealEngineExOnLevelStreamedDelegate OnLevelStreamedCallback;



	FStreamLevelStreamingListAction(bool bIsLoading, const TArray<ULevelStreaming*>& InLevelList, bool bIsMakeVisibleAfterLoad, bool bIsShouldBlockOnLoad
		, const FUnrealEngineExOnLevelStreamedDelegate& InOnLevelStreamedCallback, const FLatentActionInfo& InLatentInfo, UWorld* InWorld)
		: LevelList(InLevelList)
		, LatentInfo(InLatentInfo)
		, World(InWorld)
		, bLoading(bIsLoading)
		, bMakeVisibleAfterLoad(bIsMakeVisibleAfterLoad)
		, bShouldBlockOnLoad(bIsShouldBlockOnLoad)
		, Level(LevelList[0])
		, OnLevelStreamedCallback(InOnLevelStreamedCallback)
	{
		ActivateLevel(Level);
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if (UpdateLevel(Level))
		{
			if (Level != nullptr)
			{
				OnLevelStreamedCallback.ExecuteIfBound(Level);
			}

			LevelList.RemoveAt(0);

			if (LevelList.Num() == 0)
			{
				OnLevelStreamedCallback.ExecuteIfBound(nullptr);
				Response.FinishAndTriggerIf(true, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
			}
			else
			{
				Level = LevelList[0];
				ActivateLevel(Level);
			}
		}
	}

#if WITH_EDITOR
	// Returns a human readable description of the latent operation's current state
	virtual FString GetDescription() const override
	{
		return LevelList.Num() > 0
			? FString::Printf(TEXT("Streaming Level in progress...(%s)"), *FUnrealEngineEx::GetLevelName(LevelList[0]))
			: FString::Printf(TEXT("Streaming is finished."));
	}
#endif

	void ActivateLevel(ULevelStreaming* LevelStreamingObject)
	{
		if (LevelStreamingObject != NULL)
		{
			// Loading.
			if (bLoading)
			{
				UE_LOG(LogStreaming, Log, TEXT("Streaming in level %s (%s)..."), *LevelStreamingObject->GetName(), *LevelStreamingObject->GetWorldAssetPackageName());
				LevelStreamingObject->SetShouldBeLoaded(true);
				LevelStreamingObject->SetShouldBeVisible(LevelStreamingObject->GetShouldBeVisibleFlag() | bMakeVisibleAfterLoad);
				LevelStreamingObject->bShouldBlockOnLoad = bShouldBlockOnLoad;
			}
			// Unloading.
			else
			{
				UE_LOG(LogStreaming, Log, TEXT("Streaming out level %s (%s)..."), *LevelStreamingObject->GetName(), *LevelStreamingObject->GetWorldAssetPackageName());
				LevelStreamingObject->SetShouldBeLoaded(false);
				LevelStreamingObject->SetShouldBeVisible(false);
			}

			UWorld* LevelWorld = CastChecked<UWorld>(LevelStreamingObject->GetOuter());
			// If we have a valid world
			if (LevelWorld)
			{
				// Notify players of the change
				for (FConstPlayerControllerIterator Iterator = LevelWorld->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					APlayerController* PlayerController = Iterator->Get();

					UE_LOG(LogLevel, Log, TEXT("ActivateLevel %s %i %i %i"),
						*LevelStreamingObject->GetWorldAssetPackageName(),
						LevelStreamingObject->ShouldBeLoaded(),
						LevelStreamingObject->ShouldBeVisible(),
						LevelStreamingObject->bShouldBlockOnLoad);



					PlayerController->LevelStreamingStatusChanged(
						LevelStreamingObject,
						LevelStreamingObject->ShouldBeLoaded(),
						LevelStreamingObject->ShouldBeVisible(),
						LevelStreamingObject->bShouldBlockOnLoad,
						INDEX_NONE);
				}
			}
		}
	}

	bool UpdateLevel(ULevelStreaming* LevelStreamingObject)
	{
		// No level streaming object associated with this sequence.
		if (LevelStreamingObject == NULL)
		{
			return true;
		}
		// Level is neither loaded nor should it be so we finished (in the sense that we have a pending GC request) unloading.
		else if ((LevelStreamingObject->GetLoadedLevel() == NULL) && !LevelStreamingObject->ShouldBeLoaded())
		{
			return true;
		}
		// Level shouldn't be loaded but is as background level streaming is enabled so we need to fire finished event regardless.
		else if (LevelStreamingObject->GetLoadedLevel() && !LevelStreamingObject->ShouldBeLoaded() && !GUseBackgroundLevelStreaming)
		{
			return true;
		}
		// Level is both loaded and wanted so we finished loading.
		else if (LevelStreamingObject->GetLoadedLevel() && LevelStreamingObject->ShouldBeLoaded()
			// Make sure we are visible if we are required to be so.
			&& (!bMakeVisibleAfterLoad || LevelStreamingObject->GetLoadedLevel()->bIsVisible))
		{
			return true;
		}

		// Loading/ unloading in progress.
		return false;
	}
};


// Stream Level Action
class FStreamLevelListAction : public FPendingLatentAction
{
public:
	TArray<TSoftObjectPtr<UWorld>> LevelList;
	FLatentActionInfo LatentInfo;
	UWorld* World;

	bool bLoading;
	bool bMakeVisibleAfterLoad;
	bool bShouldBlockOnLoad;
	ULevelStreaming* Level;

	FUnrealEngineExOnLevelStreamedDelegate OnLevelStreamedCallback;



	FStreamLevelListAction(bool bIsLoading, const TArray<TSoftObjectPtr<UWorld>>& InLevelList, bool bIsMakeVisibleAfterLoad, bool bIsShouldBlockOnLoad
		, const FUnrealEngineExOnLevelStreamedDelegate& InOnLevelStreamedCallback, const FLatentActionInfo& InLatentInfo, UWorld* InWorld)
		: LevelList(InLevelList)
		, LatentInfo(InLatentInfo)
		, World(InWorld)
		, bLoading(bIsLoading)
		, bMakeVisibleAfterLoad(bIsMakeVisibleAfterLoad)
		, bShouldBlockOnLoad(bIsShouldBlockOnLoad)
		, Level(FindLevel(LevelList[0]))
		, OnLevelStreamedCallback(InOnLevelStreamedCallback)
	{
		ActivateLevel(Level);
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if (UpdateLevel(Level))
		{
			if (Level != nullptr)
			{
				OnLevelStreamedCallback.ExecuteIfBound(Level);
			}

			LevelList.RemoveAt(0);

			if (LevelList.Num() == 0)
			{
				OnLevelStreamedCallback.ExecuteIfBound(nullptr);
				Response.FinishAndTriggerIf(true, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
			}
			else
			{
				Level = FindLevel(LevelList[0]);
				ActivateLevel(Level);
			}
		}
	}

#if WITH_EDITOR
	// Returns a human readable description of the latent operation's current state
	virtual FString GetDescription() const override
	{
		return LevelList.Num() > 0
			? FString::Printf(TEXT("Streaming Level in progress...(%s)"), *FUnrealEngineEx::GetLevelName(LevelList[0]))
			: FString::Printf(TEXT("Streaming is finished."));
	}
#endif

	ULevelStreaming* FindLevel(const TSoftObjectPtr<UWorld>& LevelAssetPtr)
	{
		auto LevelPtr = World->GetStreamingLevels().FindByPredicate(FFindStreamingLevelBy(LevelAssetPtr));

		return LevelPtr ? *LevelPtr : nullptr;
	}

	void ActivateLevel(ULevelStreaming* LevelStreamingObject)
	{
		if (LevelStreamingObject != NULL)
		{
			// Loading.
			if (bLoading)
			{
				UE_LOG(LogStreaming, Log, TEXT("Streaming in level %s (%s)..."), *LevelStreamingObject->GetName(), *LevelStreamingObject->GetWorldAssetPackageName());
				LevelStreamingObject->SetShouldBeLoaded(true);
				LevelStreamingObject->SetShouldBeVisible(LevelStreamingObject->GetShouldBeVisibleFlag() | bMakeVisibleAfterLoad);
				LevelStreamingObject->bShouldBlockOnLoad = bShouldBlockOnLoad;
			}
			// Unloading.
			else
			{
				UE_LOG(LogStreaming, Log, TEXT("Streaming out level %s (%s)..."), *LevelStreamingObject->GetName(), *LevelStreamingObject->GetWorldAssetPackageName());
				LevelStreamingObject->SetShouldBeLoaded(false);
				LevelStreamingObject->SetShouldBeVisible(false);
			}

			UWorld* LevelWorld = CastChecked<UWorld>(LevelStreamingObject->GetOuter());
			// If we have a valid world
			if (LevelWorld)
			{
				// Notify players of the change
				for (FConstPlayerControllerIterator Iterator = LevelWorld->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					APlayerController* PlayerController = Iterator->Get();

					UE_LOG(LogLevel, Log, TEXT("ActivateLevel %s %i %i %i"),
						*LevelStreamingObject->GetWorldAssetPackageName(),
						LevelStreamingObject->ShouldBeLoaded(),
						LevelStreamingObject->ShouldBeVisible(),
						LevelStreamingObject->bShouldBlockOnLoad);



					PlayerController->LevelStreamingStatusChanged(
						LevelStreamingObject,
						LevelStreamingObject->ShouldBeLoaded(),
						LevelStreamingObject->ShouldBeVisible(),
						LevelStreamingObject->bShouldBlockOnLoad,
						INDEX_NONE);
				}
			}
		}
	}

	bool UpdateLevel(ULevelStreaming* LevelStreamingObject)
	{
		// No level streaming object associated with this sequence.
		if (LevelStreamingObject == NULL)
		{
			return true;
		}
		// Level is neither loaded nor should it be so we finished (in the sense that we have a pending GC request) unloading.
		else if ((LevelStreamingObject->GetLoadedLevel() == NULL) && !LevelStreamingObject->ShouldBeLoaded())
		{
			return true;
		}
		// Level shouldn't be loaded but is as background level streaming is enabled so we need to fire finished event regardless.
		else if (LevelStreamingObject->GetLoadedLevel() && !LevelStreamingObject->ShouldBeLoaded() && !GUseBackgroundLevelStreaming)
		{
			return true;
		}
		// Level is both loaded and wanted so we finished loading.
		else if (LevelStreamingObject->GetLoadedLevel() && LevelStreamingObject->ShouldBeLoaded()
			// Make sure we are visible if we are required to be so.
			&& (!bMakeVisibleAfterLoad || LevelStreamingObject->GetLoadedLevel()->bIsVisible))
		{
			return true;
		}

		// Loading/ unloading in progress.
		return false;
	}
};

void UUnrealEngineExStatics::LoadStreamLevelList(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList
	, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return;
	}

	if (LevelList.Num() == 0)
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	if (LatentManager.FindExistingAction<FStreamLevelListAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
	{
		FStreamLevelListAction* NewAction = new FStreamLevelListAction(true, LevelList, bMakeVisibleAfterLoad, bShouldBlockOnLoad, OnLevelStreamedCallback, LatentInfo, World);
		LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
	}
}

void UUnrealEngineExStatics::LoadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList
	, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return;
	}

	if (LevelList.Num() == 0)
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	if (LatentManager.FindExistingAction<FStreamLevelStreamingListAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
	{
		FStreamLevelStreamingListAction* NewAction = new FStreamLevelStreamingListAction(true, LevelList, bMakeVisibleAfterLoad, bShouldBlockOnLoad, OnLevelStreamedCallback, LatentInfo, World);
		LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
	}
}

void UUnrealEngineExStatics::UnloadStreamLevelList(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList
	, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	if (LevelList.Num() == 0)
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	if (LatentManager.FindExistingAction<FStreamLevelListAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
	{
		FStreamLevelListAction* NewAction = new FStreamLevelListAction(false, LevelList, false, false, OnLevelStreamedCallback, LatentInfo, World);
		LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
	}
}

void UUnrealEngineExStatics::UnloadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList
	, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	if (LevelList.Num() == 0)
	{
		OnLevelStreamedCallback.ExecuteIfBound(nullptr);
		return FUnrealEngineEx::FinishLatentAction(World->GetLatentActionManager(), LatentInfo);
	}

	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	if (LatentManager.FindExistingAction<FStreamLevelListAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
	{
		FStreamLevelStreamingListAction* NewAction = new FStreamLevelStreamingListAction(false, LevelList, false, false, OnLevelStreamedCallback, LatentInfo, World);
		LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
	}
}


void UUnrealEngineExStatics::UnloadStreamLevelListBlocking(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return;

	if (LevelList.Num() == 0)
		return;

	for (const TSoftObjectPtr<UWorld>& Level : LevelList)
	{
		auto LevelPtr = World->GetStreamingLevels().FindByPredicate(FFindStreamingLevelBy(Level));

		if (LevelPtr != nullptr)
		{
			ULevelStreaming* LevelStreamingObject = *LevelPtr;

			UE_LOG(LogStreaming, Log, TEXT("Streaming out level %s (%s)..."), *LevelStreamingObject->GetName(), *LevelStreamingObject->GetWorldAssetPackageName());
			LevelStreamingObject->SetShouldBeLoaded(false);
			LevelStreamingObject->SetShouldBeVisible(false);

			UWorld* LevelWorld = CastChecked<UWorld>(LevelStreamingObject->GetOuter());
			// If we have a valid world
			if (IsValid(LevelWorld))
			{
				// Notify players of the change
				for (FConstPlayerControllerIterator Iterator = LevelWorld->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					APlayerController* PlayerController = Iterator->Get();

					UE_LOG(LogLevel, Log, TEXT("ActivateLevel %s %i %i %i"),
						*LevelStreamingObject->GetWorldAssetPackageName(),
						LevelStreamingObject->ShouldBeLoaded(),
						LevelStreamingObject->ShouldBeVisible(),
						LevelStreamingObject->bShouldBlockOnLoad);



					PlayerController->LevelStreamingStatusChanged(
						LevelStreamingObject,
						LevelStreamingObject->ShouldBeLoaded(),
						LevelStreamingObject->ShouldBeVisible(),
						LevelStreamingObject->bShouldBlockOnLoad,
						INDEX_NONE);
				}
			}
		}
	}
}

void UUnrealEngineExStatics::UnloadStreamLevelStreamingListBlocking(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return;

	if (LevelList.Num() == 0)
		return;

	for (ULevelStreaming* Level : LevelList)
	{
		UE_LOG(LogStreaming, Log, TEXT("Streaming out level %s (%s)..."), *Level->GetName(), *Level->GetWorldAssetPackageName());
		Level->SetShouldBeLoaded(false);
		Level->SetShouldBeVisible(false);

		UWorld* LevelWorld = CastChecked<UWorld>(Level->GetOuter());
		// If we have a valid world
		if (IsValid(LevelWorld))
		{
			// Notify players of the change
			for (FConstPlayerControllerIterator Iterator = LevelWorld->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				APlayerController* PlayerController = Iterator->Get();

				UE_LOG(LogLevel, Log, TEXT("ActivateLevel %s %i %i %i"),
					*Level->GetWorldAssetPackageName(),
					Level->ShouldBeLoaded(),
					Level->ShouldBeVisible(),
					Level->bShouldBlockOnLoad);


				PlayerController->LevelStreamingStatusChanged(
					Level,
					Level->ShouldBeLoaded(),
					Level->ShouldBeVisible(),
					Level->bShouldBlockOnLoad,
					INDEX_NONE);
			}
		}
	}
}

void UUnrealEngineExStatics::ShowAllStreamingLevels(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return;

	for (ULevelStreaming* Level : World->GetStreamingLevels())
	{
		Level->SetShouldBeVisible(true);
	}
}

float UUnrealEngineExStatics::GetTimerPercentTimeHandle(const UObject* WorldContextObject, FTimerHandle Handle)
{
	if (!IsValid(Handle))
		return 0.f;

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		float ElapsedTime = World->GetTimerManager().GetTimerElapsed(Handle);
		float RemainigTime = World->GetTimerManager().GetTimerRemaining(Handle);

		float TotalTime = ElapsedTime + RemainigTime;
		if (TotalTime == 0)
			return 1.f;

		return ElapsedTime / TotalTime;
	}

	return 0.f;
}

UAsyncTask* UUnrealEngineExStatics::CreateAsyncTask(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	return NewObject<UAsyncTask>(World, AsyncTaskClass);
}


UAsyncTask* UUnrealEngineExStatics::CreateAsyncTaskWithCallback(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass, const FUnrealEngineExOnAsyncTaskFinishedDelegate& OnFinished, bool bAutorun)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	UAsyncTask* AsyncTask = NewObject<UAsyncTask>(World, AsyncTaskClass);

	AsyncTask->OnFinishedDelegate = OnFinished;
	if (bAutorun)
	{
		AsyncTask->Run();
	}

	return AsyncTask;
}

UAsyncTask* UUnrealEngineExStatics::CreateAsyncTaskNoCallback(const UObject* WorldContextObject, TSubclassOf<class UAsyncTask> AsyncTaskClass, bool bAutorun)
{
	return CreateAsyncTaskWithCallback(WorldContextObject, AsyncTaskClass, FUnrealEngineExOnAsyncTaskFinishedDelegate(), bAutorun);
}

void UUnrealEngineExStatics::ClearAsyncTasks()
{
	for (UAsyncTask* AsyncTask : UAsyncTaskManager::Instance->Tasks)
	{
		if (IsValid(AsyncTask))
		{
			AsyncTask->Abort();
		}
	}

	UAsyncTaskManager::Instance->Tasks.Empty();
}

FNetworkStatus UUnrealEngineExStatics::GetNetworkStatus(const UObject* WorldContextObject)
{
	FNetworkStatus Result;

	if (auto LocalPlayerController = UUnrealEngineExStatics::GetLocalPlayerController(WorldContextObject))
	{
		if (auto NetConnection = LocalPlayerController->GetNetConnection())
		{
			Result.Latency = NetConnection->AvgLag;
		}
	}

	Result.ServerTime = UUnrealEngineExStatics::GetServerWorldTimeSeconds(WorldContextObject);

	return Result;
}

void UUnrealEngineExStatics::NetRole(const UObject* WorldContextObject, TEnumAsByte<ENetRole>& OutNetRole)
{
	if (!IsValid(WorldContextObject))
	{
		OutNetRole = ROLE_None;
		return;
	}

	if (auto AsActorComponent = Cast<UActorComponent>(WorldContextObject))
	{
		OutNetRole = GetNetRole(AsActorComponent);
		return;
	}
	if (auto AsActor = Cast<AActor>(WorldContextObject))
	{
		OutNetRole = GetNetRole(AsActor);
		return;
	}

	NetRole(WorldContextObject->GetOuter(), OutNetRole);
}

bool UUnrealEngineExStatics::IsSingleplayer(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
	{
		UE_LOG_EX_WCO(LogUnrealEngineEx, Verbose, WorldContextObject, TEXT("%s"), TEXT_BOOL(false));
		return true;
	}

	switch (World->GetNetMode())
	{
	case NM_Standalone:
		UE_LOG_EX_WCO(LogUnrealEngineEx, Verbose, WorldContextObject, TEXT("%s"), TEXT_BOOL(GetPlayNumberOfClients() < 2));
		return GetPlayNumberOfClients() < 2;
	case NM_DedicatedServer:
	case NM_ListenServer:
	case NM_Client:
		UE_LOG_EX_WCO(LogUnrealEngineEx, Verbose, WorldContextObject, TEXT("%s"), TEXT_BOOL(false));
		return false;
	}

	return false;
}

bool UUnrealEngineExStatics::IsServer(const UObject* WorldContextObject)
{
	return UGameplayStatics::GetGameMode(WorldContextObject) != nullptr;
}

bool UUnrealEngineExStatics::ListenServer(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return false;

	if (IsValid(World->NetDriver))
	{
		StopServer(WorldContextObject);
	}

	FURL Url(nullptr, *World->GetLocalURL(), TRAVEL_Absolute);
	return World->Listen(Url);
}

bool UUnrealEngineExStatics::StopServer(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return false;

	GEngine->ShutdownWorldNetDriver(World);

	return true;
}

bool UUnrealEngineExStatics::ServerTravel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, bool bAbsolute, FString Options)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return false;

	if (auto GameMode = UGameplayStatics::GetGameMode(WorldContextObject))
	{
		if (World->WorldType == EWorldType::PIE)
		{
			GameMode->bUseSeamlessTravel = false;
		}
	}

	FWorldContext& WorldContext = GEngine->GetWorldContextFromWorldChecked(World);
	FString TravelURL = FPackageName::ObjectPathToPackageName(Level.ToString());
	if (Options.Len() > 0)
	{
		TravelURL += FString(TEXT("?")) + Options;
	}

	return World->ServerTravel(TravelURL, bAbsolute);
}

void UUnrealEngineExStatics::ShutdownGame()
{
	FPlatformMisc::RequestExit(false);
}

FTransform UUnrealEngineExStatics::GetEditorViewTransform()
{
#if WITH_EDITOR
	if (!IsValid(GEditor))
		return FTransform::Identity;

	if (GEditor->GetActiveViewport() == nullptr)
		return FTransform::Identity;

	if (GEditor->GetActiveViewport()->GetClient() == nullptr)
		return FTransform::Identity;

	FViewportCameraTransform VPCTranform = ((FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient())->GetViewTransform();

	return FTransform(VPCTranform.GetRotation(), VPCTranform.GetLocation());
#else
	return FTransform::Identity;
#endif
}

void UUnrealEngineExStatics::SetEditorViewTransform(FTransform Transform)
{
#if WITH_EDITOR
	if (!IsValid(GEditor))
		return;

	if (GEditor->GetActiveViewport() == nullptr)
		return;

	if (GEditor->GetActiveViewport()->GetClient() == nullptr)
		return;

	FEditorViewportClient* Client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	Client->SetViewLocation(Transform.GetLocation());
	Client->SetViewRotation(Transform.GetRotation().Rotator());
	Client->Invalidate();
#endif
}

int32 UUnrealEngineExStatics::GetPlayNumberOfClients()
{
#if WITH_EDITOR
	const ULevelEditorPlaySettings* PlayInSettings = GetDefault<ULevelEditorPlaySettings>();
	int32 PlayNumberOfClients(0);
	PlayInSettings->GetPlayNumberOfClients(PlayNumberOfClients);	// Ignore 'state' of option (handled externally)
	return PlayNumberOfClients;
#endif
	return 0;
}


FTransform UUnrealEngineExStatics::GetTransfromInFrontOfPlayer(AActor* PlayerPawnOrController, FVector Offset)
{
	const APlayerController* Controller = Cast<APlayerController>(PlayerPawnOrController);
	if (!IsValid(Controller))
	{
		const APawn* Pawn = Cast<APawn>(PlayerPawnOrController);
		if (IsValid(Pawn))
			Controller = Cast<APlayerController>(Pawn->GetController());
	}

	if (!IsValid(Controller))
		return FTransform::Identity;

	APlayerCameraManager* CameraManager = Controller->PlayerCameraManager;
	if (!IsValid(CameraManager))
		return FTransform::Identity;

	FVector CameraLocation = CameraManager->GetCameraLocation();
	FRotator CameraRotation = CameraManager->GetCameraRotation();

	FTransform Result(CameraRotation, CameraLocation, FVector::OneVector);
	Result.AddToTranslation(
		MakeCoordinateFrame(Result).GetForwardVector() * Offset.X
		+ MakeCoordinateFrame(Result).GetRightVector() * Offset.Y
		+ MakeCoordinateFrame(Result).GetUpVector() * Offset.Z
	);

	return Result;
}

bool UUnrealEngineExStatics::ReplaceWidget(UWidget* OldWidget, UWidget* NewWidget)
{
	if (!IsValid(OldWidget) || !IsValid(NewWidget))
		return false;

	UPanelWidget* Parent = OldWidget->GetParent();
	if (!IsValid(Parent))
		return false;

	// HACK: Because UPanelWidget::ReplaceChildAt does no call OnSlotAdded and OnSlotRemoved.
	int32 OldIndex = Parent->GetChildIndex(OldWidget);
	TArray<UPanelSlot*>& ParenSlots = const_cast<TArray<UPanelSlot*>&>(Parent->GetSlots());

	OldWidget->RemoveFromParent();
	UPanelSlot* NewSlot = Parent->AddChild(NewWidget);
	ParenSlots.Remove(NewSlot);
	ParenSlots.Insert(NewSlot, OldIndex);

	Parent->InvalidateLayoutAndVolatility();

	return true;
}

void ForeachChildWidget(UWidget* ParentWidget, TFunction<bool(UWidget*)> Predicate, bool TopLevelOnly)
{
	if (!IsValid(ParentWidget))
		return;

	TInlineComponentArray<UWidget*> WidgetsToCheck;
	TInlineComponentArray<UWidget*> CheckedWidgets;

	WidgetsToCheck.Push(ParentWidget);

	while (WidgetsToCheck.Num() > 0)
	{
		const bool bAllowShrinking = false;
		UWidget* PossibleParent = WidgetsToCheck.Pop(bAllowShrinking);

		if (CheckedWidgets.Contains(PossibleParent))
			continue;

		CheckedWidgets.Add(PossibleParent);

		TArray<UWidget*> Widgets;
		if (auto AsUserWidget = Cast<UUserWidget>(PossibleParent))
		{
			UWidgetTree::GetChildWidgets(AsUserWidget->GetRootWidget(), Widgets);
		}
		else
		{
			UWidgetTree::GetChildWidgets(PossibleParent, Widgets);
		}

		for (UWidget* Widget : Widgets)
		{
			if (CheckedWidgets.Contains(Widget))
				continue;

			if (!Predicate(Widget))
				return;

			if (!TopLevelOnly)
			{
				WidgetsToCheck.Push(Widget);
			}
		}

		if (TopLevelOnly)
		{
			break;
		}
	}
}

UUserWidget* UUnrealEngineExStatics::GetFirstChildWidgetsOfClass(UWidget* ParentWidget, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly)
{
	UUserWidget* Result = nullptr;
	ForeachChildWidget(ParentWidget, [&Result, &WidgetClass](UWidget* Widget) {
		if (Widget->GetClass()->IsChildOf(WidgetClass))
		{
			Result = Cast<UUserWidget>(Widget);

			return false;
		}

		return true;
	}, TopLevelOnly);

	return Result;
}

void UUnrealEngineExStatics::GetAllChildWidgetsOfClass(UWidget* ParentWidget, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly)
{
	FoundWidgets.Empty();

	ForeachChildWidget(ParentWidget, [&FoundWidgets, &WidgetClass](UWidget* Widget) {
		if (Widget->GetClass()->IsChildOf(WidgetClass))
		{
			FoundWidgets.Add(Cast<UUserWidget>(Widget));
		}

		return true;
	}, TopLevelOnly);
}

void UUnrealEngineExStatics::GetAllChildWidgetsOfInterface(class UWidget* ParentWidget, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UInterface> WidgetInterface, bool TopLevelOnly)
{
	FoundWidgets.Empty();

	ForeachChildWidget(ParentWidget, [&FoundWidgets, &WidgetInterface](UWidget* Widget) {
		if (Widget->GetClass()->ImplementsInterface(WidgetInterface))
		{
			if (auto UserWidget = Cast<UUserWidget>(Widget))
			{
				FoundWidgets.Add(Cast<UUserWidget>(Widget));
			}
		}

		return true;
	}, TopLevelOnly);
}

UWidget* UUnrealEngineExStatics::GetParentEx(UWidget* Widget)
{
	if (!IsValid(Widget))
		return nullptr;

	if (auto Parent = Widget->GetParent())
		return Parent;

	return Widget->GetTypedOuter<UWidget>();
}

FVector2D UUnrealEngineExStatics::GetWidgetPositionOnViewport(UWidget* Widget)
{
	auto WidgetGeometry = Widget->GetPaintSpaceGeometry();
	auto AnchorPosition = FVector2D::ZeroVector;

	if (auto CanvasSlot = Valid<UCanvasPanelSlot>(Widget->Slot))
	{
		auto SlotSize = CanvasSlot->GetSize();
		auto SlotAlignement = CanvasSlot->GetAlignment();
		AnchorPosition = SlotSize * SlotAlignement;
	}

	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(Widget, WidgetGeometry, AnchorPosition, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FString UUnrealEngineExStatics::GetInstanceStringID(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return FString();

	switch (World->GetNetMode())
	{
	case NM_Client:
		return FString::Printf(TEXT("Client %d"), GPlayInEditorID - 1);
	case NM_DedicatedServer:
		return FString::Printf(TEXT("Dedicated Server"));
	case NM_ListenServer:
		return FString::Printf(TEXT("Listen Server"));
	}

	return FString::Printf(TEXT("Standalone"));
}

FDebugFloatHistory UUnrealEngineExStatics::RenormalizeFloatHistorySamples(FVector2D Bounds, const FDebugFloatHistory& FloatHistory)
{
	if (FloatHistory.GetNumSamples() < 1)
		return FloatHistory;

	FDebugFloatHistory* const MutableFloatHistory = const_cast<FDebugFloatHistory*>(&FloatHistory);

	float MinValue = TNumericLimits<float>::Max();
	float MaxValue = TNumericLimits<float>::Min();

	for (float Value : FloatHistory.GetSamples())
	{
		MinValue = FMath::Min(MinValue, Value);
		MaxValue = FMath::Max(MaxValue, Value);
	}

	if (Bounds.X >= Bounds.Y)
		Bounds = FVector2D(0, 1);

	float Delta = (MaxValue - MinValue);
	if (FMath::IsNearlyZero(Delta))
	{
		MutableFloatHistory->MaxValue = MaxValue + 10.f;
		MutableFloatHistory->MinValue = MinValue - 10.f;
	}
	else
	{
		float Offset = -Bounds.X;
		Offset *= Delta / (Bounds.Y - Bounds.X);

		MutableFloatHistory->MinValue = MinValue + Offset;
		MutableFloatHistory->MaxValue = (MaxValue - MutableFloatHistory->MinValue * (1.f - Bounds.Y)) / Bounds.Y;
	}

	return FloatHistory;
}

void UUnrealEngineExStatics::DrawDebugFloatHistoryTransformEx(UObject* WorldContextObject, const FDebugFloatHistory& FloatHistory, const FTransform& DrawTransform, FVector2D DrawSize, FLinearColor DrawColor, float LifeTime)
{
#if ENABLE_DRAW_DEBUG
	UWorld * World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return;

	::DrawDebugFloatHistoryEx(*World, FloatHistory, DrawTransform, DrawSize, DrawColor.ToFColor(true), false, LifeTime);
#endif
}

bool UUnrealEngineExStatics::IsKeyMappedToAction(FKey Key, const FName ActionName)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (!IsValid(InputSettings))
		return false;

	TArray<FInputActionKeyMapping> Mappings;
	InputSettings->GetActionMappingByName(ActionName, Mappings);
	for (FInputActionKeyMapping& Mapping : Mappings)
	{
		if (Mapping.Key == Key)
			return true;
	}

	return false;
}
