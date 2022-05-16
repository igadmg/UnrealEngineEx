#pragma once



template <>
class TConstCoordinateFrame<USceneComponent>
{
public:
	TConstCoordinateFrame(const USceneComponent& Object)
		: Object(Object)
	{
	}



public:
	FORCEINLINE FVector GetRelativeLocation() const { return Object.GetRelativeLocation(); }
	FORCEINLINE FRotator GetRelativeRotation() const { return Object.GetRelativeRotation(); }
	FORCEINLINE FVector GetRelativeScale() const { return Object.GetRelativeScale3D(); }
	FORCEINLINE FTransform GetRelativeTransform() const { return Object.GetRelativeTransform(); }
	FORCEINLINE FVector GetWorldLocation() const { return Object.GetComponentLocation(); }
	FORCEINLINE FRotator GetWorldRotation() const { return Object.GetComponentRotation(); }
	FORCEINLINE FVector GetWorldScale() const { return Object.GetComponentScale(); }
	FORCEINLINE FTransform GetWorldTransform() const { return Object.GetComponentTransform(); }

	DEFINE_CF_COMMON_FUNCTIONS()


protected:
	const USceneComponent& Object;
};

template <>
class TCoordinateFrame<USceneComponent> : public TConstCoordinateFrame<USceneComponent>
{
public:
	TCoordinateFrame(USceneComponent& Object)
		: TConstCoordinateFrame<USceneComponent>(Object)
		, Object(Object)
	{
	}



public:
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetRelativeLocation(FVector NewLocation) { Object.SetRelativeLocation(NewLocation); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetRelativeRotation(FRotator NewRotation) { Object.SetRelativeRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetRelativeRotation(FQuat NewRotation) { Object.SetRelativeRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetRelativeScale(FVector NewScale) { Object.SetRelativeScale3D(NewScale); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetRelativeTransform(FTransform NewTransform) { Object.SetRelativeTransform(NewTransform); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetWorldLocation(FVector NewLocation) { Object.SetWorldLocation(NewLocation); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetWorldRotation(FRotator NewRotation) { Object.SetWorldRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetWorldRotation(FQuat NewRotation) { Object.SetWorldRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetWorldScale(FVector NewScale) { Object.SetWorldScale3D(NewScale); return *this; }
	FORCEINLINE TCoordinateFrame<USceneComponent>& SetWorldTransform(FTransform NewTransform) { Object.SetWorldTransform(NewTransform); return *this; }



protected:
	USceneComponent& Object;
};




template <typename T>
struct FCoordinateFrame<T, typename std::enable_if<std::is_base_of<USceneComponent, T>::value>::type>
{
	typedef TConstCoordinateFrame<USceneComponent> ConstReturnType;
	typedef TCoordinateFrame<USceneComponent> ReturnType;

	static TConstCoordinateFrame<USceneComponent> MakeConst(const T& Object) { return TConstCoordinateFrame<USceneComponent>(Object); }
	static TCoordinateFrame<USceneComponent> Make(T& Object) { return TCoordinateFrame<USceneComponent>(Object); }
};
