#include "Extensions/PlayerControllerEx.h"

#include "Engine/LocalPlayer.h"
#include "Engine/GameViewportClient.h"

#include "ComponentEx.final.h"



FVector2D FAPlayerControllerConstEx::GetViewportFoV() const
{
	if (auto PlayerCameraManager = This()->PlayerCameraManager)
	{
		float FOV = PlayerCameraManager->GetFOVAngle();

		auto ViewportSize = GetViewportSize();
		ViewportSize /= ViewportSize.X;
		return FOV * ViewportSize;
	}

	return FVector2D(1, 1);
}

FVector2D FAPlayerControllerConstEx::GetViewportSize() const
{
	if (auto LocalPlayer = This()->GetLocalPlayer())
	{
		if (auto ViewportClient = LocalPlayer->ViewportClient)
		{
			FVector2D ViewportSize;
			ViewportClient->GetViewportSize(ViewportSize);

			return ViewportSize;
		}
	}

	return FVector2D(1, 1);
}
