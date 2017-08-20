#pragma once



template <>
class TConstCoordinateFrame<UActorComponent>
{
public:
	TConstCoordinateFrame(const UActorComponent& Object)
		: Object(Object)
	{
	}



public:
	FORCEINLINE FVector GetRelativeLocation() const { return FVector::ZeroVector; }
	FORCEINLINE FRotator GetRelativeRotation() const { return FRotator::ZeroRotator; }
	FORCEINLINE FVector GetRelativeScale() const { return FVector::OneVector; }
	FORCEINLINE FTransform GetRelativeTransform() const { return FTransform::Identity; }
	FORCEINLINE FVector GetWorldLocation() const { return Object.GetOwner()->GetActorLocation(); }
	FORCEINLINE FRotator GetWorldRotation() const { return Object.GetOwner()->GetActorRotation(); }
	FORCEINLINE FVector GetWorldScale() const { return Object.GetOwner()->GetActorScale(); }
	FORCEINLINE FTransform GetWorldTransform() const { return Object.GetOwner()->GetActorTransform(); }



protected:
	const UActorComponent& Object;
};

template <>
class TCoordinateFrame<UActorComponent> : public TConstCoordinateFrame<UActorComponent>
{
public:
	TCoordinateFrame(UActorComponent& Object)
		: TConstCoordinateFrame<UActorComponent>(Object)
		, Object(Object)
	{
	}



public:
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetRelativeLocation(FVector NewLocation) { return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetRelativeRotation(FRotator NewRotation) { return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetRelativeRotation(FQuat NewRotation) { return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetRelativeScale(FVector NewScale) { return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetRelativeTransform(FTransform NewTransform) { return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetWorldLocation(FVector NewLocation) { Object.GetOwner()->SetActorLocation(NewLocation); return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetWorldRotation(FRotator NewRotation) { Object.GetOwner()->SetActorRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetWorldRotation(FQuat NewRotation) { Object.GetOwner()->SetActorRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetWorldScale(FVector NewScale) { Object.GetOwner()->SetActorScale3D(NewScale); return *this; }
	FORCEINLINE TCoordinateFrame<UActorComponent>& SetWorldTransform(FTransform NewTransform) { Object.GetOwner()->SetActorTransform(NewTransform); return *this; }



protected:
	UActorComponent& Object;
};


// 
//, std::negation<std::is_base_of<USceneComponent, T>::value>::value
template <typename T>
struct FCoordinateFrame<T, typename std::enable_if<
		std::conjunction<
			std::is_base_of<class UActorComponent, T>
			, std::negation<std::is_base_of<class USceneComponent, T>>
		>::value
	>::type>
{
	typedef TConstCoordinateFrame<UActorComponent> ConstReturnType;
	typedef TCoordinateFrame<UActorComponent> ReturnType;

	static TConstCoordinateFrame<UActorComponent> MakeConst(const T& Object) { return TConstCoordinateFrame<UActorComponent>(Object); }
	static TCoordinateFrame<UActorComponent> Make(T& Object) { return TCoordinateFrame<UActorComponent>(Object); }
};
