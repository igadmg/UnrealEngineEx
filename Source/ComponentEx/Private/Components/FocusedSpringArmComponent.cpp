#include "Components/FocusedSpringArmComponent.h"

#include "Async/AsyncTaskManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"


UFocusedSpringArmComponent::UFocusedSpringArmComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FVector UFocusedSpringArmComponent::GetFocusLocation() const
{
	return FTransform(PreviousDesiredRot, cf(this).GetWorldLocation()).TransformPosition(FocusOffset);
}

FRotator UFocusedSpringArmComponent::GetFocusRotation() const
{
	const FVector Direction = (FocusOffset - (SocketOffset - TargetArmLength * FVector::XAxisVector)).GetSafeNormal();
	return FRotationMatrix::MakeFromXZ(Direction, FVector::ZAxisVector).Rotator();
}

FFocusedSpringArmConfig UFocusedSpringArmComponent::ChangeCameraConfig(FFocusedSpringArmConfig NewCameraConfig, float Time, TEnumAsByte<EEasingFunc::Type> EasingFunc)
{
	FFocusedSpringArmConfig OldConfig = FFocusedSpringArmConfig::Make(this);
	ChangeCameraConfigInterpolationTimer = TLerpInterpolationTimer<FFocusedSpringArmConfig>(
		OldConfig, NewCameraConfig, Time, EasingFunc);

	if (auto AsyncTaskManager = XX::GetSubsystem<UAsyncTaskManager>(this))
	{
		AsyncTaskManager->TaskTickFunctions.Add(FCoreExOnAsyncTaskTickFunction::CreateWeakLambda(this, [this](float DeltaTime) {
			auto Config = ChangeCameraConfigInterpolationTimer.Advance(DeltaTime);

			CameraFOV = Config.CameraFOV;
			TargetArmLength = Config.TargetArmLength;
			SocketOffset = Config.SocketOffset;
			TargetOffset = Config.TargetOffset;
			FocusOffset = Config.FocusOffset;

			if (auto CameraManager = XX::GetPlayerCameraManager(this))
			{
				CameraManager->SetFOV(CameraFOV);
			}

			return ChangeCameraConfigInterpolationTimer.IsFinished();
		}));
	}

	return OldConfig;
}

FTransform UFocusedSpringArmComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	auto SocketTransform = Super::GetSocketTransform(InSocketName, TransformSpace);

	return FTransform(SocketTransform.TransformRotation(FQuat(PreviousFocusRotation)), SocketTransform.GetLocation());
}

void UFocusedSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFocusedSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);

	auto DesiredFocusRotation = GetFocusRotation();
	if (bDoRotationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraRotationLagSpeed > 0.f)
		{
			const auto LerpStep = (DesiredFocusRotation - PreviousFocusRotation).GetNormalized() * (1.f / DeltaTime);
			auto LerpTarget = PreviousFocusRotation;

			float RemainingTime = DeltaTime;
			while (RemainingTime > KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += LerpStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredFocusRotation = FRotator(FMath::QInterpTo(FQuat(PreviousFocusRotation), FQuat(LerpTarget), LerpAmount, CameraRotationLagSpeed));
			}
		}
		else
		{
			DesiredFocusRotation = FRotator(FMath::QInterpTo(FQuat(PreviousFocusRotation), FQuat(DesiredFocusRotation), DeltaTime, CameraRotationLagSpeed));
		}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDrawDebugLagMarkers)
		{
			auto WorldSocketLocation = cf(this).LocalToWorldPosition(RelativeSocketLocation);
			DrawDebugSphere(GetWorld(), GetFocusLocation(), 5.f, 8, FColor::Red);
			DrawDebugSphere(GetWorld(), WorldSocketLocation, 15.f, 8, FColor::Yellow);

			auto LineLength = FVector(FocusOffset - (SocketOffset - TargetArmLength * FVector::XAxisVector)).Length();
			auto SocketTransform = Super::GetSocketTransform(NAME_None, RTS_World);
			auto SocketDirection = SocketTransform.TransformVector(FVector::XAxisVector);
			auto WorldSocketDirection = SocketTransform.TransformRotation(FQuat(DesiredFocusRotation)).RotateVector(FVector::XAxisVector);
			DrawDebugDirectionalArrow(GetWorld(), WorldSocketLocation, WorldSocketLocation + WorldSocketDirection * LineLength, 7.5f, FColor::Yellow);
			DrawDebugDirectionalArrow(GetWorld(), WorldSocketLocation, WorldSocketLocation + SocketDirection * LineLength, 7.5f, FColor::Blue);

			auto RelativeSocketDirection = RelativeSocketRotation.RotateVector(FVector::XAxisVector);
			DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), GetComponentLocation() + RelativeSocketDirection * 100.f, 7.5f, FColor::Blue);
		}
#endif
	}
	PreviousFocusRotation = DesiredFocusRotation;
}

#undef LOCTEXT_NAMESPACE
