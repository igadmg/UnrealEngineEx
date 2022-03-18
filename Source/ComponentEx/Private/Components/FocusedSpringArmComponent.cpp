#include "Components/FocusedSpringArmComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "ComponentEx.final.h"

#define LOCTEXT_NAMESPACE "ComponentEx"


UFocusedSpringArmComponent::UFocusedSpringArmComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FVector UFocusedSpringArmComponent::GetFocusLocation() const
{
	return FTransform(PreviousDesiredRot, GetComponentLocation()).TransformPosition(FocusOffset);
}

FRotator UFocusedSpringArmComponent::GetFocusRotation() const
{
	return FRotationMatrix::MakeFromXZ((FocusOffset - RelativeSocketLocation).GetSafeNormal(), GetUpVector()).Rotator();
}

FTransform UFocusedSpringArmComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	auto SocketTransform = Super::GetSocketTransform(InSocketName, TransformSpace);	

	return FTransform(SocketTransform.TransformRotation(FQuat(PreviousFocusRotation)), SocketTransform.GetLocation());
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
				PreviousFocusRotation = DesiredFocusRotation;
			}
		}
		else
		{
			DesiredFocusRotation = FRotator(FMath::QInterpTo(FQuat(PreviousFocusRotation), FQuat(DesiredFocusRotation), DeltaTime, CameraRotationLagSpeed));
		}
	}
	PreviousFocusRotation = DesiredFocusRotation;
}

#undef LOCTEXT_NAMESPACE
