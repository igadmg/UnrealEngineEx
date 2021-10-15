#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealEngineExTypes.h"
#include "CoreEx.h"
#include "UnrealEngineExStatics.generated.h"



UENUM(BlueprintType)
enum class EBPWorldType : uint8
{
	None = 0,
	Game = EWorldType::Game,
	Editor = EWorldType::Editor,
	PIE = EWorldType::PIE,
	EditorPreview = EWorldType::EditorPreview,
	GamePreview = EWorldType::GamePreview,
	GameRPC = EWorldType::GameRPC,
	Inactive = EWorldType::Inactive
};

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	None = 0,
	UIOnly,
	GameAndUI,
	GameOnly,
};

#if !CPP
UENUM(BlueprintType)
enum ENetMode
{
	/** Standalone: a game without networking, with one or more local players. Still considered a server because it has all server functionality. */
	NM_Standalone,

	/** Dedicated server: server with no local players. */
	NM_DedicatedServer,

	/** Listen server: a server that also has a local player who is hosting the game, available to other players on the network. */
	NM_ListenServer,

	/**
	 * Network client: client connected to a remote server.
	 * Note that every mode less than this value is a kind of server, so checking NetMode < NM_Client is always some variety of server.
	 */
	 NM_Client
};
#endif

USTRUCT(BlueprintType)
struct UNREALENGINEEX_API FNetworkStatus
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadOnly)
	float Latency = 0;

	UPROPERTY(BlueprintReadOnly)
	float ServerTime = 0;
};

UCLASS()
class UNREALENGINEEX_API UUnrealEngineExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static EBPWorldType GetWorldType(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (ExpandEnumAsExecs = "OutWorldType", HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static void WorldType(const UObject* WorldContextObject, EBPWorldType& OutWorldType);

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


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, BlueprintCosmetic)
	static class AHUD* GetPlayerHUD(const UObject* Object);

	template <typename THUD>
	static THUD* GetPlayerHUD(const UObject* Object) { return Valid<THUD>(GetPlayerHUD(Object)); }

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APlayerState* GetPlayerState(const UObject* Object);

	template <typename T>
	static T* GetPlayerState(const UObject* Object) { return Valid<T>(GetPlayerState(Object)); }

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static float GetPlayerScore(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APawn* GetPawnOrSpectator(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APawn* GetPlayerPawn(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, BlueprintCosmetic)
	static class ASpectatorPawn* GetSpectatorPawn(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class AController* GetController(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APlayerController* GetPlayerController(const UObject* Object);

	template <typename T>
	static T* GetController(const UObject* Object) { return Valid<T>(GetController(Object)); }

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APlayerCameraManager* GetPlayerCameraManager(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class UCameraComponent* GetPlayerActiveCamera(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class UCharacterMovementComponent* GetCharacterMovementComponent(const UObject* Object);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static FViewSpaceDescriptor GetViewSpaceDescriptor(class ULocalPlayer* Player, FVector Locaton);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static FViewFrustum GetViewFrustum(class ULocalPlayer* Player);


	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static TSoftObjectPtr<UWorld> GetCurrentLevelAssetPtr(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure)
	static FString GetLevelName(const TSoftObjectPtr<UWorld>& Level);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static AActor* GetLevelScriptActor(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static AActor* GetLevelScriptActorFromStreamingLevel(const UObject* WorldContextObject, class ULevelStreaming* StreamingLevel);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UObject* FindLevelScriptObject(const UObject* WorldContextObject, UClass* ObjectClass);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static bool FindLevelScriptObjects(const UObject* WorldContextObject, UClass* ObjectClass, TArray<UObject*>& Objects);

	/** Add level as a sublevel. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static class ULevelStreaming* AddStreamingLevel(UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level);

	/** Load a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void LoadStreamLevelList(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo);

	/** Load a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void LoadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelStreamingList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo);

	/** Unload a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void UnloadStreamLevelList(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo);

	/** Unload a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void UnloadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void UnloadStreamLevelListBlocking(const UObject* WorldContextObject, TArray<TSoftObjectPtr<UWorld>> LevelList);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void UnloadStreamLevelStreamingListBlocking(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
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

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable)
	static void GetAllChildWidgetsOfClass(class UWidget* ParentWidget, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UUserWidget> WidgetClass, bool TopLevelOnly = true);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable)
	static void GetAllChildWidgetsOfInterface(class UWidget* ParentWidget, TArray<UUserWidget*>& FoundWidgets, TSubclassOf<UInterface> WidgetInterface, bool TopLevelOnly = true);

	UFUNCTION(Category = "UnrealEngineEx|User Interface", BlueprintCallable)
	static class UWidget* GetParentEx(class UWidget* Widget);


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
