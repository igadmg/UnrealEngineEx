#include "UnrealEngineExPrivatePCH.h"
#include "UnrealEngineExStatics.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/PanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Engine/CoreSettings.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LocalPlayer.h"
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

#include "CoordinateFrame.h"



struct FFindStreamingLevelBy {
	const TAssetPtr<UWorld>& LevelAssetPtr;
	FFindStreamingLevelBy(const TAssetPtr<UWorld>& LevelAssetPtr) : LevelAssetPtr(LevelAssetPtr) {}
	bool operator ()(const ULevelStreaming* Level) const { return FUnrealEngineEx::ConvertLevelPtrFromPIE(Level->GetWorldAsset(), Level->GetWorld()) == LevelAssetPtr; }
};


void UUnrealEngineExStatics::WorldType(const UObject* WorldContextObject, EBPWorldType& OutWorldType)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	OutWorldType = (EBPWorldType)(IsValid(World) ? (EWorldType::Type)World->WorldType : EWorldType::None);
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

AHUD* UUnrealEngineExStatics::GetPlayerHUD(const UObject* Object)
{
	if (!IsValid(Object))
		return nullptr;

	const AHUD* AsHUD = Cast<AHUD>(Object);
	if (AsHUD)
	{
		return const_cast<AHUD*>(AsHUD);
	}
	if (auto PlayerController = Cast<APlayerController>(GetController(Object)))
	{
		return PlayerController->GetHUD();
	}

	return nullptr;
}

APlayerState* UUnrealEngineExStatics::GetPlayerState(const UObject* Object)
{
	if (!IsValid(Object))
		return nullptr;


	const AActor* AsActor = Cast<AActor>(Object);
	if (AsActor)
	{
		const APlayerState* AsPlayerState = Cast<APlayerState>(Object);
		if (AsPlayerState)
		{
			return const_cast<APlayerState*>(AsPlayerState);
		}

		const APawn* AsPawn = Cast<APawn>(Object);
		if (AsPawn)
		{
			return AsPawn->GetPlayerState();
		}

		const AController* AsController = Cast<AController>(Object);
		if (AsController)
		{
			return AsController->PlayerState;
		}

		const AHUD* AsHUD = Cast<AHUD>(Object);
		if (AsHUD)
		{
			return GetPlayerState(AsHUD->GetOwningPlayerController());
		}

		return GetPlayerState(AsActor->GetOwner());
	}
	else
	{
		const UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
		if (AsActorComponent)
		{
			return GetPlayerState(AsActorComponent->GetOwner());
		}

		const UUserWidget* AsUserWidget = Cast<UUserWidget>(Object);
		if (AsUserWidget)
		{
			return GetPlayerState(AsUserWidget->GetOwningPlayer());
		}
	}

	return GetPlayerState(Object->GetOuter());
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
	if (auto PlayerState = GetPlayerState(Object))
	{
		return PlayerState->GetPawn();
	}

	return nullptr;
}

ASpectatorPawn* UUnrealEngineExStatics::GetSpectatorPawn(const UObject* Object)
{
	if (auto Controller = Cast<APlayerController>(GetController(Object)))
	{
		return Controller->GetSpectatorPawn();
	}

	return nullptr;
}

AController* UUnrealEngineExStatics::GetController(const UObject* Object)
{
	if (!IsValid(Object))
		return nullptr;


	const AActor* AsActor = Cast<AActor>(Object);
	if (AsActor)
	{
		const AController* AsController = Cast<AController>(Object);
		if (AsController)
		{
			return const_cast<AController*>(AsController);
		}

		const APawn* AsPawn = Cast<APawn>(Object);
		if (AsPawn)
		{
			return AsPawn->GetController();
		}

		const AHUD* AsHUD = Cast<AHUD>(Object);
		if (AsHUD)
		{
			return AsHUD->GetOwningPlayerController();
		}

		return GetController(AsActor->GetOwner());
	}
	else
	{
		const UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
		if (AsActorComponent)
		{
			return GetController(AsActorComponent->GetOwner());
		}

		const UUserWidget* AsUserWidget = Cast<UUserWidget>(Object);
		if (AsUserWidget)
		{
			return AsUserWidget->GetOwningPlayer();
		}
	}

	return GetController(Object->GetOuter());
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

	const ACharacter* AsCharacter = Cast<ACharacter>(Object);
	if (AsCharacter)
	{
		return AsCharacter->GetCharacterMovement();
	}

	const UActorComponent* AsActorComponent = Cast<UActorComponent>(Object);
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

TAssetPtr<UWorld> UUnrealEngineExStatics::GetCurrentLevelAssetPtr(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	return FUnrealEngineEx::ConvertLevelPtrFromPIE(FUnrealEngineEx::GetLevelPtr(World), World);
}

FString UUnrealEngineExStatics::GetLevelName(const TAssetPtr<UWorld>& Level)
{
	return FUnrealEngineEx::GetLevelName(Level);
}

AActor* UUnrealEngineExStatics::GetLevelScriptActor(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return nullptr;

	ULevel* Level = World->GetLevel(0);
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

ULevelStreaming* UUnrealEngineExStatics::AddStreamingLevel(UObject* WorldContextObject, TAssetPtr<UWorld> Level)
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
	TArray<TAssetPtr<UWorld>> LevelList;
	FLatentActionInfo LatentInfo;
	UWorld* World;

	bool bLoading;
	bool bMakeVisibleAfterLoad;
	bool bShouldBlockOnLoad;
	ULevelStreaming* Level;

	FUnrealEngineExOnLevelStreamedDelegate OnLevelStreamedCallback;



	FStreamLevelListAction(bool bIsLoading, const TArray<TAssetPtr<UWorld>>& InLevelList, bool bIsMakeVisibleAfterLoad, bool bIsShouldBlockOnLoad
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

	ULevelStreaming* FindLevel(const TAssetPtr<UWorld>& LevelAssetPtr)
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

void UUnrealEngineExStatics::LoadStreamLevelList(const UObject* WorldContextObject, TArray<TAssetPtr<UWorld>> LevelList
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

void UUnrealEngineExStatics::UnloadStreamLevelList(const UObject* WorldContextObject, TArray<TAssetPtr<UWorld>> LevelList
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


void UUnrealEngineExStatics::UnloadStreamLevelListBlocking(const UObject* WorldContextObject, TArray<TAssetPtr<UWorld>> LevelList)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(World))
		return;

	if (LevelList.Num() == 0)
		return;

	for (const TAssetPtr<UWorld>& Level : LevelList)
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

UAsyncTask* UUnrealEngineExStatics::CreateAsyncTask(const UObject* WorldContextObject, TSubclassOf<UAsyncTask> AsyncTaskClass, const FUnrealEngineExOnAsyncTaskFinishedDelegate& OnFinished, bool bAutorun)
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
