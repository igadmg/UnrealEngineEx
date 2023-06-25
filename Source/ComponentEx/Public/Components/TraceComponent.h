#pragma once

#include "Components/SceneComponent.h"

#include "TraceComponent.generated.h"


UENUM(BlueprintType)
enum class ETaceType : uint8
{
	LineSingle,
	LineMulti,
	BoxSingle,
	BoxMulti,
	SphereSingle,
	SphereMulti,
	CapsuleSingle,
	CapsuleMulti
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent = true))
class COMPONENTEX_API UTraceComponent : public USceneComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceDistance = 2000.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//ETaceType TaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTraceComplex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreSelf = true;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<AActor*> ActorsToIgnore;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor TraceColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor TraceHitColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DrawTime = 5.0f;
#endif


public:
	UFUNCTION(BlueprintCallable)
	bool LineTraceSingle(TEnumAsByte<ETraceTypeQuery> TraceChannel, TArray<AActor*> ActorsToIgnore, FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	bool CameraLineTraceSingle(TEnumAsByte<ETraceTypeQuery> TraceChannel, TArray<AActor*> ActorsToIgnore, FHitResult& HitResult);


public:
	UTraceComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

