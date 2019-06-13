#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealEngineExTypes.h"
#include "UnrealEngineExStatics.generated.h"



UCLASS()
class UNREALENGINEEX_API UUnrealEngineExStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()



public:
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


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static class APlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, BlueprintCosmetic)
	static class AHUD* GetPlayerHUD(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APlayerState* GetPlayerState(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APawn* GetPawnOrSpectator(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APawn* GetPlayerPawn(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, BlueprintCosmetic)
	static class ASpectatorPawn* GetSpectatorPawn(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class AController* GetController(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class APlayerCameraManager* GetCameraController(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class UCameraComponent* GetPlayerActiveCamera(const UObject* Object);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static class UCharacterMovementComponent* GetCharacterMovementComponent(const UObject* Object);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure)
	static FViewSpaceDescriptor GetViewSpaceDescriptor(class ULocalPlayer* Player, FVector Locaton);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static bool DoFrustumCheckSphere(class ULocalPlayer* Player, FVector Location, float Radius, bool& bIntersecting);


	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static TAssetPtr<UWorld> GetCurrentLevelAssetPtr(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintPure)
	static FString GetLevelName(const TAssetPtr<UWorld>& Level);

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
	static class ULevelStreaming* AddStreamingLevel(UObject* WorldContextObject, TAssetPtr<UWorld> Level);

	/** Load a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void LoadStreamLevelList(const UObject* WorldContextObject, TArray<TAssetPtr<UWorld>> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo);

	/** Load a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void LoadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelStreamingList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad, FLatentActionInfo LatentInfo);

	/** Unload a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void UnloadStreamLevelList(const UObject* WorldContextObject, TArray<TAssetPtr<UWorld>> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo);

	/** Unload a list of streamed in level. */
	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo"))
	static void UnloadStreamLevelStreamingList(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList
		, const FUnrealEngineExOnLevelStreamedDelegate& OnLevelStreamedCallback, FLatentActionInfo LatentInfo);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void UnloadStreamLevelListBlocking(const UObject* WorldContextObject, TArray<TAssetPtr<UWorld>> LevelList);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void UnloadStreamLevelStreamingListBlocking(const UObject* WorldContextObject, TArray<ULevelStreaming*> LevelList);

	UFUNCTION(Category = "UnrealEngineEx: Level", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ShowAllStreamingLevels(const UObject* WorldContextObject);


	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static UAsyncTask* CreateAsyncTask(const UObject* WorldContextObject, TSubclassOf<class UAsyncTask> AsyncTaskClass, const FUnrealEngineExOnAsyncTaskFinishedDelegate& OnFinished, bool bAutorun = true);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static void ClearAsyncTasks();

	UFUNCTION(Category = "UnrealEngineEx", BlueprintPure, meta = (HidePin = "WorldContextObject", WorldContext = "WorldContextObject"))
	static bool IsServer(const UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool ListenServer(UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool StopServer(UObject* WorldContextObject);

	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static void ShutdownGame();


	UFUNCTION(Category = "UnrealEngineEx", BlueprintCallable)
	static FTransform GetTransfromInFrontOfPlayer(class AActor* PlayerPawnOrController, FVector Offset);



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
};
