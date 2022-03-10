#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealEngineExTypes.h"
#include "CoreEx.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealEngineExStatics.generated.h"



UCLASS()
class UNREALENGINEEX_API UUnrealEngineExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static TEnumAsByte<EWorldType::Type> GetWorldType(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (ExpandEnumAsExecs = "OutWorldType", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static void WorldType(const UObject* WorldContextObject, TEnumAsByte<EWorldType::Type>& OutWorldType);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (ExpandEnumAsExecs = "OutNetMode", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static void NetMode(const UObject* WorldContextObject, TEnumAsByte<ENetMode>& OutNetMode);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static float GetServerWorldTimeSeconds(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static bool IsListenServer(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static UObject* GetDefaultObject(TSubclassOf<UObject> ObjectClass);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class UGameInstance* GetDefaultGameInstance();

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static int32 GetPlayerControllerIndex(class APlayerController* PlayerController);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static class APlayerStart* FindPlayerStartByTag(const UObject* WorldContextObject, FName StartTag);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static void RestartPlayer(class AController* PlayerController);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static void RestartPlayerByState(class APlayerState* PlayerState);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class APlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static class AHUD* GetLocalPlayerHUD(const UObject* WorldContextObject);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class AActor* GetOwningActor(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object", DeterminesOutputType = "ActorClass"))
	static class AActor* GetOwningActorByClass(const UObject* Object, TSubclassOf<class AActor> ActorClass);

	/** Return and actor being controlled by Object (if it is owned by Controller, HUD etc.)
		Or retrun Actor itself.
		Some form of GetOwnerActor but it prefer to fallback to GetPawnOrSpectator and skip Controllers and Huds.
	*/
	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class AActor* GetControlledActor(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, BlueprintCosmetic, meta = (DefaultToSelf = "Object"))
	static class AHUD* GetPlayerHUD(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APlayerState* GetPlayerState(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static float GetPlayerScore(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APawn* GetPawnOrSpectator(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APawn* GetPlayerPawn(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, BlueprintCosmetic, meta = (DefaultToSelf = "Object"))
	static class ASpectatorPawn* GetSpectatorPawn(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class AController* GetController(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APlayerController* GetPlayerController(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class APlayerCameraManager* GetPlayerCameraManager(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class UCameraComponent* GetPlayerActiveCamera(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (DefaultToSelf = "Object"))
	static class UCharacterMovementComponent* GetCharacterMovementComponent(const UObject* Object);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static FViewSpaceDescriptor GetViewSpaceDescriptor(class ULocalPlayer* Player, FVector Locaton);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static FViewFrustum GetViewFrustum(class ULocalPlayer* Player);


	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static TSoftObjectPtr<UWorld> GetCurrentLevelAssetPtr(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintPure)
	static FString GetLevelName(const TSoftObjectPtr<UWorld>& Level);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static AActor* GetLevelScriptActor(const UObject* WorldContextObject, int32 LevelIndex = 0);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static AActor* GetLevelScriptActorFromStreamingLevel(const UObject* WorldContextObject, class ULevelStreaming* StreamingLevel);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UObject* FindLevelScriptObject(const UObject* WorldContextObject, UClass* ObjectClass);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static bool FindLevelScriptObjects(const UObject* WorldContextObject, UClass* ObjectClass, TArray<UObject*>& Objects);

	/** Add level as a sublevel. */
	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static class ULevelStreaming* AddStreamingLevel(UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level);

	/** Load a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void LoadStreamLevelList(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo);

	/** Load a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void LoadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelStreamingList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo);

	/** Unload a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void UnloadStreamLevelList(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo);

	/** Unload a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void UnloadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void UnloadStreamLevelListBlocking(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void UnloadStreamLevelStreamingListBlocking(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList);

	UFUNCTION(Category = "UnrealEngineEx|Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ShowAllStreamingLevels(const UObject* WorldContextObject);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTask(const UObject* WorldContextObject, TSubclassOf<class UAsyncTask> AsyncTaskClass);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTaskWithCallback(const UObject* WorldContextObject, TSubclassOf<class UAsyncTask> AsyncTaskClass, const FUnrealEngineExOnAsyncTaskFinishedDelegate& OnFinished, bool bAutorun = true);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTaskNoCallback(const UObject* WorldContextObject, TSubclassOf<class UAsyncTask> AsyncTaskClass, bool bAutorun = true);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static void ClearAsyncTasks();


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static FNetworkStatus GetNetworkStatus(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static bool IsSingleplayer(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (ExpandEnumAsExecs = "OutNetRole", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static void NetRole(const UObject* WorldContextObject, TEnumAsByte<ENetRole>& OutNetRole);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static bool IsServer(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool ListenServer(UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool StopServer(UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool ServerTravel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, bool bAbsolute = true, FString Options = FString(TEXT("")));


	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static void ShutdownGame();


	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static FTransform GetTransfromInFrontOfPlayer(class AActor* PlayerPawnOrController, FVector Offset);


	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable)
	static bool ReplaceWidget(class UWidget* OldWidget, class UWidget* NewWidget);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable, meta = (DeterminesOutputType = "WidgetClass"))
	static UUserWidget* GetFirstChildWidgetsOfClass(class UWidget* ParentWidget, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly = true);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable)
	static void GetAllChildWidgetsOfClass(class UWidget* ParentWidget, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly = true);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable)
	static void GetAllChildWidgetsOfInterface(class UWidget* ParentWidget, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UInterface> WidgetInterface, bool TopLevelOnly = true);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintPure)
	static class UWidget* GetParentEx(class UWidget* Widget);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintPure)
	static FVector2D GetWidgetPositionOnViewport(class UWidget* Widget);


	UFUNCTION(Category = "UnrealEngineEx|Debug", BlueprintCallable, meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static FString GetInstanceStringID(UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx|Debug", BlueprintCallable)
	static FDebugFloatHistory RenormalizeFloatHistorySamples(FVector2D Bounds, const FDebugFloatHistory& FloatHistory);

	UFUNCTION(Category = "UnrealEngineEx|Debug", BlueprintCallable, meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static void DrawDebugFloatHistoryTransformEx(UObject* WorldContextObject, const FDebugFloatHistory& FloatHistory, const FTransform& DrawTransform, FVector2D DrawSize, FLinearColor DrawColor = FLinearColor::White, float Duration = 0.f);


	UFUNCTION(Category = "Settings", BlueprintPure)
	static bool IsKeyMappedToAction(FKey Key, const FName ActionName);

	UFUNCTION(Category = "Utilities|String", BlueprintPure, meta = (DisplayName = "ToString (Timespan)", AdvancedDisplay = "1", BlueprintAutocast))
	static FString Conv_TimespanToString(FTimespan Timespan, FString Format)
	{
		return Timespan.ToString(*Format);
	}



public:
	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure)
	static FTransform GetEditorViewTransform();

	UFUNCTION(Category = "UnrealEditorEx", BlueprintCallable)
	static void SetEditorViewTransform(FTransform Transform);

	UFUNCTION(Category = "UnrealEditorEx", BlueprintPure, meta = (DevelopmentOnly))
	static int32 GetPlayNumberOfClients();
};


class AActor;
class AController;
class AGameModeBase;
class AHUD;
class APawn;
class APlayerState;
class ASpectatorPawn;

namespace XX
{	
	template <typename TActor = AActor>
	TActor* GetOwningActor(const UObject* Object)
	{
		return Cast<TActor>(UUnrealEngineExStatics::GetOwningActor(Object));
	}

	template <typename TActor = AActor>
	const TActor* GetOwningActor(const AActor* Object)
	{
		return Cast<TActor>(Object);
	}

	template <typename TActor = AActor>
	TActor* GetOwningActor(const UActorComponent* Object)
	{
		return Cast<TActor>(Object->GetOwner());
	}

	template <typename TGameMode = AGameModeBase>
	TGameMode* GetGameMode(const UObject* Object)
	{
		return TValid<TGameMode, AGameModeBase>::Valid(UGameplayStatics::GetGameMode(Object));
	}

	template <typename THUD = AHUD>
	THUD* GetPlayerHUD(const UObject* Object)
	{
		return TValid<THUD, AHUD>::Valid(UUnrealEngineExStatics::GetPlayerHUD(Object));
	}

	template <typename TPlayerState = APlayerState>
	TPlayerState* GetPlayerState(const UObject* Object)
	{
		return TValid<TPlayerState, APlayerState>::Valid(UUnrealEngineExStatics::GetPlayerState(Object));
	}

	template <typename TPawn = APawn>
	TPawn* GetPawnOrSpectator(const UObject* Object)
	{
		return TValid<TPawn, APawn>::Valid(UUnrealEngineExStatics::GetPawnOrSpectator(Object));
	}

	template <typename TPawn = APawn>
	TPawn* GetPlayerPawn(const UObject* Object)
	{
		return TValid<TPawn, APawn>::Valid(UUnrealEngineExStatics::GetPlayerPawn(Object));
	}

	template <typename TSpectatorPawn = ASpectatorPawn>
	TSpectatorPawn* GetSpectatorPawn(const UObject* Object)
	{
		return TValid<TSpectatorPawn, ASpectatorPawn>::Valid(UUnrealEngineExStatics::GetSpectatorPawn(Object));
	}

	template <typename TController = AController>
	TController* GetController(const UObject* Object)
	{
		return TValid<TController, AController>::Valid(UUnrealEngineExStatics::GetController(Object));
	}
}

namespace XX
{
	template <typename TActor = AActor>
	TActor* GetLevelScriptActor(const UObject* Object, int32 LevelIndex = 0)
	{
		return TValid<TActor, AActor>::Valid(UUnrealEngineExStatics::GetLevelScriptActor(Object, LevelIndex));
	}
}
