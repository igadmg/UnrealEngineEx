#pragma once



template <>
class TConstCoordinateFrame<FTransform>
{
public:
	TConstCoordinateFrame(const FTransform& Object)
		: Object(Object)
	{
	}



public:
	FORCEINLINE FVector GetRelativeLocation() const { return FVector::ZeroVector; }
	FORCEINLINE FRotator GetRelativeRotation() const { return FRotator::ZeroRotator; }
	FORCEINLINE FVector GetRelativeScale() const { return FVector::OneVector; }
	FORCEINLINE FTransform GetRelativeTransform() const { return FTransform::Identity; }
	FORCEINLINE FVector GetWorldLocation() const { return Object.GetLocation(); }
	FORCEINLINE FRotator GetWorldRotation() const { return Object.Rotator(); }
	FORCEINLINE FVector GetWorldScale() const { return Object.GetScale3D(); }
	FORCEINLINE FTransform GetWorldTransform() const { return Object; }

	FORCEINLINE FVector GetForwardVector() const { return Object.GetUnitAxis(EAxis::X); }
	FORCEINLINE FVector GetRightVector() const { return Object.GetUnitAxis(EAxis::Y); }
	FORCEINLINE FVector GetUpVector() const { return Object.GetUnitAxis(EAxis::Z); }



protected:
	const FTransform& Object;
};

template <>
class TCoordinateFrame<FTransform> : public TConstCoordinateFrame<FTransform>
{
public:
	TCoordinateFrame(FTransform& Object)
		: TConstCoordinateFrame<FTransform>(Object)
		, Object(Object)
	{
	}



public:
	FORCEINLINE TCoordinateFrame<FTransform>& SetRelativeLocation(FVector NewLocation) { return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetRelativeRotation(FRotator NewRotation) { return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetRelativeRotation(FQuat NewRotation) { return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetRelativeScale(FVector NewScale) { return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetRelativeTransform(FTransform NewTransform) { return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetWorldLocation(FVector NewLocation) { Object.SetLocation(NewLocation); return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetWorldRotation(FRotator NewRotation) { Object.SetRotation(FQuat(NewRotation)); return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetWorldRotation(FQuat NewRotation) { Object.SetRotation(NewRotation); return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetWorldScale(FVector NewScale) { Object.SetScale3D(NewScale); return *this; }
	FORCEINLINE TCoordinateFrame<FTransform>& SetWorldTransform(FTransform NewTransform) { Object = NewTransform; return *this; }



protected:
	FTransform& Object;
};



template <typename T>
struct FCoordinateFrame<T, typename std::enable_if<std::is_base_of<FTransform, T>::value>::type>
{
	typedef TConstCoordinateFrame<FTransform> ConstReturnType;
	typedef TCoordinateFrame<FTransform> ReturnType;

	static TConstCoordinateFrame<FTransform> MakeConst(const T& Object) { return TConstCoordinateFrame<FTransform>(Object); }
	static TCoordinateFrame<FTransform> Make(T& Object) { return TCoordinateFrame<FTransform>(Object); }
};
