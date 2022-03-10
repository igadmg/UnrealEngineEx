#pragma once

#include "SceneView.h"
#include "UnrealEngineExTypes.generated.h"



UENUM(BlueprintType)
enum class EAsyncTaskResult : uint8
{
	Complete,
	Abort,
	Timeout,
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
namespace EWorldType
{
	enum Type
	{
		/** An untyped world, in most cases this will be the vestigial worlds of streamed in sub-levels */
		None,

		/** The game world */
		Game,

		/** A world being edited in the editor */
		Editor,

		/** A Play In Editor world */
		PIE,

		/** A preview world for an editor tool */
		EditorPreview,

		/** A preview world for a game */
		GamePreview,

		/** A minimal RPC world for a game */
		GameRPC,

		/** An editor world that was loaded but not currently being edited in the level editor */
		Inactive
	};
}

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


	UPROPERTY(Category = "UnrealEditorEx", BlueprintReadOnly)
	float Latency = 0;

	UPROPERTY(Category = "UnrealEditorEx", BlueprintReadOnly)
	float ServerTime = 0;
};

USTRUCT(BlueprintType)
struct FViewSpaceDescriptor
{
	GENERATED_BODY()



	FViewSpaceDescriptor()
		: bIsPerspective(false)
		, bIsOrthoXY(false)
		, bIsOrthoXZ(false)
		, bIsOrthoYZ(false)
		, UniformScale(false)
		, Scale(FVector::ZeroVector)
	{
	}

	/*
	*  Creates a new FSpaceDescriptor.
	*
	*  @param View         The virtual view for the space.
	*  @param Viewport     The real viewport for the space.
	*  @param InLocation   The location of the camera in the virtual space.
	*/
	FViewSpaceDescriptor(const FSceneView* View, const FVector& InLocation)
		: bIsPerspective(View->ViewMatrices.GetProjectionMatrix().M[3][3] < 1.0f)
		, bIsOrthoXY(!bIsPerspective && FMath::Abs(View->ViewMatrices.GetViewMatrix().M[2][2]) > 0.0f)
		, bIsOrthoXZ(!bIsPerspective && FMath::Abs(View->ViewMatrices.GetViewMatrix().M[1][2]) > 0.0f)
		, bIsOrthoYZ(!bIsPerspective && FMath::Abs(View->ViewMatrices.GetViewMatrix().M[0][2]) > 0.0f)
		, UniformScale(View->WorldToScreen(InLocation).W * (4.0f / View->UnscaledViewRect.Width() / View->ViewMatrices.GetProjectionMatrix().M[0][0]))
		, Scale(CreateScale())
	{
	}



	/// Whether or not the view is perspective.
	UPROPERTY(Category = "View Space", VisibleAnywhere, BlueprintReadOnly)
	bool bIsPerspective;

	/// Whether or not the view is orthogonal to the XY plane.
	UPROPERTY(Category = "View Space", VisibleAnywhere, BlueprintReadOnly)
	bool bIsOrthoXY;

	/// Whether or not the view is orthogonal to the XZ plane.
	UPROPERTY(Category = "View Space", VisibleAnywhere, BlueprintReadOnly)
	bool bIsOrthoXZ;

	/// Whether or not the view is orthogonal to the YZ plane.
	UPROPERTY(Category = "View Space", VisibleAnywhere, BlueprintReadOnly)
	bool bIsOrthoYZ;

	/// The uniform scale for the space.
	UPROPERTY(Category = "View Space", VisibleAnywhere, BlueprintReadOnly)
	float UniformScale;

	/// The scale vector for the space based on orientation.
	UPROPERTY(Category = "View Space", VisibleAnywhere, BlueprintReadOnly)
	FVector Scale;



	bool IsValid() const { return !Scale.IsNearlyZero(); }

	/*
	*  Used to determine whether or not the X axis should be drawn.
	*
	*  @param AxisToDraw   The desired axis to draw.
	*
	*  @return True if the axis should be drawn. False otherwise.
	*/
	bool ShouldDrawAxisX(const EAxisList::Type AxisToDraw)
	{
		return ShouldDrawAxis(EAxisList::X, AxisToDraw, bIsOrthoYZ);
	}

	/*
	*  Used to determine whether or not the Y axis should be drawn.
	*
	*  @param AxisToDraw   The desired axis to draw.
	*
	*  @return True if the axis should be drawn. False otherwise.
	*/
	bool ShouldDrawAxisY(const EAxisList::Type AxisToDraw)
	{
		return ShouldDrawAxis(EAxisList::Y, AxisToDraw, bIsOrthoXZ);
	}

	/*
	*  Used to determine whether or not the Z axis should be drawn.
	*
	*  @param AxisToDraw   The desired axis to draw.
	*
	*  @return True if the axis should be drawn. False otherwise.
	*/
	bool ShouldDrawAxisZ(const EAxisList::Type AxisToDraw)
	{
		return ShouldDrawAxis(EAxisList::Z, AxisToDraw, bIsOrthoXY);
	}

private:

	/*
	*  Creates a space scale vector from the determined orientation and uniform scale.
	*
	*  @return Space scale vector.
	*/
	FVector CreateScale()
	{
		if (bIsOrthoXY)
		{
			return FVector(UniformScale, UniformScale, 1.0f);
		}
		else if (bIsOrthoXZ)
		{
			return FVector(UniformScale, 1.0f, UniformScale);
		}
		else if (bIsOrthoYZ)
		{
			return FVector(1.0f, UniformScale, UniformScale);
		}
		else
		{
			return FVector(UniformScale, UniformScale, UniformScale);
		}
	}

	/*
	*  Used to determine whether or not a specific axis should be drawn.
	*
	*  @param AxisToCheck  The axis to check.
	*  @param AxisToDraw   The desired axis to draw.
	*  @param bIsOrtho     Whether or not the axis to check is orthogonal to the viewing orientation.
	*
	*  @return True if the axis should be drawn. False otherwise.
	*/
	bool ShouldDrawAxis(const EAxisList::Type AxisToCheck, const EAxisList::Type AxisToDraw, const bool bIsOrtho)
	{
		return (AxisToCheck & AxisToDraw) && (bIsPerspective || !bIsOrtho);
	}
};


USTRUCT(BlueprintType)
struct FViewFrustum
{
	GENERATED_BODY()



	FConvexVolume FrustumVolume;
	bool bIsValid;


	FViewFrustum()
		: bIsValid(false)
	{
	}

	FViewFrustum(FConvexVolume FrustumVolume)
		: FrustumVolume(FrustumVolume)
		, bIsValid(true)
	{
	}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FUnrealEngineExOnAsyncTaskFinishedDelegate, EAsyncTaskResult, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUnrealEngineExOnLevelStreamedDelegate, class ULevelStreaming*, StreamedLevel);
